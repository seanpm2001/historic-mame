/***************************************************************************

Crazy Climber memory map (preliminary)
as described by Lionel Theunissen (lionelth@ozemail.com.au)

0000h-4fffh ;20k program ROMs. ROM11=0000h
                               ROM10=1000h
                               ROM09=2000h
                               ROM08=3000h
                               ROM07=4000h

8000h-83ffh ;1k scratchpad RAM.
8800h-88ffh ;256 bytes Bigsprite RAM.
9000h-93ffh ;1k screen RAM.
9800h-981fh ;Column smooth scroll position. Corresponds to each char
             column.

9880h-989fh ;Sprite controls. 8 groups of 4 bytes:
  1st byte; code/attribute.
            Bits 0-5: sprite code.
            Bit    6: x invert.
            Bit    7: y invert.
  2nd byte ;color.
            Bits 0-3: colour. (palette scheme 0-15)
            Bit    4: 0=charset1, 1 =charset 2.
  3rd byte ;y position
  4th byte ;x position

98ddh ;Bigsprite colour/attribute.
            Bit 0-2: Big sprite colour.
            Bit   4: x invert.
            Bit   5: y invert. (not used by CC)
98deh ;Bigsprite y position.
98dfh ;Bigsprite x position.

9c00h-9fffh ;1/2k colour RAM: Bits 0-3: colour. (palette scheme 0-15)
                              Bit    4: 0=charset1, 1=charset2.
                              Bit    5: (not used by CC)
                              Bit    6: x invert.
                              Bit    7: y invert. (not used by CC)

a000h ;RD: Player 1 controls.
            Bit 0: Left up
                1: Left down
                2: Left left
                3: Left right
                4: Right up
                5: Right down
                6: Right left
                7: Right right

a000h ;WR: Non Maskable interrupt.
            Bit 0: 0=NMI disable, 1=NMI enable.

a001h ;WR: Horizontal video direction (for table model).
            Bit 0: 0=Normal, 1=invert.

a002h ;WR: Vertical video direction (for table model).
            Bit 0: 0=Normal, 1=invert.

a004h ;WR: Sample trigger.
            Bit 0: 0=Trigger.

a800h ;RD: Player 2 controls (table model only).
            Bit 0: Left up
                1: Left down
                2: Left left
                3: Left right
                4: Right up
                5: Right down
                6: Right left
                7: Right right


a800h ;WR: Sample rate speed.
              Full byte value (0-255).

b000h ;RD: DIP switches.
            Bit 1,0: Number of climbers.
                     00=3, 01=4, 10=5, 11=6.
            Bit   2: Extra climber bonus.
                     0=30000, 1=50000.
            Bit   3: 1=Test Pattern
            Bit 5,4: Coins per credit.
                     00=1, 01=2, 10=3 11=4.
            Bit 7,6: Plays per credit.
                     00=1, 01=2, 10=3, 11=Freeplay.

b000h ;WR: Sample volume.
            Bits 0-5: Volume (0-31).

b800h ;RD: Machine switches.
            Bit 0: Coin 1.
            Bit 1: Coin 2.
            Bit 2: 1 Player start.
            Bit 3: 2 Player start.
            Bit 4: Upright/table select.
                   0=table, 1=upright.


I/O 8  ;AY-3-8910 Control Reg.
I/O 9  ;AY-3-8910 Data Write Reg.
I/O C  ;AY-3-8910 Data Read Reg.
        Port A of the 8910 selects the digital sample to play

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"
#include "sndhrdw/8910intf.h"



extern unsigned char *cclimber_bsvideoram;
extern unsigned char *cclimber_bigspriteram;
extern unsigned char *cclimber_column_scroll;
extern void cclimber_colorram_w(int offset,int data);
extern void cclimber_bigsprite_videoram_w(int offset,int data);
extern void cclimber_vh_convert_color_prom(unsigned char *palette, unsigned char *colortable,const unsigned char *color_prom);
extern int cclimber_vh_start(void);
extern void cclimber_vh_stop(void);
extern void cclimber_vh_screenrefresh(struct osd_bitmap *bitmap);

extern void cclimber_sample_trigger_w(int offset,int data);
extern void cclimber_sample_rate_w(int offset,int data);
extern void cclimber_sample_volume_w(int offset,int data);
extern int cclimber_sh_start(void);



static struct MemoryReadAddress readmem[] =
{
	{ 0x8000, 0x83ff, MRA_RAM },
	{ 0x0000, 0x4fff, MRA_ROM },
	{ 0x9880, 0x989f, MRA_RAM },	/* sprite registers */
	{ 0x98dc, 0x98df, MRA_RAM },	/* bigsprite registers */
	{ 0xa000, 0xa000, input_port_0_r },	/* IN0 */
	{ 0xa800, 0xa800, input_port_1_r },	/* IN1 */
	{ 0xb000, 0xb000, input_port_2_r },	/* DSW1 */
	{ 0xb800, 0xb800, input_port_3_r },	/* IN2 */
	{ 0x9000, 0x93ff, MRA_RAM },	/* video RAM */
	{ 0x9c00, 0x9fff, MRA_RAM },	/* color RAM */
	{ 0x9800, 0x981f, MRA_RAM },	/* column scroll registers */
	{ -1 }	/* end of table */
};

