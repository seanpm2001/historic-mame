/*
	vlm5030.c

	VLM5030 emurator

	Written by Tatsuyuki Satoh
	core decode program is used from MAME's TMS5220 emurator.

	memory cycle(sampling rate ?) = 122.9u(440clock)
	interpolator(LC8109 = 2.5ms)  = 20 * samples(125us)
	frame time  (20ms)            =  8 * interpolator

----------- command format (Analytical result) ----------

1)end of speech (8bit/frame)
:00000011:

2)silent        (8bit/frame)
:????LL01:

LL : Silent frames
     00=2 frame
     01=4 frame
     10=6 frame
     11=8 frame

3)play frame    (48bit/frame)
: 1st    :   2nd  :   3rd  :   4th  :  5th   :   6th  :
:??PPPPP0:?????EEE:????????:????????:????????:????????:

EEE   : energy ( volume )
PPPPP : pitch
?     : unnown

*/
#include "driver.h"
#include "vlm5030.h"

#define MIN_SLICE 10

#define IP_SIZE 20		/* samples per interpolator */
#define FR_SIZE 8		/* interpolator per frame   */

static int sample_pos;
static int buffer_len;
static int emulation_rate;
static struct VLM5030interface *intf;

static unsigned char *buffer;
static int channel;

static unsigned char *VLM5030_rom;
static int VLM5030_address;
static int pin_BSY;
static int pin_ST;
static int pin_RST;
static int latch_data = 0;
static int sampling_mode;

static int table_h;

#define PH_RESET 0
#define PH_IDLE  1
#define PH_SETUP 2
#define PH_WAIT  3
#define PH_RUN   4
#define PH_STOP  5
static int phase;

/* these contain data describing the current and previous voice frames */
static unsigned short old_energy = 0;
static unsigned short old_pitch = 0;
static int old_k[10] = {0,0,0,0,0,0,0,0,0,0};

static unsigned short new_energy = 0;
static unsigned short new_pitch = 0;
static int new_k[10] = {0,0,0,0,0,0,0,0,0,0};

/* these are all used to contain the current state of the sound generation */
static unsigned short current_energy = 0;
static unsigned short current_pitch = 0;
static int current_k[10] = {0,0,0,0,0,0,0,0,0,0};

static unsigned short target_energy = 0;
static unsigned short target_pitch = 0;
static int target_k[10] = {0,0,0,0,0,0,0,0,0,0};

static int interp_count = 0;       /* number of interp periods (0-7) */
static int sample_count = 0;       /* sample number within interp (0-19) */
static int pitch_count = 0;

static int u[11] = {0,0,0,0,0,0,0,0,0,0,0};
static int x[10] = {0,0,0,0,0,0,0,0,0,0};

static int randbit = 0;

/* ROM Tables */

/* !!!!!!!!!! ROM table is not correct.        !!!!!!!!!! */
/* !!!!!!!!!!  These are ROM tables of TMS5220 !!!!!!!!!! */

/* This is the energy lookup table (4-bits -> 10-bits) */

static const unsigned short energytable[0x10]={
0x0000,0x00C0,0x0140,0x01C0,0x0280,0x0380,0x0500,0x0740,
0x0A00,0x0E40,0x1440,0x1C80,0x2840,0x38C0,0x5040,0x7FC0};

/* This is the pitch lookup table (6-bits -> 8-bits) */

static const unsigned short pitchtable [0x40]={
0x0000,0x1000,0x1100,0x1200,0x1300,0x1400,0x1500,0x1600,
0x1700,0x1800,0x1900,0x1A00,0x1B00,0x1C00,0x1D00,0x1E00,
0x1F00,0x2000,0x2100,0x2200,0x2300,0x2400,0x2500,0x2600,
0x2700,0x2800,0x2900,0x2A00,0x2B00,0x2D00,0x2F00,0x3100,
0x3300,0x3500,0x3600,0x3900,0x3B00,0x3D00,0x3F00,0x4200,
0x4500,0x4700,0x4900,0x4D00,0x4F00,0x5100,0x5500,0x5700,
0x5C00,0x5F00,0x6300,0x6600,0x6A00,0x6E00,0x7300,0x7700,
0x7B00,0x8000,0x8500,0x8A00,0x8F00,0x9500,0x9A00,0xA000};

/* These are the reflection coefficient lookup tables */

/* K1 is (5-bits -> 9 bits+sign, 2's comp. fractional (-1 < x < 1) */

