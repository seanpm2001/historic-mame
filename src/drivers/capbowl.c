/***************************************************************************

  Coors Light Bowling/Bowl-O-Rama memory map (preliminary)

  CPU Board:

  0000-3fff     3 Graphics ROMS mapped in using 0x4800 (Coors Light Bowling only)
  0000-001f		Turbo board area (Bowl-O-Rama only) See Below.
  4000          Scan line read or modified
  4800          Graphics ROM select
  5000-57ff     Static RAM (Saves machine state after shut off)
                Enter setup menu by holding down the F2 key on the
                high score screen
  5800-5846		TMS34061 control area
  5900-591f		TMS34061 XY pixel area (only used on Bowl-O-Rama)
  5b00-5bff     Graphics RAM for one scanline (0x4000)

                First 0x20 bytes provide a 16 color palette for this
                scan line. 2 bytes per color: 0000RRRR GGGGBBBB.

                Remaining 0xe0 bytes contain 2 pixels each for a total of
                448 pixels, but only 360 seem to be displayed.
                (Each scanline appears vertically because the monitor is rotated)

  6000          Sound command
  6800			???
  7000          Input port 1    Bit 0-3 Trackball Vertical,
							  	Bit 4   Player 2 Hook Left
								Bit 5   Player 2 Hook Right
								Bit 6   Upright/Cocktail DIP Switch
                                Bit 7   Left Coin
  7800          Input port 2    Bit 0-3 Trackball Horizontal
                                Bit 4   Hook Left
                                Bit 5   Hook Right
                                Bit 6   Start
                                Bit 7   Right Coin
  8000-ffff		ROM


  Sound Board:

  0000-07ff		RAM
  1000-1001		YM2203
			  	Port A D7 Read  is ticket dispenser status
				Port B D7 Write is ticket dispenser enable
				Port B D6 is pulsated continously. Don't know what it does
  2000			???
  6000			DAC write
  7000			Sound command read (0x34 is used to dispense a ticket)
  8000-ffff		ROM


  Turbo Board Layout:

  Bowl-O-Rama	Copyright 1991 P&P Marketing
				Marquee says "EXIT Entertainment"

				This portion: Mike Appolo with the help of Andrew Pines.
				Andrew was one of the game designers for Capcom Bowling,
				Coors Light Bowling, Strata Bowling, and Bowl-O-Rama.

				This game was an upgrade for Capcom Bowling and included a
				"Turbo PCB" that had a GAL address decoder / data mask

  Memory Map for turbo board (where GR0 is on Capcom Bowling PCBs:

  0000   		Read Mask
  0001-0003		Unused
  0004  		Read Data
  0005-0007		Unused
  0008  		GR Address High Byte (GR17-16)
  0009-0016		Unused
  0017			GR Address Middle Byte (GR15-0 written as a word to 0017-0018)
  0018  		GR address Low byte
  0019-001f		Unused

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"
#include "vidhrdw/tms34061.h"
#include "machine/ticket.h"
#include "M6809/M6809.h"

void capbowl_init_machine(void);

void capbowl_vh_screenrefresh(struct osd_bitmap *bitmap);

int  capbowl_vh_start(void);
void capbowl_vh_stop(void);

extern unsigned char *capbowl_scanline;

void capbowl_videoram_w(int offset,int data);
int  capbowl_videoram_r(int offset);

void capbowl_rom_select_w(int offset,int data);

int  capbowl_pagedrom_r(int offset);

void bowlrama_turbo_w(int offset, int data);
int  bowlrama_turbo_r(int offset);



static void capbowl_sndcmd_w(int offset,int data)
{
	cpu_cause_interrupt(1, M6809_INT_IRQ);

	soundlatch_w(offset, data);
}


/* handler called by the 2203 emulator when the internal timers cause an IRQ */
/* should be enabled with 	OPNSetIrqHandler(0,irqhandler); */
static void firqhandler(void)
{
	cpu_cause_interrupt(1, M6809_INT_FIRQ);
}


