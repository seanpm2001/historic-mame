/***************************************************************************

Rock'n'Rage(GX620) (c) 1986 Konami

Preliminary driver by:
	Manuel Abadia <manu@teleline.es>

TODO: Use the color lookup tables

***************************************************************************/

#include "driver.h"
#include "cpu/m6809/m6809.h"
#include "vidhrdw/generic.h"
#include "vidhrdw/konamiic.h"

extern int rockrage_irq_enable;

/* from vidhrdw */
int rockrage_vh_start(void);
void rockrage_vh_stop(void);
void rockrage_vh_screenrefresh(struct osd_bitmap *bitmap,int full_refresh);
void rockrage_vreg_w(int offset, int data);

static int rockrage_interrupt( void )
{
	if (K007342_is_INT_enabled)
        return M6309_INT_IRQ;
    else
		return ignore_interrupt();
}

static void rockrage_bankswitch_w(int offset, int data)
{
	unsigned char *RAM = Machine->memory_region[Machine->drv->cpu[0].memory_region];
	int bankaddress;

	/* bits 4-6 = bank number */
	bankaddress = 0x10000 + ((data & 0x70) >> 4) * 0x2000;
	cpu_setbank(1,&RAM[bankaddress]);

	/* bits 0 & 1 = coin counters */
	coin_counter_w(0,data & 0x01);
	coin_counter_w(1,data & 0x02);

	/* other bits unknown */
}

static void rockrage_sh_irqtrigger_w(int offset, int data)
{
	soundlatch_w(offset, data);
	cpu_cause_interrupt(1,M6809_INT_IRQ);
}

static int rockrage_VLM5030_busy_r( int offs ) {
	return ( VLM5030_BSY() ? 1 : 0 );
}

static void rockrage_speech_w( int offs, int data ) {
	VLM5030_RST( ( data >> 2 ) & 0x01 );
	VLM5030_ST(  ( data >> 0 ) & 0x01 );
	VLM5030_VCU( ( data >> 1 ) & 0x01 );
}

static struct MemoryReadAddress rockrage_readmem[] =
{
	{ 0x0000, 0x1fff, K007342_r },			/* Color RAM + Video RAM */
	{ 0x2000, 0x21ff, K007420_r },			/* Sprite RAM */
	{ 0x2200, 0x23ff, MRA_RAM },			/* ??? */
	{ 0x2400, 0x247f, MRA_RAM },			/* Palette */
	{ 0x2e01, 0x2e01, input_port_3_r },		/* 1P controls */
	{ 0x2e02, 0x2e02, input_port_4_r },		/* 2P controls */
	{ 0x2e03, 0x2e03, input_port_1_r },		/* DISPW #2 */
	{ 0x2e40, 0x2e40, input_port_0_r },		/* DIPSW #1 */
	{ 0x2e00, 0x2e00, input_port_2_r },		/* coinsw, testsw, startsw */
	{ 0x4000, 0x5fff, MRA_RAM },			/* RAM */
	{ 0x6000, 0x7fff, MRA_BANK1 },			/* banked ROM */
	{ 0x8000, 0xffff, MRA_ROM },			/* ROM */
	{ -1 }	/* end of table */
};

static struct MemoryWriteAddress rockrage_writemem[] =
{
	{ 0x0000, 0x1fff, K007342_w },				/* Color RAM + Video RAM */
	{ 0x2000, 0x21ff, K007420_w },				/* Sprite RAM */
	{ 0x2200, 0x23ff, MWA_RAM },				/* ??? */
	{ 0x2400, 0x247f, paletteram_xBBBBBGGGGGRRRRR_w, &paletteram },/* palette */
	{ 0x2600, 0x2607, K007342_vreg_w },			/* Video Registers */
	{ 0x2e80, 0x2e80, rockrage_sh_irqtrigger_w },/* cause interrupt on audio CPU */
	{ 0x2ec0, 0x2ec0, watchdog_reset_w },		/* watchdog reset */
	{ 0x2f00, 0x2f00, rockrage_vreg_w },		/* ??? */
	{ 0x2f40, 0x2f40, rockrage_bankswitch_w },	/* bankswitch control */
	{ 0x4000, 0x5fff, MWA_RAM },				/* RAM */
	{ 0x6000, 0x7fff, MWA_RAM },				/* banked ROM */
	{ 0x8000, 0xffff, MWA_ROM },				/* ROM */
	{ -1 }	/* end of table */
};