static const short k1table    [0x20]={
0x82C0,0x8380,0x83C0,0x8440,0x84C0,0x8540,0x8600,0x8780,
0x8880,0x8980,0x8AC0,0x8C00,0x8D40,0x8F00,0x90C0,0x92C0,
0x9900,0xA140,0xAB80,0xB840,0xC740,0xD8C0,0xEBC0,0x0000,
0x1440,0x2740,0x38C0,0x47C0,0x5480,0x5EC0,0x6700,0x6D40};

/* K2 is (5-bits -> 9 bits+sign, 2's comp. fractional (-1 < x < 1) */

static const short k2table    [0x20]={
0xAE00,0xB480,0xBB80,0xC340,0xCB80,0xD440,0xDDC0,0xE780,
0xF180,0xFBC0,0x0600,0x1040,0x1A40,0x2400,0x2D40,0x3600,
0x3E40,0x45C0,0x4CC0,0x5300,0x5880,0x5DC0,0x6240,0x6640,
0x69C0,0x6CC0,0x6F80,0x71C0,0x73C0,0x7580,0x7700,0x7E80};

/* K3 is (4-bits -> 9 bits+sign, 2's comp. fractional (-1 < x < 1) */

static const short k3table    [0x10]={
0x9200,0x9F00,0xAD00,0xBA00,0xC800,0xD500,0xE300,0xF000,
0xFE00,0x0B00,0x1900,0x2600,0x3400,0x4100,0x4F00,0x5C00};

/* K4 is (4-bits -> 9 bits+sign, 2's comp. fractional (-1 < x < 1) */

static const short k4table    [0x10]={
0xAE00,0xBC00,0xCA00,0xD800,0xE600,0xF400,0x0100,0x0F00,
0x1D00,0x2B00,0x3900,0x4700,0x5500,0x6300,0x7100,0x7E00};

/* K5 is (4-bits -> 9 bits+sign, 2's comp. fractional (-1 < x < 1) */

static const short k5table    [0x10]={
0xAE00,0xBA00,0xC500,0xD100,0xDD00,0xE800,0xF400,0xFF00,
0x0B00,0x1700,0x2200,0x2E00,0x3900,0x4500,0x5100,0x5C00};

/* K6 is (4-bits -> 9 bits+sign, 2's comp. fractional (-1 < x < 1) */

static const short k6table    [0x10]={
0xC000,0xCB00,0xD600,0xE100,0xEC00,0xF700,0x0300,0x0E00,
0x1900,0x2400,0x2F00,0x3A00,0x4500,0x5000,0x5B00,0x6600};

/* K7 is (4-bits -> 9 bits+sign, 2's comp. fractional (-1 < x < 1) */

static const short k7table    [0x10]={
0xB300,0xBF00,0xCB00,0xD700,0xE300,0xEF00,0xFB00,0x0700,
0x1300,0x1F00,0x2B00,0x3700,0x4300,0x4F00,0x5A00,0x6600};

/* K8 is (3-bits -> 9 bits+sign, 2's comp. fractional (-1 < x < 1) */

static const short k8table    [0x08]={
0xC000,0xD800,0xF000,0x0700,0x1F00,0x3700,0x4F00,0x6600};

/* K9 is (3-bits -> 9 bits+sign, 2's comp. fractional (-1 < x < 1) */

static const short k9table    [0x08]={
0xC000,0xD400,0xE800,0xFC00,0x1000,0x2500,0x3900,0x4D00};

/* K10 is (3-bits -> 9 bits+sign, 2's comp. fractional (-1 < x < 1) */

static const short k10table   [0x08]={
0xCD00,0xDF00,0xF100,0x0400,0x1600,0x2000,0x3B00,0x4D00};

/* chirp table */

static char chirptable[41]={
0x00, 0x2a, 0xd4, 0x32,
0xb2, 0x12, 0x25, 0x14,
0x02, 0xe1, 0xc5, 0x02,
0x5f, 0x5a, 0x05, 0x0f,
0x26, 0xfc, 0xa5, 0xa5,
0xd6, 0xdd, 0xdc, 0xfc,
0x25, 0x2b, 0x22, 0x21,
0x0f, 0xff, 0xf8, 0xee,
0xed, 0xef, 0xf7, 0xf6,
0xfa, 0x00, 0x03, 0x02,
0x01
};

/* interpolation coefficients */
static char interp_coeff[8] = {
8, 8, 8, 4, 4, 2, 2, 1
};


/* //////////////////////////////////////////////////////// */

/* check sample file */
static int check_samplefile(int num)
{
	if (Machine->samples == 0) return 0;
	if (Machine->samples->total <= num ) return 0;
	if (Machine->samples->sample[num] == 0) return 0;
	/* sample file is found */
	return 1;
}

