/***************************************************************************

Donkey Kong memory map (preliminary)

0000-3fff ROM (Donkey Kong Jr.: 0000-5fff)
6000-6fff RAM
7000-73ff ?
7400-77ff Video RAM


memory mapped ports:

read:
7c00      IN0
7c80      IN1
7d00      IN2
7d80      DSW1

*
 * IN0 (bits NOT inverted)
 * bit 7 : ?
 * bit 6 : reset (when player 1 active)
 * bit 5 : ?
 * bit 4 : JUMP player 1
 * bit 3 : DOWN player 1
 * bit 2 : UP player 1
 * bit 1 : LEFT player 1
 * bit 0 : RIGHT player 1
 *
*
 * IN1 (bits NOT inverted)
 * bit 7 : ?
 * bit 6 : reset (when player 2 active)
 * bit 5 : ?
 * bit 4 : JUMP player 2
 * bit 3 : DOWN player 2
 * bit 2 : UP player 2
 * bit 1 : LEFT player 2
 * bit 0 : RIGHT player 2
 *
*
 * IN2 (bits NOT inverted)
 * bit 7 : COIN
 * bit 6 : ?
 * bit 5 : ?
 * bit 4 : ?
 * bit 3 : START 2
 * bit 2 : START 1
 * bit 1 : ?
 * bit 0 : ? if this is 1, the code jumps to $4000, outside the rom space
 *
*
 * DSW1 (bits NOT inverted)
 * bit 7 : COCKTAIL or UPRIGHT cabinet (1 = UPRIGHT)
 * bit 6 : \ 000 = 1 coin 1 play   001 = 2 coins 1 play  010 = 1 coin 2 plays
 * bit 5 : | 011 = 3 coins 1 play  100 = 1 coin 3 plays  101 = 4 coins 1 play
 * bit 4 : / 110 = 1 coin 4 plays  111 = 5 coins 1 play
 * bit 3 : \bonus at
 * bit 2 : / 00 = 7000  01 = 10000  10 = 15000  11 = 20000
 * bit 1 : \ 00 = 3 lives  01 = 4 lives
 * bit 0 : / 10 = 5 lives  11 = 6 lives
 *

write:
6900-6a7f sprites
7800-7803 ?
7808      ?
7c00      Background sound/music select:
          00 - nothing
		  01 - Intro tune
		  02 - How High? (intermisson) tune
		  03 - Out of time
		  04 - Hammer
		  05 - Rivet level 2 completed (end tune)
		  06 - Hammer hit
		  07 - Standard level end
		  08 - Background 1	(first screen)
		  09 - ???
		  0A - Background 3	(springs)
		  0B - Background 2 (rivet)
		  0C - Rivet level 1 completed (end tune)
		  0D - Rivet removed
		  0E - Rivet level completed
		  0F - Gorilla roar
7c80      gfx bank select (Donkey Kong Jr. only)
7d00      digital sound trigger - walk
7d01      digital sound trigger - jump
7d02      digital sound trigger - boom (gorilla stomps foot)
7d03      digital sound trigger - coin input/spring
7d04      digital sound trigger	- gorilla fall
7d05      digital sound trigger - barrel jump/prize
7d06      ?
7d07      ?
7d80      digital sound trigger - dead
7d82      ?
7d83      ?
7d84      interrupt enable
7d85      0/1 toggle
7d86      sound processor control?
7d87      sound processor control?

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"



extern void dkongjr_gfxbank_w(int offset,int data);
extern int dkong_vh_start(void);
extern void dkong_vh_screenrefresh(struct osd_bitmap *bitmap);

extern void dkong_sh1_w(int offset,int data);
extern void dkong_sh2_w(int offset,int data);
extern void dkong_sh3_w(int offset,int data);
extern void dkong_sh_update(void);



static struct MemoryReadAddress dkong_readmem[] =
{
	{ 0x6000, 0x6fff, MRA_RAM },	/* including sprites RAM */
	{ 0x0000, 0x3fff, MRA_ROM },
	{ 0x7c00, 0x7c00, input_port_0_r },	/* IN0 */
	{ 0x7c80, 0x7c80, input_port_1_r },	/* IN1 */
	{ 0x7d00, 0x7d00, input_port_2_r },	/* IN2 */
	{ 0x7d80, 0x7d80, input_port_3_r },	/* DSW1 */
	{ 0x7400, 0x77ff, MRA_RAM },	/* video RAM */
	{ -1 }	/* end of table */
};
static struct MemoryReadAddress dkongjr_readmem[] =
{
	{ 0x6000, 0x6fff, MRA_RAM },	/* including sprites RAM */
	{ 0x0000, 0x5fff, MRA_ROM },
	{ 0x7c00, 0x7c00, input_port_0_r },	/* IN0 */
	{ 0x7c80, 0x7c80, input_port_1_r },	/* IN1 */
	{ 0x7d00, 0x7d00, input_port_2_r },	/* IN2 */
	{ 0x7d80, 0x7d80, input_port_3_r },	/* DSW1 */
	{ 0x7400, 0x77ff, MRA_RAM },	/* video RAM */
	{ -1 }	/* end of table */
};