static struct MemoryWriteAddress writemem[] =
{
	{ 0x8000, 0x83ff, MWA_RAM },
	{ 0x9880, 0x989f, MWA_RAM, &spriteram },
	{ 0xa000, 0xa000, interrupt_enable_w },
	{ 0x9000, 0x93ff, videoram_w, &videoram },
	{ 0x9800, 0x981f, MWA_RAM, &cclimber_column_scroll },
	{ 0x9c00, 0x9fff, cclimber_colorram_w, &colorram },
	{ 0x8800, 0x88ff, cclimber_bigsprite_videoram_w, &cclimber_bsvideoram },
	{ 0x9400, 0x97ff, videoram_w },	/* mirror address, used to draw windows */
	{ 0x98dc, 0x98df, MWA_RAM, &cclimber_bigspriteram },
	{ 0xa004, 0xa004, cclimber_sample_trigger_w },
	{ 0xb000, 0xb000, cclimber_sample_volume_w },
	{ 0xa800, 0xa800, cclimber_sample_rate_w },
	{ 0xa001, 0xa002, MWA_NOP },
	{ 0x0000, 0x4fff, MWA_ROM },
	{ -1 }	/* end of table */
};

static struct IOReadPort readport[] =
{
	{ 0x0c, 0x0c, AY8910_read_port_0_r },
	{ -1 }	/* end of table */
};

static struct IOWritePort writeport[] =
{
	{ 0x08, 0x08, AY8910_control_port_0_w },
	{ 0x09, 0x09, AY8910_write_port_0_w },
	{ -1 }	/* end of table */
};