static struct MemoryReadAddress rockrage_readmem_sound[] =
{
	{ 0x3000, 0x3000, rockrage_VLM5030_busy_r },/* VLM5030 */
	{ 0x5000, 0x5000, soundlatch_r },			/* soundlatch_r */
	{ 0x6001, 0x6001, YM2151_status_port_0_r },	/* YM 2151 */
	{ 0x7000, 0x77ff, MRA_RAM },				/* RAM */
	{ 0x8000, 0xffff, MRA_ROM },				/* ROM */
	{ -1 }	/* end of table */
};

static struct MemoryWriteAddress rockrage_writemem_sound[] =
{
	{ 0x2000, 0x2000, VLM5030_data_w }, 			/* VLM5030 */
	{ 0x4000, 0x4000, rockrage_speech_w },			/* VLM5030 */
	{ 0x6000, 0x6000, YM2151_register_port_0_w },	/* YM 2151 */
	{ 0x6001, 0x6001, YM2151_data_port_0_w },		/* YM 2151 */
	{ 0x7000, 0x77ff, MWA_RAM },					/* RAM */
	{ 0x8000, 0xffff, MWA_ROM },					/* ROM */
	{ -1 }	/* end of table */
};

/***************************************************************************

	Input Ports

***************************************************************************/

INPUT_PORTS_START( input_ports )
	PORT_START	/* DSW #1 */
	PORT_DIPNAME( 0x0f, 0x0f, DEF_STR( Coin_A ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(    0x05, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(    0x08, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x04, DEF_STR( 3C_2C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 4C_3C ) )
	PORT_DIPSETTING(    0x0f, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x03, DEF_STR( 3C_4C ) )
	PORT_DIPSETTING(    0x07, DEF_STR( 2C_3C ) )
	PORT_DIPSETTING(    0x0e, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x06, DEF_STR( 2C_5C ) )
	PORT_DIPSETTING(    0x0d, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(    0x0c, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(    0x0b, DEF_STR( 1C_5C ) )
	PORT_DIPSETTING(    0x0a, DEF_STR( 1C_6C ) )
	PORT_DIPSETTING(    0x09, DEF_STR( 1C_7C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0xf0, 0xf0, DEF_STR( Coin_B ) )
	PORT_DIPSETTING(    0x20, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(    0x50, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(    0x80, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x40, DEF_STR( 3C_2C ) )
	PORT_DIPSETTING(    0x10, DEF_STR( 4C_3C ) )
	PORT_DIPSETTING(    0xf0, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x30, DEF_STR( 3C_4C ) )
	PORT_DIPSETTING(    0x70, DEF_STR( 2C_3C ) )
	PORT_DIPSETTING(    0xe0, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x60, DEF_STR( 2C_5C ) )
	PORT_DIPSETTING(    0xd0, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(    0xc0, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(    0xb0, DEF_STR( 1C_5C ) )
	PORT_DIPSETTING(    0xa0, DEF_STR( 1C_6C ) )
	PORT_DIPSETTING(    0x90, DEF_STR( 1C_7C ) )
//	PORT_DIPSETTING(    0x00, "Invalid" )

	PORT_START	/* DSW #2 */
	PORT_DIPNAME( 0x03, 0x01, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x03, "1" )
	PORT_DIPSETTING(    0x02, "2" )
	PORT_DIPSETTING(    0x01, "3" )
	PORT_DIPSETTING(    0x00, "5" )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Bonus_Life ) )
	PORT_DIPSETTING(    0x08, "30k and every 70k" )
	PORT_DIPSETTING(    0x00, "40k and every 80k" )
	PORT_DIPNAME( 0x10, 0x10, "Freeze Screen" )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x00, DEF_STR( Demo_Sounds ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_START	/* COINSW */
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_COIN2 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_COIN3 )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Flip_Screen ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x00, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x40, DEF_STR( On ) )
	PORT_SERVICE( 0x80, IP_ACTIVE_LOW )

	PORT_START	/* PLAYER 1 INPUTS */
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT  | IPF_8WAY | IPF_PLAYER1 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT | IPF_8WAY | IPF_PLAYER1 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP    | IPF_8WAY | IPF_PLAYER1 )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN  | IPF_8WAY | IPF_PLAYER1 )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 | IPF_PLAYER1 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_BUTTON2 | IPF_PLAYER1 )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNKNOWN )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNKNOWN )

	PORT_START	/* PLAYER 2 INPUTS */
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT  | IPF_8WAY | IPF_PLAYER2 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT | IPF_8WAY | IPF_PLAYER2 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP    | IPF_8WAY | IPF_PLAYER2 )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN  | IPF_8WAY | IPF_PLAYER2 )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 | IPF_PLAYER2 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_BUTTON2 | IPF_PLAYER2 )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNKNOWN )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNKNOWN )

INPUT_PORTS_END

static struct GfxLayout charlayout =
{
	8,8,			/* 8*8 characters */
	0x20000/16,		/* 8192 characters */
	4,				/* 4 bpp */
	{ 0, 1, 2, 3 },	/* the four bitplanes are packed in one nibble */
	{ 0*4, 1*4, 0x20000*8+0*4, 0x20000*8+1*4, 2*4, 3*4, 0x20000*8+2*4, 0x20000*8+3*4 },
	{ 0*16, 1*16, 2*16, 3*16, 4*16, 5*16, 6*16, 7*16 },
	16*8		/* every char takes 16 consecutive bytes */
};

static struct GfxLayout spritelayout =
{
	16,16,			/* 16*16 sprites */
	0x40000/128,	/* 2048 sprites */
	4,				/* 4 bpp */
	{ 0, 1, 2, 3 },	/* the four bitplanes are packed in one nibble */
	{ 0*4, 1*4, 2*4, 3*4, 4*4, 5*4, 6*4, 7*4,
		32*8+0*4, 32*8+1*4, 32*8+2*4, 32*8+3*4, 32*8+4*4, 32*8+5*4, 32*8+6*4, 32*8+7*4 },
	{ 0*32, 1*32, 2*32, 3*32, 4*32, 5*32, 6*32, 7*32,
		64*8+0*32, 64*8+1*32, 64*8+2*32, 64*8+3*32, 64*8+4*32, 64*8+5*32, 64*8+6*32, 64*8+7*32 },
	128*8			/* every sprite takes 128 consecutive bytes */
};

static struct GfxDecodeInfo gfxdecodeinfo[] =
{
	{ 1, 0x000000, &charlayout,		0,			1 },	/* colors 00..31 */
	{ 1, 0x040000, &spritelayout,	2*16,		1 },	/* colros 32..63 */
	{ -1 } /* end of array */
};

/***************************************************************************

	Machine Driver

***************************************************************************/

static struct YM2151interface ym2151_interface =
{
	1,			/* 1 chip */
	3579545,	/* 3.579545 MHz */
	{ YM3012_VOL(60,MIXER_PAN_LEFT,60,MIXER_PAN_RIGHT) },
	{ 0 },
	{ 0 }
};

static struct VLM5030interface vlm5030_interface =
{
	3579545,	/* 3.579545 MHz */
	60,			/* volume */
	3,			/* memory region of speech rom */
	0,
	0
};

static struct MachineDriver machine_driver =
{
	/* basic machine hardware */
	{
		{
			CPU_M6309,
			3000000,		/* 24MHz/8 (?) */
			0,
			rockrage_readmem,rockrage_writemem,0,0,
            rockrage_interrupt,1
        },
		{
			CPU_M6809 | CPU_AUDIO_CPU,
			3000000,		/* 24MHz/8 (?) */
			2,
			rockrage_readmem_sound, rockrage_writemem_sound,0,0,
			ignore_interrupt,0	/* interrupts are triggered by the main CPU */
		}
	},
	60, DEFAULT_60HZ_VBLANK_DURATION,	/* frames per second, vblank duration */
	1,	/* 1 CPU slice per frame - interleaving is forced when a sound command is written */
	0,

	/* video hardware */
	32*8, 32*8, { 0*8, 32*8-1, 2*8, 30*8-1 },
	gfxdecodeinfo,
	64, 64,
	0,

	VIDEO_TYPE_RASTER | VIDEO_MODIFIES_PALETTE,
	0,
	rockrage_vh_start,
	rockrage_vh_stop,
	rockrage_vh_screenrefresh,

	/* sound hardware */
	0,0,0,0,
	{
		{
			SOUND_YM2151,
			&ym2151_interface
		},
		{
			SOUND_VLM5030,
			&vlm5030_interface
		}
	}
};


/***************************************************************************

  Game ROMs

***************************************************************************/