static struct MemoryReadAddress capbowl_readmem[] =
{
	{ 0x0000, 0x3fff, MRA_BANK1 },
	{ 0x5000, 0x57ff, MRA_RAM },
	{ 0x5800, 0x5846, tms34061_r },
	{ 0x5b00, 0x5bff, capbowl_videoram_r },
	{ 0x5c00, 0x5c00, MRA_NOP }, /* Off by 1 */
	{ 0x7000, 0x7000, input_port_0_r },
	{ 0x7800, 0x7800, input_port_1_r },
	{ 0x8000, 0xffff, MRA_ROM },
	{ -1 }  /* end of table */
};

static struct MemoryReadAddress bowlrama_readmem[] =
{
	{ 0x0000, 0x001f, bowlrama_turbo_r },
	{ 0x5000, 0x57ff, MRA_RAM },
	{ 0x5800, 0x5846, tms34061_r },
	{ 0x5900, 0x591f, tms34061_xypixel_r },
	{ 0x5b00, 0x5bff, capbowl_videoram_r },
	{ 0x5c00, 0x5c00, MRA_NOP }, /* Off by 1 bug */
	{ 0x7000, 0x7000, input_port_0_r },
	{ 0x7800, 0x7800, input_port_1_r },
	{ 0x8000, 0xffff, MRA_ROM },
	{ -1 }  /* end of table */
};

static struct MemoryWriteAddress writemem[] =
{
	{ 0x0000, 0x001f, bowlrama_turbo_w },	/* Bowl-O-Rama only */
	{ 0x4000, 0x4000, MWA_RAM, &capbowl_scanline },
	{ 0x4800, 0x4800, capbowl_rom_select_w },
	{ 0x5000, 0x57ff, MWA_RAM },
	{ 0x5800, 0x5846, tms34061_w },
	{ 0x5900, 0x591f, tms34061_xypixel_w }, /* Bowl-O-Rama only */
	{ 0x5b00, 0x5bff, capbowl_videoram_w },
	{ 0x5c00, 0x5c00, MWA_NOP }, /* Off by 1 */
	{ 0x6000, 0x6000, capbowl_sndcmd_w },
	{ 0x6800, 0x6800, MWA_NOP },	/* trackball reset? */
	{ -1 }  /* end of table */
};


static struct MemoryReadAddress sound_readmem[] =
{
	{ 0x0000, 0x07ff, MRA_RAM },
	{ 0x1000, 0x1000, YM2203_status_port_0_r },
	{ 0x1001, 0x1001, YM2203_read_port_0_r },
	{ 0x7000, 0x7000, soundlatch_r },
	{ 0x8000, 0xffff, MRA_ROM },
	{ -1 }  /* end of table */
};

static struct MemoryWriteAddress sound_writemem[] =
{
	{ 0x0000, 0x07ff, MWA_RAM},
	{ 0x1000, 0x1000, YM2203_control_port_0_w },
	{ 0x1001, 0x1001, YM2203_write_port_0_w },
	{ 0x2000, 0x2000, MWA_NOP },  /* ????? */
	{ 0x6000, 0x6000, DAC_data_w },
	{ -1 }  /* end of table */
};



/***************************************************************************

  Coors Light Bowling uses NMI to trigger the self test. We use a fake input
  port to tie that event to a keypress.

***************************************************************************/
static int capbowl_interrupt(void)
{
	if (readinputport(2) & 1)	/* get status of the F2 key */
		return nmi_interrupt();	/* trigger self test */

	return ignore_interrupt();
}

static int capbowl_raster_interrupt(void)
{
	return M6809_INT_FIRQ;
}

