/***************************************************************************

Star Force memory map (preliminary)
MAIN BOARD:
0000-7fff ROM
8000-8fff RAM
9000-93ff Video RAM
9400-97ff Color RAM
9800-987f Sprites
9c00-9d7f Palette RAM
a000-a1ff Background Video RAM #1
a800-a9ff Background Video RAM #2
b000-b1ff Background Video RAM #3

read:
d000      IN0
d001      IN1
d002      IN2
d003      ?
d004      DSW1
d005      DSW2

write:
9e20-9e21 background #1 x position
9e28-9e29 ?
9e25      background #1 y position
9e30-9e31 background #2 & #3 x position
9e35      background #2 & #3 y position
d002      watchdog reset?
d004      ?

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"
#include "sndhrdw/generic.h"
#include "sndhrdw/8910intf.h"


extern unsigned char *starforc_scrollx2,*starforc_scrolly2;
extern unsigned char *starforc_scrollx3,*starforc_scrolly3;
extern unsigned char *starforc_paletteram;
extern unsigned char *starforc_tilebackground2;
extern unsigned char *starforc_tilebackground3;
extern unsigned char *starforc_tilebackground4;
extern void starforc_tiles2_w(int offset,int data);
extern void starforc_tiles3_w(int offset,int data);
extern void starforc_tiles4_w(int offset,int data);
extern void starforc_paletteram_w(int offset,int data);

extern int starforc_vh_start(void);
extern void starforc_vh_stop(void);

extern void starforc_vh_convert_color_prom(unsigned char *palette, unsigned char *colortable,const unsigned char *color_prom);
extern void starforc_vh_screenrefresh(struct osd_bitmap *bitmap);


static struct MemoryReadAddress readmem[] =
{
	{ 0x8000, 0x97ff, MRA_RAM },
	{ 0x0000, 0x7fff, MRA_ROM },
	{ 0xa000, 0xa1ff, MRA_RAM },
	{ 0xa800, 0xa9ff, MRA_RAM },
	{ 0xb000, 0xb1ff, MRA_RAM },
	{ 0xd000, 0xd000, input_port_0_r },	/* player 1 input */
	{ 0xd001, 0xd001, input_port_0_r },	/* player 2 input (use player 1's) */
	{ 0xd002, 0xd002, input_port_1_r },	/* coin */
	{ 0xd004, 0xd004, input_port_2_r },	/* DSW1 */
	{ 0xd005, 0xd005, input_port_3_r },	/* DSW2 */
	{ -1 }  /* end of table */
};

static struct MemoryWriteAddress writemem[] =
{
	{ 0x8000, 0x8fff, MWA_RAM },
	{ 0x9000, 0x93ff, videoram_w, &videoram },
	{ 0x9400, 0x97ff, colorram_w, &colorram },
	{ 0xa000, 0xa1ff, starforc_tiles2_w, &starforc_tilebackground2 },
	{ 0xa800, 0xa9ff, starforc_tiles3_w, &starforc_tilebackground3 },
	{ 0xb000, 0xb1ff, starforc_tiles4_w, &starforc_tilebackground4 },
	{ 0x9e20, 0x9e21, MWA_RAM, &starforc_scrollx2 },
	{ 0x9e30, 0x9e31, MWA_RAM, &starforc_scrollx3 },
	{ 0x9e25, 0x9e25, MWA_RAM, &starforc_scrolly2 },
	{ 0x9e35, 0x9e35, MWA_RAM, &starforc_scrolly3 },
	{ 0x9800, 0x987f, MWA_RAM, &spriteram },
	{ 0x9c00, 0x9d7f, starforc_paletteram_w, &starforc_paletteram },
	{ 0x0000, 0x7fff, MWA_ROM },
	{ -1 }  /* end of table */
};


static struct MemoryReadAddress sound_readmem[] =
{
        { 0x4000, 0x43ff, MRA_RAM },
	{ 0x0000, 0x3fff, MRA_ROM },
	{ -1 }  /* end of table */
};

static struct MemoryWriteAddress sound_writemem[] =
{
        { 0x4000, 0x43ff, MWA_RAM },
	{ 0x0000, 0x3fff, MWA_ROM },
	{ -1 }  /* end of table */
};