ROM_START( rockrage_rom )
	ROM_REGION( 0x20000 ) /* code + banked roms */
	ROM_LOAD( "rr-q01.rom", 0x08000, 0x08000, 0x0ddb5ef5 )	/* fixed ROM */
	ROM_LOAD( "rr-q02.rom", 0x10000, 0x10000, 0xb4f6e346 )	/* banked ROM */

	ROM_REGION_DISPOSE( 0x080000 ) /* graphics (disposed after conversion) */
	ROM_LOAD( "620k06.15g",	0x000000, 0x20000, BADCRC( 0xc0e2b35c ) )	/* tiles */
	ROM_LOAD( "620k05.16g",	0x020000, 0x20000, BADCRC( 0xca9d9346 ) )	/* tiles */
	ROM_LOAD( "rr-k11.rom",	0x040000, 0x20000, 0x70449239 )	/* sprites */
	ROM_LOAD( "rr-l10.rom",	0x060000, 0x20000, 0x06d108e0 )	/* sprites */

	ROM_REGION( 0x10000 ) /* 64k for the sound CPU */
	ROM_LOAD( "620k03.11c", 0x08000, 0x08000, 0x9fbefe82 )

	ROM_REGION( 0x08000 ) /* VLM3050 data */
	ROM_LOAD( "620k04.6e", 0x00000, 0x08000, 0x8be969f3 )

	ROM_REGION( 0x0300 ) /* lookup tables */
	ROM_LOAD( "620k08.12g", 0x00000, 0x00100, 0xb499800c )
	ROM_LOAD( "620k09.11g", 0x00100, 0x00100, 0x9f0e0608 )
	ROM_LOAD( "620k07.13g", 0x00200, 0x00100, 0xb6135ee0 )
ROM_END

ROM_START( rockragj_rom )
	ROM_REGION( 0x20000 ) /* code + banked roms */
	ROM_LOAD( "620k01.16c", 0x08000, 0x08000, 0x4f5171f7 )	/* fixed ROM */
	ROM_LOAD( "620k02.15c", 0x10000, 0x10000, 0x04c4d8f7 )	/* banked ROM */

	ROM_REGION_DISPOSE( 0x080000 ) /* graphics (disposed after conversion) */
	ROM_LOAD( "620k06.15g",	0x000000, 0x20000, 0xc0e2b35c )	/* tiles */
	ROM_LOAD( "620k05.16g",	0x020000, 0x20000, 0xca9d9346 )	/* tiles */
	ROM_LOAD( "620k11.7g",	0x040000, 0x20000, 0x7430f6e9 )	/* sprites */
	ROM_LOAD( "620k10.8g",	0x060000, 0x20000, 0x0d1a95ab )	/* sprites */

	ROM_REGION( 0x10000 ) /* 64k for the sound CPU */
	ROM_LOAD( "620k03.11c", 0x08000, 0x08000, 0x9fbefe82 )

	ROM_REGION( 0x08000 ) /* VLM3050 data */
	ROM_LOAD( "620k04.6e", 0x00000, 0x08000, 0x8be969f3 )

	ROM_REGION( 0x0300 ) /* lookup tables */
	ROM_LOAD( "620k08.12g", 0x00000, 0x00100, 0xb499800c )
	ROM_LOAD( "620k09.11g", 0x00100, 0x00100, 0x9f0e0608 )
	ROM_LOAD( "620k07.13g", 0x00200, 0x00100, 0xb6135ee0 )
ROM_END

/***************************************************************************

  Game driver(s)

***************************************************************************/

struct GameDriver rockrage_driver =
{
	__FILE__,
	0,
	"rockrage",
	"Rock 'n Rage (World?)",
	"1986",
	"Konami",
	"Manuel Abadia",
	GAME_IMPERFECT_COLORS,
	&machine_driver,
	0,

	rockrage_rom,
	0, 0,
	0,
	0,	/* sound_prom */

	input_ports,

	PROM_MEMORY_REGION(4), 0, 0,
    ORIENTATION_DEFAULT,
	0, 0
};

struct GameDriver rockragj_driver =
{
	__FILE__,
	&rockrage_driver,
	"rockragj",
	"Koi no Hotrock (Japan)",
	"1986",
	"Konami",
	"Manuel Abadia",
	GAME_IMPERFECT_COLORS,
	&machine_driver,
	0,

	rockragj_rom,
	0, 0,
	0,
	0,	/* sound_prom */

	input_ports,

	PROM_MEMORY_REGION(4), 0, 0,
    ORIENTATION_DEFAULT,
	0, 0
};