static int get_bits(int sbit,int bits)
{
#if 1 /* LSB FIRST */
	int offset = VLM5030_address + (sbit>>3);
	int data;

	data = VLM5030_rom[offset] | (((int)VLM5030_rom[offset+1])<<8);
	data >>= sbit;
	data &= (0xff>>(8-bits));
#else
	int offset = VLM5030_address + (sbit>>3);
	int data;

	data = (((int)VLM5030_rom[offset])<<8) | VLM5030_rom[offset+1];
	data <<= (sbit&0x07);
	data &= 0xffff;
	data >>= (16 - bits);
#endif

	return data;
}

/* get next frame */
static int parse_frame (void)
{
	unsigned char cmd;

	/* remember previous frame */
	old_energy = new_energy;
	old_pitch = new_pitch;
	memcpy( old_k , new_k , sizeof(old_k) );
	/* command byte check */
	cmd = VLM5030_rom[VLM5030_address];
	if( cmd & 0x01 )
	{	/* extend frame */
		new_energy = new_pitch = 0;
		memset( new_k , 0 , sizeof(new_k));
		VLM5030_address++;
		if( cmd & 0x02 )
		{	/* end of speech */
			if(errorlog) fprintf(errorlog,"VLM5030 %04X end \n",VLM5030_address );
			return 0;
		}
		else
		{	/* silent frame */
			int nums = ( (cmd>>2)+1 )*2;
			if(errorlog) fprintf(errorlog,"VLM5030 %04X silent %d frame\n",VLM5030_address,nums );
			return nums * FR_SIZE;
		}
	}
	/* normal frame */
/*
0        8        16       24       32       40
:??PPPPP0:11111EEE:11112222:33334444:55556667:77889900:

PPPPP : pitch  ( maybe )
EEE   : volume (energy?)

K10 TOTAL 35bit(+2bit)

*/
	new_pitch  = pitchtable[get_bits( 1,5)<<1] / 256;
	new_energy = energytable[get_bits( 6,5)>>1] >> 6;

	/* 10 K's */
	new_k[0] = k1table[get_bits(11,5)];
	new_k[1] = k2table[get_bits(16,4)<<0];
	new_k[2] = k3table[get_bits(20,4)<<0];
	new_k[3] = k4table[get_bits(24,4)<<0];
	new_k[4] = k5table[get_bits(28,4)<<0];
	new_k[5] = k6table[get_bits(32,4)<<0];
	new_k[6] = k7table[get_bits(36,4)<<0];
	new_k[7] = k8table[get_bits(40,3)<<0];
	new_k[8] = k9table[get_bits(43,3)<<0];
	new_k[9] = k10table[get_bits(46,2)<<1];

	VLM5030_address+=6;
	if(errorlog) fprintf(errorlog,"VLM5030 %04X voice \n",VLM5030_address );
	return FR_SIZE;
}