static struct MemoryWriteAddress dkong_writemem[] =
{
	{ 0x6000, 0x68ff, MWA_RAM },
	{ 0x6a80, 0x6fff, MWA_RAM },
	{ 0x6900, 0x6a7f, MWA_RAM, &spriteram },
	{ 0x7d84, 0x7d84, interrupt_enable_w },
	{ 0x7400, 0x77ff, videoram_w, &videoram },
	{ 0x7c80, 0x7c80, dkongjr_gfxbank_w },
	{ 0x0000, 0x3fff, MWA_ROM },
	{ 0x7800, 0x7803, MWA_RAM },	/* ???? */
	{ 0x7808, 0x7808, MWA_RAM },	/* ???? */
	{ 0x7c00, 0x7c00, dkong_sh2_w },    	/* ???? */
	{ 0x7d00, 0x7d07, dkong_sh1_w },    /* ???? */
	{ 0x7d80, 0x7d80, dkong_sh3_w },
	{ 0x7d81, 0x7d83, MWA_RAM },	/* ???? */
	{ 0x7d85, 0x7d87, MWA_RAM },	/* ???? */
	{ -1 }	/* end of table */
};
static struct MemoryWriteAddress dkongjr_writemem[] =
{
	{ 0x6000, 0x68ff, MWA_RAM },
	{ 0x6a80, 0x6fff, MWA_RAM },
	{ 0x6900, 0x6a7f, MWA_RAM, &spriteram },
	{ 0x7d84, 0x7d84, interrupt_enable_w },
	{ 0x7400, 0x77ff, videoram_w, &videoram },
	{ 0x7c80, 0x7c80, dkongjr_gfxbank_w },
	{ 0x0000, 0x5fff, MWA_ROM },
	{ 0x7800, 0x7803, MWA_RAM },	/* ???? */
	{ 0x7808, 0x7808, MWA_RAM },	/* ???? */
	{ 0x7c00, 0x7c00, MWA_RAM },	/* ???? */
	{ 0x7d00, 0x7d07, MWA_RAM },	/* ???? */
	{ 0x7d80, 0x7d83, MWA_RAM },	/* ???? */
	{ 0x7d85, 0x7d87, MWA_RAM },	/* ???? */
	{ -1 }	/* end of table */
};