static struct InputPort input_ports[] =
{
	{	/* IN0 */
		0x00,
		{ OSD_KEY_E, OSD_KEY_D, OSD_KEY_S, OSD_KEY_F,
				OSD_KEY_I, OSD_KEY_K, OSD_KEY_J, OSD_KEY_L },
		{ OSD_JOY_UP, OSD_JOY_DOWN, OSD_JOY_LEFT, OSD_JOY_RIGHT,
				OSD_JOY_FIRE2, OSD_JOY_FIRE3, OSD_JOY_FIRE1, OSD_JOY_FIRE4 }
	},
	{	/* IN1 */
		0x00,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{	/* DSW1 */
		0x00,
		{ 0, 0, 0, OSD_KEY_F1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{	/* IN2 */
		0x10,	/* standup cabinet */
		{ 0, OSD_KEY_3, OSD_KEY_1, OSD_KEY_2, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{ -1 }	/* end of table */
};


static struct KEYSet keys[] =
{
        { 0, 0, "JOY1 MOVE UP" },
        { 0, 2, "JOY1 MOVE LEFT"  },
        { 0, 3, "JOY1 MOVE RIGHT" },
        { 0, 1, "JOY1 MOVE DOWN" },
        { 0, 4, "JOY2 MOVE UP" },
        { 0, 6, "JOY2 MOVE LEFT"  },
        { 0, 7, "JOY2 MOVE RIGHT" },
        { 0, 5, "JOY2 MOVE DOWN" },
        { -1 }
};


static struct DSW dsw[] =
{
	{ 2, 0x03, "LIVES", { "3", "4", "5", "6" } },
	{ 2, 0x04, "BONUS", { "30000", "50000" } },
	{ -1 }
};



static struct GfxLayout charlayout =
{
	8,8,	/* 8*8 characters */
	256,	/* 256 characters */
	2,	/* 2 bits per pixel */
	{ 0, 256*8*8 },	/* the two bitplanes are separated */
	{ 0, 1, 2, 3, 4, 5, 6, 7 },	/* pretty straightforward layout */
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	8*8	/* every char takes 8 consecutive bytes */
};
static struct GfxLayout spritelayout =
{
	16,16,	/* 16*16 sprites */
	64,	/* 64 sprites */
	2,	/* 2 bits per pixel */
	{ 0, 64*16*16 },	/* the two bitplanes are separated */
	{ 0, 1, 2, 3, 4, 5, 6, 7,	/* pretty straightforward layout */
			8*8+0, 8*8+1, 8*8+2, 8*8+3, 8*8+4, 8*8+5, 8*8+6, 8*8+7 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8,
			16*8, 17*8, 18*8, 19*8, 20*8, 21*8, 22*8, 23*8 },
	32*8	/* every sprite takes 32 consecutive bytes */
};



static struct GfxDecodeInfo gfxdecodeinfo[] =
{
	{ 1, 0x0000, &charlayout,     0, 16 },	/* char set #1 */
	{ 1, 0x1000, &charlayout,     0, 16 },	/* char set #2 */
	{ 1, 0x2000, &charlayout,  4*16,  8 },	/* big sprite char set */
	{ 1, 0x0000, &spritelayout,   0, 16 },	/* sprite set #1 */
	{ 1, 0x1000, &spritelayout,   0, 16 },	/* sprite set #2 */
	{ -1 } /* end of array */
};



static unsigned char color_prom[] =
{
	/* char palette */
	0x00,0x79,0x04,0x87,0x00,0xb7,0xff,0x5f,0x00,0xc0,0xe8,0xf4,0x00,0x3f,0x04,0x38,
	0x00,0x0d,0x7a,0xb7,0x00,0x07,0x26,0x02,0x00,0x27,0x16,0x30,0x00,0xb7,0xf4,0x0c,
	0x00,0x4f,0xf6,0x24,0x00,0xb6,0xff,0x5f,0x00,0x33,0x00,0xb7,0x00,0x66,0x00,0x3a,
	0x00,0xc0,0x3f,0xb7,0x00,0x20,0xf4,0x16,0x00,0xff,0x7f,0x87,0x00,0xb6,0xf4,0xc0,
	/* bigsprite palette */
	0x00,0xff,0x18,0xc0,0x00,0xff,0xc6,0x8f,0x00,0x0f,0xff,0x1e,0x00,0xff,0xc0,0x67,
	0x00,0x47,0x7f,0x80,0x00,0x88,0x47,0x7f,0x00,0x7f,0x88,0x47,0x00,0x40,0x08,0xff
};



static struct MachineDriver machine_driver =
{
	/* basic machine hardware */
	{
		{
			CPU_Z80,
			3072000,	/* 3.072 Mhz */
			0,
			readmem,writemem,readport,writeport,
			nmi_interrupt,1
		}
	},
	60,
	0,

	/* video hardware */
	32*8, 32*8, { 0*8, 32*8-1, 2*8, 30*8-1 },
	gfxdecodeinfo,
	96,4*24,
	cclimber_vh_convert_color_prom,

	0,
	cclimber_vh_start,
	cclimber_vh_stop,
	cclimber_vh_screenrefresh,

	/* sound hardware */
	0,
	0,
	cclimber_sh_start,
	AY8910_sh_stop,
	AY8910_sh_update
};



/***************************************************************************

  Game driver(s)

***************************************************************************/

ROM_START( cclimber_rom )
	ROM_REGION(0x10000)	/* 64k for code */
	ROM_LOAD( "cc11", 0x0000, 0x1000 )
	ROM_LOAD( "cc10", 0x1000, 0x1000 )
	ROM_LOAD( "cc09", 0x2000, 0x1000 )
	ROM_LOAD( "cc08", 0x3000, 0x1000 )
	ROM_LOAD( "cc07", 0x4000, 0x1000 )

	ROM_REGION(0x3000)	/* temporary space for graphics (disposed after conversion) */
	ROM_LOAD( "cc06", 0x0000, 0x0800 )
	ROM_LOAD( "cc04", 0x0800, 0x0800 )
	ROM_LOAD( "cc05", 0x1000, 0x0800 )
	ROM_LOAD( "cc03", 0x1800, 0x0800 )
	ROM_LOAD( "cc02", 0x2000, 0x0800 )
	ROM_LOAD( "cc01", 0x2800, 0x0800 )

	ROM_REGION(0x2000)	/* samples */
	ROM_LOAD( "cc13", 0x0000, 0x1000 )
	ROM_LOAD( "cc12", 0x1000, 0x1000 )
ROM_END

ROM_START( ccjap_rom )
	ROM_REGION(0x10000)	/* 64k for code */
	ROM_LOAD( "cc11j.bin", 0x0000, 0x1000 )
	ROM_LOAD( "cc10j.bin", 0x1000, 0x1000 )
	ROM_LOAD( "cc09j.bin", 0x2000, 0x1000 )
	ROM_LOAD( "cc08j.bin", 0x3000, 0x1000 )
	ROM_LOAD( "cc07j.bin", 0x4000, 0x1000 )

	ROM_REGION(0x3000)	/* temporary space for graphics (disposed after conversion) */
	ROM_LOAD( "cc06j.bin", 0x0000, 0x0800 )
	ROM_LOAD( "cc04j.bin", 0x0800, 0x0800 )
	ROM_LOAD( "cc05j.bin", 0x1000, 0x0800 )
	ROM_LOAD( "cc03j.bin", 0x1800, 0x0800 )
	ROM_LOAD( "cc02j.bin", 0x2000, 0x0800 )
	ROM_LOAD( "cc01j.bin", 0x2800, 0x0800 )

	ROM_REGION(0x2000)	/* samples */
	ROM_LOAD( "cc13j.bin", 0x0000, 0x1000 )
	ROM_LOAD( "cc12j.bin", 0x1000, 0x1000 )
ROM_END

ROM_START( ccboot_rom )
	ROM_REGION(0x10000)	/* 64k for code */
	ROM_LOAD( "m11.bin", 0x0000, 0x1000 )
	ROM_LOAD( "m10.bin", 0x1000, 0x1000 )
	ROM_LOAD( "m09.bin", 0x2000, 0x1000 )
	ROM_LOAD( "m08.bin", 0x3000, 0x1000 )
	ROM_LOAD( "m07.bin", 0x4000, 0x1000 )

	ROM_REGION(0x3000)	/* temporary space for graphics (disposed after conversion) */
	ROM_LOAD( "m06.bin", 0x0000, 0x0800 )
	ROM_LOAD( "m04.bin", 0x0800, 0x0800 )
	ROM_LOAD( "m05.bin", 0x1000, 0x0800 )
	ROM_LOAD( "m03.bin", 0x1800, 0x0800 )
	ROM_LOAD( "m02.bin", 0x2000, 0x0800 )
	ROM_LOAD( "m01.bin", 0x2800, 0x0800 )

	ROM_REGION(0x2000)	/* samples */
	ROM_LOAD( "m13.bin", 0x0000, 0x1000 )
	ROM_LOAD( "m12.bin", 0x1000, 0x1000 )
ROM_END



static unsigned cclimber_decode(int A)
{
	static const unsigned char evetab[] =
	{
		0x44,0x14,0x47,0x12,0x54,0x10,0x13,0x46,0x4c,0x1c,0x4f,0x1a,0x5c,0x18,0x1b,0x4e,
		0x11,0x41,0x07,0x52,0x05,0x50,0x53,0x06,0x19,  -1,0x0f,0x5a,0x0d,0x58,0x5b,0x0e,
		0x64,0x34,0x67,0x32,0x74,0x30,0x33,0x66,0x6c,0x3c,0x6f,0x3a,0x7c,0x38,0x3b,0x6e,
		0x31,  -1,0x27,0x72,0x25,0x70,0x73,0x26,0x39,  -1,0x2f,0x7a,0x2d,0x78,0x7b,0x2e,
		0x51,0x00,0x43,0x16,0x40,0x55,0x17,0x42,0x59,0x08,0x4b,0x1e,0x48,0x5d,0x1f,0x4a,
		  -1,0x04,0x03,0x56,0x01,0x15,0x57,0x02,  -1,  -1,0x0b,0x5e,0x09,0x1d,0x5f,0x0a,
		0x71,0x20,0x63,0x36,0x60,0x75,0x37,0x62,0x79,0x28,0x6b,0x3e,0x68,0x7d,0x3f,0x6a,
		  -1,  -1,0x23,0x76,0x21,0x35,0x77,0x22,  -1,  -1,0x2b,0x7e,0x29,0x3d,0x7f,0x2a,
		0xd4,0xd1,0xd3,0x86,0x95,0xc5,0x92,  -1,0xdc,0xd9,0xdb,  -1,0x9d,0xcd,0x9a,  -1,
		0xc4,0x81,0xd2,0xc2,0x91,0xc1,0x82,  -1,0xcc,  -1,0xda,0xca,0x99,0xc9,0x8a,  -1,
		0xf4,0xf1,0xf3,  -1,0xb5,0xe5,0xb2,  -1,0xfc,0xf9,0xfb,  -1,0xbd,0xed,0xba,  -1,
		0xe4,  -1,0xf2,0xe2,0xb1,0xe1,0xa2,  -1,0xec,  -1,0xfa,0xea,0xb9,0xe9,0xaa,  -1,
		0x84,0xd5,0xc3,0x83,0xd0,  -1,0x87,0x97,0x8c,0xdd,0xcb,  -1,0xd8,  -1,0x8f,0x9f,
		0x80,0x90,0x93,0x96,0xc0,  -1,0xc6,0xd6,0x88,0x98,0x9b,0x9e,0xc8,  -1,0xce,0xde,
		0xa4,0xf5,0xe3,  -1,0xf0,  -1,0xa7,0xb7,0xac,0xfd,0xeb,  -1,0xf8,  -1,0xaf,0xbf,
		0xa0,0xb0,0xb3,0xb6,0xe0,  -1,0xe6,0xf6,0xa8,0xb8,0xbb,0xbe,0xe8,  -1,0xee,0xfe
	};
	static const unsigned char oddtab[] =
	{
		0x44,0x10,0x06,0x53,0x15,0x55,0x47,0x02,0x4c,0x18,0x0e,0x5b,0x1d,0x5d,0x4f,0x0a,
		0x00,0x41,0x46,0x12,  -1,0x51,  -1,0x57,0x08,  -1,0x4e,0x1a,  -1,0x59,  -1,0x5f,
		0x64,0x30,0x26,0x73,0x35,0x75,0x67,0x22,0x6c,0x38,0x2e,0x7b,0x3d,0x7d,0x6f,0x2a,
		0x20,  -1,0x66,0x32,  -1,0x71,  -1,0x77,0x28,  -1,0x6e,0x3a,  -1,0x79,  -1,0x7f,
		0x14,0x45,0x13,0x56,0x11,0x50,0x52,0x42,0x1c,0x4d,0x1b,0x5e,0x19,0x58,0x5a,  -1,
		0x01,0x54,0x07,  -1,0x04,0x05,0x16,0x03,0x09,0x5c,0x0f,  -1,0x0c,0x0d,0x1e,0x0b,
		0x34,0x65,0x33,0x76,0x31,0x70,0x72,  -1,0x3c,0x6d,0x3b,0x7e,0x39,0x78,0x7a,  -1,
		0x21,0x74,0x27,  -1,0x24,0x25,0x36,0x23,0x29,0x7c,0x2f,  -1,0x2c,0x2d,0x3e,0x2b,
		  -1,  -1,  -1,  -1,  -1,  -1,0xd6,0x83,  -1,0xdc,  -1,  -1,  -1,0xd8,0xde,  -1,
		0xd1,0x81,0x97,0xc2,  -1,0xc0,0xc7,0xc3,0xd9,  -1,0x9f,0xca,  -1,0xc8,  -1,0xcb,
		  -1,  -1,  -1,  -1,0xb4,  -1,0xf6,  -1,  -1,  -1,  -1,  -1,  -1,  -1,0xfe,  -1,
		0xf1,  -1,0xb7,0xe2,  -1,0xe0,  -1,0xe3,0xf9,  -1,0xbf,0xea,  -1,0xe8,  -1,0xeb,
		0xc1,0x90,0xd3,0x86,0x80,0xd5,0xd2,0x87,0xc9,0x98,0xdb,0x8e,  -1,0xdd,0xda,0x8f,
		0x85,0xc4,0x93,0xc6,0x91,0xc5,0x92,0x96,  -1,0xcc,0x9b,0xce,0x99,0xcd,  -1,0x9e,
		0xe1,0xb0,0xf3,0xa6,  -1,0xf5,0xf2,0xa7,0xe9,0xb8,0xfb,0xae,  -1,0xfd,0xfa,0xaf,
		  -1,0xe4,0xb3,0xe6,0xb1,0xe5,  -1,0xb6,  -1,0xec,0xbb,0xee,0xb9,0xed,  -1,0xbe
	};


	if (A & 1) return oddtab[RAM[A]];
	else return evetab[RAM[A]];
}

static unsigned ccjap_decode(int A)
{
	static const unsigned char evetab[] =
	{
		0x41,0x54,0x46,0x13,0x51,0x14,0x02,0x52,0x49,0x5c,0x4e,0x1b,0x59,0x1c,0x0a,0x5a,
		0x05,0x10,0x43,0x56,0x01,0x55,0x06,0x16,0x0d,0x18,0x4b,0x5e,0x09,0x5d,0x0e,0x1e,
		0x61,0x74,0x66,0x33,0x71,0x34,0x22,0x72,0x69,0x7c,0x6e,0x3b,0x79,0x3c,0x2a,0x7a,
		0x25,0x30,0x63,0x76,0x21,0x75,0x26,0x36,0x2d,0x38,0x6b,0x7e,0x29,0x7d,0x2e,0x3e,
		0x44,0x11,0x17,0x42,0x00,0x50,0x53,0x57,0x4c,0x19,0x1f,0x4a,0x08,0x58,0x5b,0x5f,
		0x15,0x40,0x07,0x12,0x04,0x45,0x03,0x47,0x1d,0x48,0x0f,0x1a,0x0c,0x4d,0x0b,0x4f,
		0x64,0x31,0x37,0x62,0x20,0x70,0x73,0x77,0x6c,0x39,0x3f,0x6a,0x28,0x78,0x7b,0x7f,
		0x35,0x60,0x27,0x32,0x24,0x65,0x23,0x67,0x3d,0x68,0x2f,0x3a,0x2c,0x6d,0x2b,0x6f,
		0x94,0xc1,0x87,0xd7,0x81,0xc4,0x82,0xd2,0x9c,0xc9,0x8f,0xdf,0x89,0xcc,0x8a,0xda,
		0x84,0xd0,0x93,0xc2,0xd1,0xc5,0xd6,0x96,0x8c,0xd8,0x9b,0xca,0xd9,0xcd,0xde,0x9e,
		0xb4,0xe1,0xa7,0xf7,0xa1,0xe4,0xa2,0xf2,0xbc,0xe9,0xaf,0xff,0xa9,0xec,0xaa,0xfa,
		0xa4,0xf0,0xb3,0xe2,0xf1,0xe5,0xf6,0xb6,0xac,0xf8,0xbb,0xea,0xf9,0xed,0xfe,0xbe,
		0x91,0xc0,0xc7,0xd3,0xd4,0x95,0x92,0x86,0x99,0xc8,0xcf,0xdb,0xdc,0x9d,0x9a,0x8e,
		0x90,0x80,0xc6,0x83,0xd5,0x85,0xc3,0x97,0x98,0x88,0xce,0x8b,0xdd,0x8d,0xcb,0x9f,
		0xb1,0xe0,0xe7,0xf3,0xf4,0xb5,0xb2,0xa6,0xb9,0xe8,0xef,0xfb,0xfc,0xbd,0xba,0xae,
		0xb0,0xa0,0xe6,0xa3,0xf5,0xa5,0xe3,0xb7,0xb8,0xa8,0xee,0xab,0xfd,0xad,0xeb,0xbf
	};
	static const unsigned char oddtab[] =
	{
		0x50,0x11,0x12,0x52,0x40,0x55,0x56,0x57,0x58,0x19,0x1a,0x5a,0x48,0x5d,0x5e,0x5f,
		0x51,0x14,0x03,0x46,0x45,0x04,0x42,0x06,0x59,0x1c,0x0b,0x4e,0x4d,0x0c,0x4a,0x0e,
		0x70,0x31,0x32,0x72,0x60,0x75,0x76,0x77,0x78,0x39,0x3a,0x7a,0x68,0x7d,0x7e,0x7f,
		0x71,0x34,0x23,0x66,0x65,0x24,0x62,0x26,0x79,0x3c,0x2b,0x6e,0x6d,0x2c,0x6a,0x2e,
		0x54,0x15,0x16,0x13,0x10,0x05,0x02,0x43,0x5c,0x1d,0x1e,0x1b,0x18,0x0d,0x0a,0x4b,
		0x44,0x01,0x47,0x17,0x00,0x41,0x53,0x07,0x4c,0x09,0x4f,0x1f,0x08,0x49,0x5b,0x0f,
		0x74,0x35,0x36,0x33,0x30,0x25,0x22,0x63,0x7c,0x3d,0x3e,0x3b,0x38,0x2d,0x2a,0x6b,
		0x64,0x21,0x67,0x37,0x20,0x61,0x73,0x27,0x6c,0x29,0x6f,0x3f,0x28,0x69,0x7b,0x2f,
		0x81,0x85,0xd7,0xd2,0xc1,0xc5,0x97,0x92,0x89,0x8d,0xdf,0xda,0xc9,0xcd,0x9f,0x9a,
		0xd4,0xd0,0x83,0x86,0xd5,0x90,0xc3,0xc6,0xdc,0xd8,0x8b,0x8e,0xdd,0x98,0xcb,0xce,
		0xa1,0xa5,0xf7,0xf2,0xe1,0xe5,0xb7,0xb2,0xa9,0xad,0xff,0xfa,0xe9,0xed,0xbf,0xba,
		0xf4,0xf0,0xa3,0xa6,0xf5,0xb0,0xe3,0xe6,0xfc,0xf8,0xab,0xae,0xfd,0xb8,0xeb,0xee,
		0x91,0x95,0xc7,0xc2,0xd1,0x94,0x87,0x82,0x99,0x9d,0xcf,0xca,0xd9,0x9c,0x8f,0x8a,
		0xc4,0xc0,0x93,0x96,0x84,0x80,0xd3,0xd6,0xcc,0xc8,0x9b,0x9e,0x8c,0x88,0xdb,0xde,
		0xb1,0xb5,0xe7,0xe2,0xf1,0xb4,0xa7,0xa2,0xb9,0xbd,0xef,0xea,0xf9,0xbc,0xaf,0xaa,
		0xe4,0xe0,0xb3,0xb6,0xa4,0xa0,0xf3,0xf6,0xec,0xe8,0xbb,0xbe,0xac,0xa8,0xfb,0xfe
	};


	if (A & 1) return oddtab[RAM[A]];
	else return evetab[RAM[A]];
}



static int hiload(const char *name)
{
	/* check if the hi score table has already been initialized */
	if (memcmp(&RAM[0x8083],"\x02\x00\x00",3) == 0 &&
			memcmp(&RAM[0x808f],"\x02\x00\x00",3) == 0)
	{
		FILE *f;


		if ((f = fopen(name,"rb")) != 0)
		{
			fread(&RAM[0x8083],1,17*5,f);
			fclose(f);
		}

		return 1;
	}
	else return 0;	/* we can't load the hi scores yet */
}



static void hisave(const char *name)
{
	FILE *f;


	if ((f = fopen(name,"wb")) != 0)
	{
		fwrite(&RAM[0x8083],1,17*5,f);
		fclose(f);
	}
}



struct GameDriver cclimber_driver =
{
	"Crazy Climber (US version)",
	"cclimber",
	"LIONEL THEUNISSEN\nNICOLA SALMORIA",
	&machine_driver,

	cclimber_rom,
	0, cclimber_decode,
	0,

	input_ports, dsw, keys,

	color_prom, 0, 0,
	8*13, 8*16,

	hiload, hisave
};

struct GameDriver ccjap_driver =
{
	"Crazy Climber (Japanese version)",
	"ccjap",
	"LIONEL THEUNISSEN\nNICOLA SALMORIA",
	&machine_driver,

	ccjap_rom,
	0, ccjap_decode,
	0,

	input_ports, dsw, keys,

	color_prom, 0, 0,
	8*13, 8*16,

	hiload, hisave
};

struct GameDriver ccboot_driver =
{
	"Crazy Climber (bootleg)",
	"ccboot",
	"LIONEL THEUNISSEN\nNICOLA SALMORIA",
	&machine_driver,

	ccboot_rom,
	0, ccjap_decode,
	0,

	input_ports, dsw, keys,

	color_prom, 0, 0,
	8*13, 8*16,

	hiload, hisave
};