INPUT_PORTS_START( input_ports )
	PORT_START	/* IN0 */
	PORT_ANALOG ( 0x0f, 0x00, IPT_TRACKBALL_Y | IPF_REVERSE | IPF_CENTER, 100, 7, 0, 0 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 | IPF_COCKTAIL )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 | IPF_COCKTAIL )
	PORT_DIPNAME( 0x40, 0x40, "Cabinet", IP_KEY_NONE ) /* This version of Bowl-O-Rama */
	PORT_DIPSETTING(    0x40, "Upright" )			   /* is Upright only */
	PORT_DIPSETTING(    0x00, "Cocktail" )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN1 )

	PORT_START	/* IN1 */
	PORT_ANALOG ( 0x0f, 0x00, IPT_TRACKBALL_X | IPF_CENTER, 100, 7, 0, 0 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN2 )

	PORT_START	/* FAKE */
	/* This fake input port is used to get the status of the F2 key, */
	/* and activate the test mode, which is triggered by a NMI */
	PORT_BITX(0x01, IP_ACTIVE_HIGH, IPT_SERVICE, "Service Mode", OSD_KEY_F2, IP_JOY_NONE, 0 )
INPUT_PORTS_END



static struct YM2203interface ym2203_interface =
{
	1,			/* 1 chip */
	2000000,	/* 2 MHz ??? */
	{ YM2203_VOL(255,255) },
	{ ticket_dispenser_r },
	{ 0 },
	{ 0 },
	{ ticket_dispenser_w },
	{ firqhandler }
};

static struct DACinterface dac_interface =
{
	1,
	441000,
	{ 64, 64 },
	{  1,  1 }
};


#define MACHINEDRIVER(NAME, VISIBLE_Y)						\
															\
static struct MachineDriver NAME##_machine_driver =			\
{															\
	/* basic machine hardware */   							\
	{														\
		{													\
			CPU_M6809,										\
			2000000,        /* 2 Mhz */						\
			0,												\
			NAME##_readmem,writemem,0,0,					\
			capbowl_interrupt, 1,       /* To check Service mode status */ \
			capbowl_raster_interrupt, 0	/* Generated by the TMS34061 */ \
		},													\
		{													\
			CPU_M6809 | CPU_AUDIO_CPU,						\
			2000000,        /* 2 Mhz */						\
			2,												\
			sound_readmem,sound_writemem,0,0,				\
			ignore_interrupt,1	/* interrupts are generated by the sound hardware */ \
		}													\
	},														\
	60, DEFAULT_60HZ_VBLANK_DURATION,	/* frames per second, vblank duration */ \
	1,	/* 1 CPU slice per frame - interleaving is forced when a sound command is written */ \
	capbowl_init_machine,									\
															\
	/* video hardware */									\
	360, 256, { 0, 359, 0, VISIBLE_Y },						\
	0,														\
	16*256,16*256,											\
	0,														\
															\
	VIDEO_TYPE_RASTER | VIDEO_MODIFIES_PALETTE,				\
	0,														\
	capbowl_vh_start,										\
	capbowl_vh_stop,										\
	capbowl_vh_screenrefresh,								\
															\
	/* sound hardware */									\
	0,0,0,0,												\
	{														\
		{													\
			SOUND_YM2203,									\
			&ym2203_interface								\
		},													\
		{													\
			SOUND_DAC,										\
			&dac_interface									\
		}													\
	}														\
};


MACHINEDRIVER(capbowl,  244)

MACHINEDRIVER(bowlrama, 239)


/***************************************************************************

  Game driver(s)

***************************************************************************/

ROM_START( capbowl_rom )
	ROM_REGION(0x28000)   /* 160k for code and graphics */
	ROM_LOAD( "u6",  0x8000 , 0x8000, 0xb70297ae )
	ROM_LOAD( "gr0", 0x10000, 0x8000, 0xfb7d35bd )
	ROM_LOAD( "gr1", 0x18000, 0x8000, 0xe28dc4ef )
	ROM_LOAD( "gr2", 0x20000, 0x8000, 0x325fce25 )

	ROM_REGION(0x1000)      /* temporary space for graphics (disposed after conversion) */
	/* empty memory region - not used by the game, but needed because the main */
	/* core currently always frees region #1 after initialization. */

	ROM_REGION(0x10000)   /* 64k for sound */
	ROM_LOAD( "sound", 0x8000, 0x8000, 0x1ec37619 )