static struct InputPort input_ports[] =
{
	{	/* IN0 */
		0x00,
		{ OSD_KEY_RIGHT, OSD_KEY_LEFT, OSD_KEY_UP, OSD_KEY_DOWN,
				OSD_KEY_CONTROL, 0, 0, 0 },
		{ OSD_JOY_RIGHT, OSD_JOY_LEFT, OSD_JOY_UP, OSD_JOY_DOWN,
				OSD_JOY_FIRE, 0, 0, 0 },
	},
	{	/* IN1 */
		0x00,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{	/* IN2 */
		0x00,
		{ 0, 0, OSD_KEY_1, OSD_KEY_2, 0, 0, 0, OSD_KEY_3 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{	/* DSW1 */
		0x84,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{ -1 }	/* end of table */
};



static struct DSW dsw[] =
{
	{ 3, 0x03, "LIVES", { "3", "4", "5", "6" } },
	{ 3, 0x0c, "BONUS", { "7000", "10000", "15000", "20000" } },
	{ -1 }
};


static struct GfxLayout dkong_charlayout =
{
	8,8,	/* 8*8 characters */
	256,	/* 256 characters */
	2,	/* 2 bits per pixel */
	{ 0, 256*8*8 },	/* the two bitplanes are separated */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },	/* pretty straightforward layout */
	8*8	/* every char takes 8 consecutive bytes */
};
static struct GfxLayout dkongjr_charlayout =
{
	8,8,	/* 8*8 characters */
	512,	/* 512 characters */
	2,	/* 2 bits per pixel */
	{ 0, 512*8*8 },	/* the two bitplanes are separated */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },	/* pretty straightforward layout */
	8*8	/* every char takes 8 consecutive bytes */
};
static struct GfxLayout spritelayout =
{
	16,16,	/* 16*16 sprites */
	128,	/* 128 sprites */
	2,	/* 2 bits per pixel */
	{ 0, 128*16*16 },	/* the two bitplanes are separated */
	{ 15*8, 14*8, 13*8, 12*8, 11*8, 10*8, 9*8, 8*8,
			7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7,	/* the two halves of the sprite are separated */
			64*16*16+0, 64*16*16+1, 64*16*16+2, 64*16*16+3, 64*16*16+4, 64*16*16+5, 64*16*16+6, 64*16*16+7 },
	16*8	/* every sprite takes 16 consecutive bytes */
};



static struct GfxDecodeInfo dkong_gfxdecodeinfo[] =
{
	{ 1, 0x0000, &dkong_charlayout,   0, 64 },
	{ 1, 0x1000, &spritelayout,    64*4, 16 },
	{ -1 } /* end of array */
};
static struct GfxDecodeInfo dkongjr_gfxdecodeinfo[] =
{
	{ 1, 0x0000, &dkongjr_charlayout,     0, 128 },
	{ 1, 0x2000, &spritelayout,       128*4,  16 },
	{ -1 } /* end of array */
};



static unsigned char palette[] =
{
	0x00,0x00,0x00, /* BLACK */
	0xff,0xff,0xff, /* WHITE */
	0xff,0x00,0x00, /* RED */
	0xff,0x00,0xff, /* PURPLE */
	0x00,0xff,0xff, /* CYAN */
	0xff,0xff,0x80, /* LTORANGE */
	0xdb,0x00,0x00, /* DKRED */
	0x00,0x00,0xff, /* BLUE */
	0xff,0xff,0x00, /* YELLOW */
	239,3,239,      /* PINK */
	3,180,239,      /* LTBLUE */
	255,131,3,      /* ORANGE */
	0x00,0xff,0x00, /* GREEN */
	167,3,3,        /* DKBROWN */
	255,183,115,    /* LTBROWN */
	0x00,0x46,0x00, /* DKGREEN */
};

enum { BLACK,WHITE,RED,PURPLE,CYAN,LTORANGE,DKRED,BLUE,YELLOW,PINK,
		LTBLUE,ORANGE,GREEN,DKBROWN,LTBROWN,DKGREEN };

static unsigned char dkong_colortable[] =
{
	/* chars */
	BLACK,0,0,WHITE,        /* 0-9 */
	BLACK,0,0,WHITE,        /* 0-9 */
	BLACK,0,0,WHITE,        /* 0-9 */
	BLACK,0,0,WHITE,        /* 0-9 */
	BLACK,0,0,RED,  /* A-Z */
	BLACK,0,0,RED,  /* A-Z */
	BLACK,0,0,RED,  /* A-Z */
	BLACK,0,0,RED,  /* A-Z */
	BLACK,0,0,RED,  /* A-Z */
	BLACK,0,0,RED,  /* A-Z */
	BLACK,0,0,RED,  /* A-Z */
	BLACK,0,0,RED,  /* A-Z */
	BLACK,0,0,RED,  /* A-Z */
	BLACK,0,0,RED,  /* A-Z */
	BLACK,0,0,RED,  /* A-Z */
	BLACK,0,0,RED,  /* A-Z */
	0,0,0,RED,      /* RUB END */
	0,0,0,RED,      /* RUB END */
	0,5,6,RED,      /* (C), ITC */
	0,5,6,RED,      /* (C), ITC */
	0,LTORANGE,DKRED,WHITE, /* Kong in intermission */
	0,LTORANGE,DKRED,WHITE, /* Kong in intermission */
	0,LTORANGE,DKRED,WHITE, /* Kong in intermission */
	0,LTORANGE,DKRED,WHITE, /* Kong in intermission */
	0,LTORANGE,DKRED,WHITE, /* Kong in intermission */
	0,LTORANGE,DKRED,WHITE, /* Kong in intermission */
	BLACK,RED,PURPLE,CYAN,  /* BONUS */
	BLACK,RED,PURPLE,CYAN,  /* BONUS */
	BLACK,RED,PURPLE,CYAN,  /* BONUS */
	BLACK,RED,PURPLE,CYAN,  /* BONUS */
	BLACK,RED,PURPLE,CYAN,  /* BONUS */
	BLACK,RED,PURPLE,CYAN,  /* BONUS */
	BLACK,0,0,LTORANGE,     /* 0-9 in intermission */
	BLACK,0,0,LTORANGE,     /* 0-9 in intermission */
	BLACK,0,0,LTORANGE,     /* 0-9 in intermission */
	BLACK,RED,PURPLE,CYAN,  /* 0-9 in intermission, BONUS */
	0,0,0,0,        /* unused */
	0,0,0,0,        /* unused */
	0,0,0,0,        /* unused */
	0,0,0,RED,      /* (TM) */
	0,0,0,0,        /* unused */
	0,0,0,0,        /* unused */
	0,0,0,0,        /* unused */
	0,0,0,0,        /* unused */
	0,CYAN,BLUE,YELLOW,     /* ziqqurat level */
	0,CYAN,BLUE,YELLOW,     /* ziqqurat level */
	0,CYAN,0,0,     /* _ ziqqurat level */
	0,0,0,0,        /* unused */
	BLACK,RED,PURPLE,CYAN,  /* barrel level */
	BLACK,RED,PURPLE,CYAN,  /* barrel level */
	0,0,0,0,        /* unused */
	0,0,0,0,        /* unused */
	BLACK,RED,PURPLE,CYAN,  /* barrel level */
	BLACK,RED,PURPLE,CYAN,  /* barrel level */
	0,0,0,0,        /* unused */
	0,0,0,CYAN,     /* HELP! */
	BLACK,RED,PURPLE,CYAN,  /* barrel level */
	BLACK,RED,PURPLE,CYAN,  /* barrel level */
	0,0,0,0,        /* unused */
	0,0,0,CYAN,     /* HELP! */
	BLACK,RED,PURPLE,CYAN,  /* barrel level */
	BLACK,RED,PURPLE,CYAN,  /* barrel level */
	0,0,0,RED,      /* ? */
	0,RED,LTORANGE,BLUE,    /* lives */

	/* sprites */
	BLACK,LTBLUE,LTBROWN,RED,       /* Fireball (When Mario has hammer) */
							/* Rotating ends on conveyors */
							/* Springy things (lift screen) */
	BLACK,RED,YELLOW,WHITE, /* Fireball (normal) */
							/* Flames (on top of oil tank) */
	BLACK,RED,LTORANGE,BLUE,        /* Mario */
	BLACK,1,2,3,                    /* -Moving Ladder (conveyor screen) */
							/* Moving Lift */
	BLACK,4,5,6,
	BLACK,7,8,9,
	BLACK,10,11,12,
	BLACK,LTBROWN,DKBROWN,WHITE,    /* Kong (Head), Hammer, Scores (100,200,500,800 etc) */
	BLACK,LTBROWN,DKBROWN,ORANGE,   /* Kong (body) */
	BLACK,ORANGE,WHITE,PINK,        /* girl (Head), Heart (when screen completed) */
	BLACK,WHITE,BLUE,PINK,  /* Girl (lower half), Umbrella, Purse, hat */
	BLACK,ORANGE,BLUE,YELLOW,       /* Rolling Barrel (type 1), Standing Barrel (near Kong) */
	BLACK,WHITE,LTBLUE,BLUE,        /* Oil tank, Rolling Barrel (type 2), Explosion (barrel hit withhammer) */
	BLACK,3,4,5,
	BLACK,GREEN,1,2,        /* -Pies (Conveyor screen) */
	BLACK,YELLOW,RED,BLACK  /* -Thing at top/bottom of lifts, Clipping sprite (all black) */
};

static unsigned char dkongjr_colortable[] =
{
	/* chars (first bank) */
	BLACK,BLACK,BLACK,WHITE,       /* 0-9 */
	BLACK,BLACK,BLACK,WHITE,       /* 0-9 */
	BLACK,BLACK,BLACK,WHITE,       /* 0-9 */
	BLACK,BLACK,BLACK,WHITE,       /* 0-9 */
	BLACK,BLACK,BLACK,RED,         /* A-Z */
	BLACK,BLACK,BLACK,RED,         /* A-Z */
	BLACK,BLACK,BLACK,RED,         /* A-Z */
	BLACK,BLACK,BLACK,RED,         /* A-Z */
	BLACK,BLACK,BLACK,RED,         /* A-Z */
	BLACK,BLACK,BLACK,RED,         /* A-Z */
	BLACK,BLACK,BLACK,RED,         /* A-Z */
	BLACK,BLACK,BLACK,RED,         /* A-Z */
	BLACK,BLACK,BLACK,RED,         /* A-Z */
	BLACK,BLACK,BLACK,RED,         /* A-Z */
	BLACK,BLACK,BLACK,RED,         /* A-Z */
	BLACK,BLACK,BLACK,RED,         /* A-Z */
	BLACK,BLACK,BLACK,WHITE,       /* RUB END */
	BLACK,BLACK,BLACK,WHITE,       /* RUB END */
	BLACK,BLACK,BLACK,WHITE,       /* RUB END */
	BLACK,BLACK,BLACK,WHITE,       /* RUB END */
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	BLACK,BLACK,RED,BLUE,          /* Bonus */
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	BLACK,BLACK,RED,BLUE,          /* Bonus Box */
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	BLACK,BLACK,RED,BLUE,          /* Bonus Box */
	BLACK,BLACK,RED,BLUE,          /* Bonus Box */
	BLACK,BLACK,RED,BLUE,          /* Bonus Box */
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	BLACK,BLACK,ORANGE,BLACK,      /* Screen1: Islands Bottom */
	BLACK,LTBLUE,BLUE,RED,         /* Screen4: Locks */
	BLACK,LTBLUE,BLUE,WHITE,       /* Screen1: Water */
	BLACK,BLACK,BLUE,BLACK,        /* Screen4: Rope Cage-Lock */
	BLACK,BLACK,BLUE,BLACK,        /* Screen4: Rope Cage-Lock */
	BLACK,GREEN,RED,LTBROWN,       /* Screen1: Vines */
	BLACK,GREEN,RED,RED,           /* Rope & Chains */
	BLACK,GREEN,GREEN,WHITE,       /* Screen4: Wall */
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,1,2,3,                       /* ?? */
	0,4,5,6,                       /* ?? */
	BLACK,GREEN,DKGREEN,WHITE,     /* Screen1: Islands Top */
	BLACK,GREEN,DKGREEN,WHITE,     /* Screen1&2: Top Floor Bottom Part */
	0,7,8,9,                       /* ?? */
	0,10,11,12,                    /* ?? */
	BLACK,WHITE,ORANGE,YELLOW,     /* Screen1&2: Platform */
	0,13,14,15,                    /* ?? */
	0,1,1,1,                       /* ?? */
	BLACK,DKGREEN,DKBROWN,LTBROWN, /* Lives */

	/* chars (second bank) */
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLACK,      /* Logo: Donkey Kong */
	BLACK,LTORANGE,RED,BLUE,       /* Logo: Junior */
	BLACK,LTORANGE,RED,BLUE,       /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	BLACK,BLACK,RED,BLUE,          /* Logo: Junior */
	0,0,0,0,
	BLACK,GREEN,BLACK,BLACK,       /* Logo: (c) 198 */
	BLACK,GREEN,BLACK,BLACK,       /*       2 Nin   */
	BLACK,GREEN,BLACK,BLACK,       /*       tendo   */
	BLACK,GREEN,BLACK,BLACK,       /*       of Am   */
	BLACK,GREEN,BLACK,BLACK,       /*       erica   */
	BLACK,GREEN,BLACK,BLACK,       /*       Inc.    */
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,

	/* sprites */
	BLACK,LTBROWN,BLUE,RED,        /* Mario */
	BLACK,BLACK,BLACK,WHITE,       /* Bonus Score */
	BLACK,GREEN,ORANGE,WHITE,      /* Screen2: Moving Platform */
	BLACK,1,2,3,                   /* ?? */
	BLACK,YELLOW,BLUE,WHITE,       /* Cursor */
	BLACK,4,5,6,                   /* ?? */
	BLACK,7,8,9,                   /* ?? */
	BLACK,WHITE,DKBROWN,LTBROWN,   /* Kong Jr */
	BLACK,BLUE,BLUE,LTBLUE,        /* Cage */
	BLACK,ORANGE,DKBROWN,LTBROWN,  /* Kong */
	BLACK,1,7,9,                   /* ?? */
	BLACK,GREEN,ORANGE,YELLOW,     /* Banana & Pear */
	BLACK,GREEN,RED,WHITE,         /* Apple */
	BLACK,CYAN,BLUE,WHITE,         /* Blue Creature */
	BLACK,YELLOW,RED,WHITE,        /* Red Creature */
	BLACK,YELLOW,PURPLE,WHITE      /* Key */
};



static struct MachineDriver dkong_machine_driver =
{
	/* basic machine hardware */
	{
		{
			CPU_Z80,
			3072000,	/* 3.072 Mhz (?) */
			0,
			dkong_readmem,dkong_writemem,0,0,
			nmi_interrupt,1
		}
	},
	60,
	0,

	/* video hardware */
	32*8, 32*8, { 2*8, 30*8-1, 0*8, 32*8-1 },
	dkong_gfxdecodeinfo,
	sizeof(palette)/3,sizeof(dkong_colortable),
	0,

	0,
	dkong_vh_start,
	generic_vh_stop,
	dkong_vh_screenrefresh,

	/* sound hardware */
	0,
	0,
	0,
	0,
	dkong_sh_update
};



static struct MachineDriver dkongjr_machine_driver =
{
	/* basic machine hardware */
	{
		{
			CPU_Z80,
			3072000,	/* 3.072 Mhz (?) */
			0,
			dkongjr_readmem,dkongjr_writemem,0,0,
			nmi_interrupt,1
		}
	},
	60,
	0,

	/* video hardware */
	32*8, 32*8, { 2*8, 30*8-1, 0*8, 32*8-1 },
	dkongjr_gfxdecodeinfo,
	sizeof(palette)/3,sizeof(dkongjr_colortable),
	0,

	0,
	dkong_vh_start,
	generic_vh_stop,
	dkong_vh_screenrefresh,

	/* sound hardware */
	0,
	0,
	0,
	0,
	0
};



/***************************************************************************

  Game driver(s)

***************************************************************************/

ROM_START( dkong_rom )
	ROM_REGION(0x10000)	/* 64k for code */
	ROM_LOAD( "dk.5e",  0x0000, 0x1000 )
	ROM_LOAD( "dk.5c",  0x1000, 0x1000 )
	ROM_LOAD( "dk.5b",  0x2000, 0x1000 )
	ROM_LOAD( "dk.5a",  0x3000, 0x1000 )

	ROM_REGION(0x3000)	/* temporary space for graphics (disposed after conversion) */
	ROM_LOAD( "dk.3n",  0x0000, 0x0800 )
	ROM_LOAD( "dk.3p",  0x0800, 0x0800 )
	ROM_LOAD( "dk.7c",  0x1000, 0x0800 )
	ROM_LOAD( "dk.7d",  0x1800, 0x0800 )
	ROM_LOAD( "dk.7e",  0x2000, 0x0800 )
	ROM_LOAD( "dk.7f",  0x2800, 0x0800 )

	ROM_REGION(0x1000)	/* sound? */
	ROM_LOAD( "dk.3h",  0x0000, 0x0800 )
	ROM_LOAD( "dk.3f",  0x0800, 0x0800 )
ROM_END

ROM_START( dkongjr_rom )
	ROM_REGION(0x10000)	/* 64k for code */
	ROM_LOAD( "dkj.5b",  0x0000, 0x1000 )
	ROM_CONTINUE(        0x3000, 0x1000 )
	ROM_LOAD( "dkj.5c",  0x2000, 0x0800 )
	ROM_CONTINUE(        0x4800, 0x0800 )
	ROM_CONTINUE(        0x1000, 0x0800 )
	ROM_CONTINUE(        0x5800, 0x0800 )
	ROM_LOAD( "dkj.5e",  0x4000, 0x0800 )
	ROM_CONTINUE(        0x2800, 0x0800 )
	ROM_CONTINUE(        0x5000, 0x0800 )
	ROM_CONTINUE(        0x1800, 0x0800 )

	ROM_REGION(0x4000)	/* temporary space for graphics (disposed after conversion) */
	ROM_LOAD( "dkj.3n",  0x0000, 0x1000 )
	ROM_LOAD( "dkj.3p",  0x1000, 0x1000 )
	ROM_LOAD( "dkj.7c",  0x2000, 0x0800 )
	ROM_LOAD( "dkj.7d",  0x2800, 0x0800 )
	ROM_LOAD( "dkj.7e",  0x3000, 0x0800 )
	ROM_LOAD( "dkj.7f",  0x3800, 0x0800 )

	ROM_REGION(0x1000)	/* sound? */
	ROM_LOAD( "dkj.3h",  0x0000, 0x1000 )
ROM_END



static const char *sample_names[] =
{
	"walk.sam",
	"jump.sam",
	"boom.sam",
	"coin.sam",
	"drop.sam",
	"prize.sam",
	"",
	"",
	"",
	"intro.sam",
	"howhigh.sam",
	"time.sam",
	"hammer.sam",
	"rivet2a.sam",
	"hamhit.sam",
	"lvl1end.sam",
	"back1.sam",
	"",
	"back3.sam",
	"back2.sam",
	"rivet1a.sam",
	"",
	"rivet1.sam",
	"gorilla.sam",
	"death.sam",
	0	/* end of array */
};



static int hiload(const char *name)
{
	/* check if the hi score table has already been initialized */
	if (memcmp(&RAM[0x611d],"\x50\x76\x00",3) == 0 &&
			memcmp(&RAM[0x61a5],"\x00\x43\x00",3) == 0)
	{
		FILE *f;


		if ((f = fopen(name,"rb")) != 0)
		{
			fread(&RAM[0x6100],1,34*5,f);
			RAM[0x60b8] = RAM[0x611d];
			RAM[0x60b9] = RAM[0x611e];
			RAM[0x60ba] = RAM[0x611f];
			/* also copy the high score to the screen, otherwise it won't be */
			/* updated until a new game is started */
			cpu_writemem(0x7621,RAM[0x6108]);
			cpu_writemem(0x7601,RAM[0x6109]);
			cpu_writemem(0x75e1,RAM[0x610a]);
			cpu_writemem(0x75c1,RAM[0x610b]);
			cpu_writemem(0x75a1,RAM[0x610b]);
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
		fwrite(&RAM[0x6100],1,34*5,f);
		fclose(f);
	}
}



struct GameDriver dkong_driver =
{
	"dkong",
	&dkong_machine_driver,

	dkong_rom,
	0, 0,
	sample_names,

	input_ports, dsw,

	0, palette, dkong_colortable,
	{ 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,	/* numbers */
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,	/* letters */
		0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a },
	0, 45,
	8*13, 8*16, 4,

	hiload, hisave
};

struct GameDriver dkongjr_driver =
{
	"dkongjr",
	&dkongjr_machine_driver,

	dkongjr_rom,
	0, 0,
	0,

	input_ports, dsw,

	0, palette, dkongjr_colortable,
	{ 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,	/* numbers */
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,	/* letters */
		0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a },
	0, 60,
	8*13, 8*16, 4,

	hiload, hisave
};