/* decode and buffering data */
static void vlm5030_process(unsigned char *buffer, int size)
{
	int buf_count=0;
	int interp_effect;

	/* running */
	if( phase == PH_RUN )
	{
		/* playing speech */
		while (size > 0)
		{
			int current_val;

			/* check new interpolator or  new frame */
			if( sample_count == 0 )
			{
				sample_count = IP_SIZE;
				/* interpolator changes */
				if ( interp_count == 0 )
				{
					/* change to new frame */
					interp_count = parse_frame(); /* with change phase */
					if ( interp_count == 0 )
					{
						sample_count = 160; /* end -> stop time */
						phase = PH_STOP;
						goto phase_stop; /* continue to stop phase */
					}
					/* Set old target as new start of frame */
					current_energy = old_energy;
					current_pitch = old_pitch;
					memcpy( current_k , old_k , sizeof(current_k) );
					/* is this a zero energy frame? */
					if (current_energy == 0)
					{
						/*printf("processing frame: zero energy\n");*/
						target_energy = 0;
						target_pitch = current_pitch;
						memcpy( target_k , current_k , sizeof(target_k) );
					}
					else
					{
						/*printf("processing frame: Normal\n");*/
						/*printf("*** Energy = %d\n",current_energy);*/
						/*printf("proc: %d %d\n",last_fbuf_head,fbuf_head);*/
						target_energy = new_energy;
						target_pitch = new_pitch;
						memcpy( target_k , new_k , sizeof(target_k) );
					}
				}
				/* next interpolator */
				/* Update values based on step values */
				/*printf("\n");*/
				interp_effect = (int)(interp_coeff[FR_SIZE - (interp_count%FR_SIZE)]);

				current_energy += (target_energy - current_energy) / interp_effect;
				if (old_pitch != 0)
					current_pitch += (target_pitch - current_pitch) / interp_effect;
				/*printf("*** Energy = %d\n",current_energy);*/
				current_k[0] += (target_k[0] - current_k[0]) / interp_effect;
				current_k[1] += (target_k[1] - current_k[1]) / interp_effect;
				current_k[2] += (target_k[2] - current_k[2]) / interp_effect;
				current_k[3] += (target_k[3] - current_k[3]) / interp_effect;
				current_k[4] += (target_k[4] - current_k[4]) / interp_effect;
				current_k[5] += (target_k[5] - current_k[5]) / interp_effect;
				current_k[6] += (target_k[6] - current_k[6]) / interp_effect;
				current_k[7] += (target_k[7] - current_k[7]) / interp_effect;
				current_k[8] += (target_k[8] - current_k[8]) / interp_effect;
				current_k[9] += (target_k[9] - current_k[9]) / interp_effect;
				interp_count --;
			}
			/* calcrate digital filter */
			if (old_energy == 0)
			{
				/* generate silent samples here */
				current_val = 0x00;
			}
			else if (old_pitch == 0)
			{
				/* generate unvoiced samples here */
				randbit = (rand () % 2) * 2 - 1;
				current_val = (randbit * current_energy) / 4;
			}
			else
			{
				/* generate voiced samples here */
				if (pitch_count < sizeof (chirptable))
					current_val = (chirptable[pitch_count] * current_energy) / 256;
				else
					current_val = 0x00;
			}

			/* Lattice filter here */
			u[10] = current_val;
			u[9] = u[10] - ((current_k[9] * x[9]) / 32768);
			u[8] = u[ 9] - ((current_k[8] * x[8]) / 32768);
			u[7] = u[ 8] - ((current_k[7] * x[7]) / 32768);
			u[6] = u[ 7] - ((current_k[6] * x[6]) / 32768);
			u[5] = u[ 6] - ((current_k[5] * x[5]) / 32768);
			u[4] = u[ 5] - ((current_k[4] * x[4]) / 32768);
			u[3] = u[ 4] - ((current_k[3] * x[3]) / 32768);
			u[2] = u[ 3] - ((current_k[2] * x[2]) / 32768);
			u[1] = u[ 2] - ((current_k[1] * x[1]) / 32768);
			u[0] = u[ 1] - ((current_k[0] * x[0]) / 32768);

			x[9] = x[8] + ((current_k[8] * u[8]) / 32768);
			x[8] = x[7] + ((current_k[7] * u[7]) / 32768);
			x[7] = x[6] + ((current_k[6] * u[6]) / 32768);
			x[6] = x[5] + ((current_k[5] * u[5]) / 32768);
			x[5] = x[4] + ((current_k[4] * u[4]) / 32768);
			x[4] = x[3] + ((current_k[3] * u[3]) / 32768);
			x[3] = x[2] + ((current_k[2] * u[2]) / 32768);
			x[2] = x[1] + ((current_k[1] * u[1]) / 32768);
			x[1] = x[0] + ((current_k[0] * u[0]) / 32768);
			x[0] = u[0];
			/* clipping, buffering */
			if (u[0] > 511)
				buffer[buf_count] = 127;
			else if (u[0] < -512)
				buffer[buf_count] = -128;
			else
				buffer[buf_count] = u[0] >> 2;
			buf_count++;

			/* sample count */
			sample_count--;
			/* pitch */
			pitch_count++;
			if (pitch_count >= current_pitch )
				pitch_count = 0;
			/* size */
			size--;
		}
/*		return;*/
	}
	/* stop phase */
phase_stop:
	switch( phase )
	{
	case PH_SETUP:
		sample_count -= size;
		if( sample_count <= 0 )
		{
			if(errorlog) fprintf(errorlog,"VLM5030 BSY=H\n" );
			pin_BSY = 1;
			phase = PH_WAIT;
		}
		break;
	case PH_STOP:
		sample_count -= size;
		if( sample_count <= 0 )
		{
			if(errorlog) fprintf(errorlog,"VLM5030 BSY=L\n" );
			pin_BSY = 0;
			phase = PH_IDLE;
		}
	}
	/* silent buffering */
	while (size > 0)
	{
		buffer[buf_count++] = 0x00;
		size--;
	}
}