ROM_END

ROM_START( clbowl_rom )
	ROM_REGION(0x28000)   /* 160k for code and graphics */
	ROM_LOAD( "u6",  0x8000 , 0x8000, 0x99fede6e )
	ROM_LOAD( "gr0", 0x10000, 0x8000, 0x64039867 )
	ROM_LOAD( "gr1", 0x18000, 0x8000, 0x3a758375 )
	ROM_LOAD( "gr2", 0x20000, 0x8000, 0xb63eb4f2 )

	ROM_REGION(0x1000)      /* temporary space for graphics (disposed after conversion) */
	/* empty memory region - not used by the game, but needed because the main */
	/* core currently always frees region #1 after initialization. */

	ROM_REGION(0x10000)   /* 64k for sound */
	ROM_LOAD( "sound", 0x8000, 0x8000, 0xe27c494a )
ROM_END

ROM_START( bowlrama_rom )
	ROM_REGION(0x10000)      /* 64k for code */
	ROM_LOAD( "u6",  0x08000, 0x08000, 0x5ba3c13d )

	ROM_REGION(0x1000)      /* temporary space for graphics (disposed after conversion) */
	/* empty memory region - not used by the game, but needed because the main */
	/* core currently always frees region #1 after initialization. */

	ROM_REGION(0x10000)     /* 64k for sound */
	ROM_LOAD( "u30", 0x8000, 0x8000, 0xa8bbc179 )

	ROM_REGION(0x40000)     /* 256K for Graphics */
	ROM_LOAD( "ux7", 0x00000, 0x40000, 0x9e8ae380 )
ROM_END


static int hiload(void)
{
	void *f;
	/* get RAM pointer (this game is multiCPU, we can't assume the global */
	/* RAM pointer is pointing to the right place) */
	unsigned char *RAM = Machine->memory_region[0];


	/* Try loading static RAM */
	if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,0)) != 0)
	{
		osd_fread(f,&RAM[0x5000],0x800);
		osd_fclose(f);
	}
	else
	{
		/* Invalidate the static RAM to force reset to factory settings */
		memset(&RAM[0x5000],0x01,0x800);
	}

	RAM[0x5725] = 0x01; // Otherwise Bowl-O-Rama won't start
						// This location doesn't seem to be used by
						// Capcom Bowling, so this should be Ok.
	return 1;
}

static void hisave(void)
{
	void *f;
	/* get RAM pointer (this game is multiCPU, we can't assume the global */
	/* RAM pointer is pointing to the right place) */
	unsigned char *RAM = Machine->memory_region[0];


	if ((f = osd_fopen(Machine->gamedrv->name,0,OSD_FILETYPE_HIGHSCORE,1)) != 0)
	{
		osd_fwrite(f,&RAM[0x5000],0x0800);
		osd_fclose(f);
	}
}


#define GAMEDRIVER(NAME, BASE, DESC, CREDITS)	\
												\
struct GameDriver NAME##_driver =				\
{												\
	DESC,  										\
	#NAME,										\
	CREDITS,									\
	&BASE##_machine_driver,						\
												\
	NAME##_rom,									\
	0, 0,										\
	0,											\
	0,											\
												\
	input_ports,								\
												\
	0, 0, 0,									\
												\
	ORIENTATION_ROTATE_270,						\
												\
	hiload, hisave								\
};


#define BASE_CREDITS  "Zsolt Vasvari\nMirko Buffoni\nNicola Salmoria"

GAMEDRIVER(capbowl,  capbowl,  "Capcom Bowling",      BASE_CREDITS)

GAMEDRIVER(clbowl,   capbowl,  "Coors Light Bowling", BASE_CREDITS)

GAMEDRIVER(bowlrama, bowlrama, "Bowl-O-Rama",         "Michael Appolo\n"BASE_CREDITS)