static struct InputPort input_ports[] =
{
	{	/* IN0 / IN1 */
		0x00,
		{ OSD_KEY_RIGHT, OSD_KEY_LEFT, OSD_KEY_UP, OSD_KEY_DOWN,
				OSD_KEY_CONTROL, 0, 0, 0 },
		{ OSD_JOY_RIGHT, OSD_JOY_LEFT, OSD_JOY_UP, OSD_JOY_DOWN,
				OSD_JOY_FIRE, 0, 0, 0 }
	},
	{	/* IN2 */
		0x00,
		{ 0, OSD_KEY_3, OSD_KEY_1, OSD_KEY_2, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{	/* DSW1 */
		0x40,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{	/* DSW2 */
		0x00,
		{ 0, 0, 0, 0, 0, 0, OSD_KEY_F1,OSD_KEY_F2 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{ -1 }  /* end of table */
};


static struct KEYSet keys[] =
{
	{ 0, 2, "MOVE UP" },
	{ 0, 1, "MOVE LEFT"  },
	{ 0, 0, "MOVE RIGHT" },
	{ 0, 3, "MOVE DOWN" },
	{ 0, 4, "FIRE" },
	{ -1 }
};


static struct DSW dsw[] =
{
	{ 2, 0x30, "LIVES", { "3", "4", "5", "2" } },
	{ 3, 0x07, "BONUS", { "50K 200K 500K", "100K 300K 800K", "50K 200K", "100K 300K", "50K", "100K", "200K", "NONE" } },
	{ 3, 0x38, "DIFFICULTY", { "DEFAULT", "DIFFICULT 1", "DIFFICULT 2", "DIFFICULT 3", "DIFFICULT 4", "DIFFICULT 5", "UNUSED", "UNUSED" } },
	{ 2, 0x80, "DEMO SOUNDS", { "ON", "OFF" }, 1 },
	{ -1 }
};



static struct GfxLayout charlayout1 =
{
	8,8,	/* 8*8 characters */
	512,	/* 512 characters */
	3,	/* 3 bits per pixel */
	{ 0, 512*8*8, 2*512*8*8 },	/* the bitplanes are separated */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 },	/* pretty straightforward layout */
	8*8	/* every char takes 8 consecutive bytes */
};
static struct GfxLayout charlayout2 =
{
	16,16,	/* 16*16 characters */
	256,	/* 256 characters */
	3,	/* 3 bits per pixel */
	{ 2*256*16*16, 256*16*16, 0 },	/* the bitplanes are separated */
	{ 23*8, 22*8, 21*8, 20*8, 19*8, 18*8, 17*8, 16*8,
			7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7,	/* pretty straightforward layout */
			8*8+0, 8*8+1, 8*8+2, 8*8+3, 8*8+4, 8*8+5, 8*8+6, 8*8+7 },
	32*8	/* every character takes 32 consecutive bytes */
};
static struct GfxLayout charlayout3 =
{
	16,16,	/* 16*16 characters */
	128,	/* 128 characters */
	3,	/* 3 bits per pixel */
	{ 2*128*16*16, 128*16*16, 0 },	/* the bitplanes are separated */
	{ 23*8, 22*8, 21*8, 20*8, 19*8, 18*8, 17*8, 16*8,
			7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7,	/* pretty straightforward layout */
			8*8+0, 8*8+1, 8*8+2, 8*8+3, 8*8+4, 8*8+5, 8*8+6, 8*8+7 },
	32*8	/* every character takes 32 consecutive bytes */
};
static struct GfxLayout spritelayout1 =
{
	16,16,	/* 16*16 sprites */
	256,	/* 256 sprites */
	3,	/* 3 bits per pixel */
	{ 2*512*16*16, 512*16*16, 0 },	/* the bitplanes are separated */
	{ 23*8, 22*8, 21*8, 20*8, 19*8, 18*8, 17*8, 16*8,
			7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7,	/* pretty straightforward layout */
			8*8+0, 8*8+1, 8*8+2, 8*8+3, 8*8+4, 8*8+5, 8*8+6, 8*8+7 },
	32*8	/* every sprite takes 32 consecutive bytes */
};

static struct GfxLayout spritelayout2 =
{
	32,32,	/* 32*32 sprites */
	64,	/* 64 sprites */
	3,	/* 3 bits per pixel */
	{ 2*128*32*32, 128*32*32, 0 },	/* the bitplanes are separated */
	{ 87*8, 86*8, 85*8, 84*8, 83*8, 82*8, 81*8, 80*8,
			71*8, 70*8, 69*8, 68*8, 67*8, 66*8, 65*8, 64*8,
			23*8, 22*8, 21*8, 20*8, 19*8, 18*8, 17*8, 16*8,
			7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 },
	{ 0, 1, 2, 3, 4, 5, 6, 7,	/* pretty straightforward layout */
			8*8+0, 8*8+1, 8*8+2, 8*8+3, 8*8+4, 8*8+5, 8*8+6, 8*8+7,
			32*8+0, 32*8+1, 32*8+2, 32*8+3, 32*8+4, 32*8+5, 32*8+6, 32*8+7,
			40*8+0, 40*8+1, 40*8+2, 40*8+3, 40*8+4, 40*8+5, 40*8+6, 40*8+7 },
	128*8	/* every sprite takes 128 consecutive bytes */
};



static struct GfxDecodeInfo gfxdecodeinfo[] =
{
	{ 1, 0x00000, &charlayout1,      0, 8 },	/* characters */
	{ 1, 0x03000, &charlayout2,   16*8, 8 },	/* background #1 */
	{ 1, 0x09000, &charlayout2,    8*8, 8 },	/* background #2 */
	{ 1, 0x0f000, &charlayout3,   24*8, 8 },	/* star background */
	{ 1, 0x12000, &spritelayout1, 40*8, 8 },	/* normal sprites */
	{ 1, 0x14000, &spritelayout2, 40*8, 8 },	/* large sprites */
	{ -1 } /* end of array */
};



static struct MachineDriver machine_driver =
{
	/* basic machine hardware */
	{
		{
			CPU_Z80,
			4000000,	/* 4 Mhz */
			0,
			readmem,writemem,0,0,
			interrupt,1
		},
                {
                        CPU_Z80 | CPU_AUDIO_CPU,
                        4000000,        /* 4 Mhz */
                        2,
                        sound_readmem,sound_writemem,0,0,
                        nmi_interrupt,1
                }
	},
	60,
	0,

	/* video hardware */
	32*8, 32*8, { 2*8, 30*8-1, 0, 32*8-1 },
	gfxdecodeinfo,
	256, 48*8,
	starforc_vh_convert_color_prom,

	0,
	starforc_vh_start,
	starforc_vh_stop,
	starforc_vh_screenrefresh,

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

ROM_START( starforc_rom )
	ROM_REGION(0x10000)	/* 64k for code */
	ROM_LOAD( "starforc.dt1",  0x0000, 0x8000 )

	ROM_REGION(0x1f000)	/* temporary space for graphics (disposed after conversion) */
	ROM_LOAD( "starforc.dt7",  0x00000, 0x0f000 )
	ROM_LOAD( "starforc.dt2",  0x0f000, 0x04000 )
	ROM_LOAD( "starforc.dt3",  0x13000, 0x04000 )
	ROM_LOAD( "starforc.dt4",  0x17000, 0x04000 )
	ROM_LOAD( "starforc.dt5",  0x1b000, 0x04000 )

	ROM_REGION(0x10000)	/* 64k for sound board */
	ROM_LOAD( "starforc.dt6",  0x0000, 0x04000 )
ROM_END



static int hiload(const char *name)
{
	/* get RAM pointer (this game is multiCPU, we can't assume the global */
	/* RAM pointer is pointing to the right place) */
	unsigned char *RAM = Machine->memory_region[0];


	/* check if the hi score table has already been initialized */
	if (memcmp(&RAM[0x803a],"\x00\x05\x08\x00",4) == 0 &&
			memcmp(&RAM[0x809d],"\x00\x01\x80\x00",4) == 0)
	{
		FILE *f;


		if ((f = fopen(name,"rb")) != 0)
		{
			fread(&RAM[0x803a],1,11*10,f);
			RAM[0x8348] = RAM[0x803d];
			RAM[0x8349] = RAM[0x803c];
			RAM[0x834a] = RAM[0x803b];
			RAM[0x834b] = RAM[0x803a];
			fclose(f);
		}

		return 1;
	}
	else return 0;	/* we can't load the hi scores yet */
}



static void hisave(const char *name)
{
	FILE *f;
	/* get RAM pointer (this game is multiCPU, we can't assume the global */
	/* RAM pointer is pointing to the right place) */
	unsigned char *RAM = Machine->memory_region[0];


	if ((f = fopen(name,"wb")) != 0)
	{
		fwrite(&RAM[0x803a],1,11*10,f);
		fclose(f);
	}
}



struct GameDriver starforc_driver =
{
	"Star Force",
	"starforc",
	"MIRKO BUFFONI\nNICOLA SALMORIA",
	&machine_driver,

	starforc_rom,
	0, 0,
	0,

	input_ports, dsw, keys,

	0, 0, 0,
	8*13, 8*16,

	/* TODO: high score load doesn't work */
	hiload, hisave
};