/* realtime update */
void VLM5030_update(void)
{
	int newpos;

	if (Machine->sample_rate == 0) return;

	if( !sampling_mode )
	{
		/* docode mode */
		newpos = cpu_scalebyfcount(buffer_len);	/* get current position based on the timer */

		if (newpos - sample_pos < MIN_SLICE)
			return;
		if (sample_pos < buffer_len)
			vlm5030_process (buffer + sample_pos, newpos - sample_pos);

		sample_pos = newpos;
	}
	else
	{
		/* sampling mode (check  busy flag) */
		if( pin_ST == 0 && pin_BSY == 1 )
		{
			if( osd_get_sample_status(channel) )
				pin_BSY = 0;
		}
	}
}

/* get BSY pin level */
int VLM5030_BSY(void)
{
	VLM5030_update();
	return pin_BSY;
}

/* latch contoll data */
void VLM5030_data_w(int offset,int data)
{
	latch_data = data;
}

/* set RST pin level : reset / set table address A8-A15 */
void VLM5030_RST (int pin )
{
	if( pin_RST )
	{
		if( !pin )
		{	/* H -> L : latch high address table */
			pin_RST = 0;
/*			table_h = latch_data * 256; */
			table_h = 0;
		}
	}
	else
	{
		if( pin )
		{	/* L -> H : reset chip */
			pin_RST = 1;
			if( pin_BSY )
			{
				if( sampling_mode )
					osd_stop_sample( channel );
				phase = PH_RESET;
				pin_BSY = 0;
			}
		}
	}
}

/* set VCU pin level : ?? unknown */
void VLM5030_VCU(int pin)
{
	/* unknown */
	intf->vcu = pin;
	return;
}

/* set ST pin level  : set table address A0-A7 / start speech */
void VLM5030_ST(int pin )
{
	int table = table_h | latch_data;

	if( pin_ST != pin )
	{
		/* pin level is change */
		if( !pin )
		{	/* H -> L */
			pin_ST = 0;
			/* start speech */

			if (Machine->sample_rate == 0)
			{
				pin_BSY = 0;
				return;
			}
			/* set play mode samplingfile or emulate */
			sampling_mode = check_samplefile(table/2);
			if( !sampling_mode )
			{
				VLM5030_update();

				if(errorlog) fprintf(errorlog,"VLM5030 %02X start adr=%04X\n",table/2,VLM5030_address );

				/* docode mode */
				VLM5030_address = (((int)VLM5030_rom[table])<<8)|VLM5030_rom[table+1];
				/* reset process status */
				interp_count = sample_count = 0;
				/* clear filter */
				/* after 3 sampling start */
				phase = PH_RUN;
			}
			else
			{
				/* sampling mode */
				int num = table>>1;

				osd_play_sample(channel,
					Machine->samples->sample[num]->data,
					Machine->samples->sample[num]->length,
					Machine->samples->sample[num]->smpfreq,
					Machine->samples->sample[num]->volume,
					0);
					pin_BSY = 1;
			}
		}
		else
		{	/* L -> H */
			pin_ST = 1;
			/* setup speech , BSY on after 30ms? */
			phase = PH_SETUP;
			sample_count = 1; /* wait time for busy on */
		}
	}
}

/* start VLM5030 with sound rom              */
/* speech_rom == 0 -> use sampling data mode */
int VLM5030_sh_start( struct VLM5030interface *interface )
{
    intf = interface;

	buffer_len = intf->clock / 440 / Machine->drv->frames_per_second;
	emulation_rate = buffer_len * Machine->drv->frames_per_second;
	sample_pos = 0;
	pin_BSY = pin_RST = pin_ST  = 0;
/*	VLM5030_VCU(intf->vcu); */

	VLM5030_rom = Machine->memory_region[intf->memory_region];
	if ((buffer = malloc(buffer_len)) == 0)
	{
		return 1;
	}
	memset(buffer,0x80,buffer_len);

	channel = get_play_channels(1);
	return 0;
}

/* update VLM5030 */
void VLM5030_sh_update( void )
{
	if (Machine->sample_rate == 0) return;

	if( !sampling_mode )
	{
		if (sample_pos < buffer_len)
			vlm5030_process (buffer + sample_pos, buffer_len - sample_pos);
		sample_pos = 0;
		osd_play_streamed_sample(channel,(signed char *)buffer,buffer_len,emulation_rate,intf->volume);
	}
}

/* update VLM5030 */
void VLM5030_sh_stop( void )
{
	if (buffer != 0 )
	{
    	free( buffer );
    	buffer = 0;
	}
}