/***************************************************************************

Vulgus memory map (preliminary)

MAIN CPU
0000-9fff ROM
cc00-cc7f Sprites
d000-d3ff Video RAM
d400-d7ff Color RAM
d800-dbff background video RAM
dc00-dfff background color RAM
e000-efff RAM

read:
c000      IN0
c001      IN1
c002      IN2
c003      DSW1
c004      DSW2

write:
c802      background x scroll low 8 bits
c803      background y scroll low 8 bits
c902      background x scroll high bit
c903      background y scroll high bit

SOUND CPU
0000-3fff ROM
4000-47ff RAM

write:
8000      YM2203 #1 control
8001      YM2203 #1 write
c000      YM2203 #2 control
c001      YM2203 #2 write

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"
#include "sndhrdw/generic.h"
#include "sndhrdw/8910intf.h"



extern int c1942_interrupt(void);

extern unsigned char *vulgus_bgvideoram,*vulgus_bgcolorram;
extern unsigned char *vulgus_scrolllow,*vulgus_scrollhigh;
extern void vulgus_bgvideoram_w(int offset,int data);
extern void vulgus_bgcolorram_w(int offset,int data);
int vulgus_vh_start(void);
void vulgus_vh_stop(void);
extern void vulgus_vh_convert_color_prom(unsigned char *palette, unsigned char *colortable,const unsigned char *color_prom);
extern void vulgus_vh_screenrefresh(struct osd_bitmap *bitmap);

extern int c1942_sh_start(void);



static struct MemoryReadAddress readmem[] =
{
	{ 0xd000, 0xefff, MRA_RAM },
	{ 0xc000, 0xc000, input_port_0_r },
	{ 0xc001, 0xc001, input_port_1_r },
	{ 0xc002, 0xc002, input_port_2_r },
	{ 0xc003, 0xc003, input_port_3_r },
	{ 0xc004, 0xc004, input_port_4_r },
	{ 0x0000, 0x9fff, MRA_ROM },
	{ -1 }	/* end of table */
};

static struct MemoryWriteAddress writemem[] =
{
	{ 0xe000, 0xefff, MWA_RAM },
	{ 0xd000, 0xd3ff, videoram_w, &videoram },
	{ 0xd400, 0xd7ff, colorram_w, &colorram },
	{ 0xd800, 0xdbff, vulgus_bgvideoram_w, &vulgus_bgvideoram },
	{ 0xdc00, 0xdfff, vulgus_bgcolorram_w, &vulgus_bgcolorram },
	{ 0xcc00, 0xcc7f, MWA_RAM, &spriteram },
	{ 0xc802, 0xc803, MWA_RAM, &vulgus_scrolllow },
	{ 0xc902, 0xc903, MWA_RAM, &vulgus_scrollhigh },
	{ 0xc800, 0xc800, sound_command_w },
	{ 0x0000, 0x9fff, MWA_ROM },
	{ -1 }	/* end of table */
};



static struct MemoryReadAddress sound_readmem[] =
{
	{ 0x4000, 0x47ff, MRA_RAM },
	{ 0x6000, 0x6000, sound_command_latch_r },
	{ 0x0000, 0x1fff, MRA_ROM },
	{ -1 }	/* end of table */
};

static struct MemoryWriteAddress sound_writemem[] =
{
	{ 0x4000, 0x47ff, MWA_RAM },
	{ 0x8000, 0x8000, AY8910_control_port_0_w },
	{ 0x8001, 0x8001, AY8910_write_port_0_w },
	{ 0xc000, 0xc000, AY8910_control_port_1_w },
	{ 0xc001, 0xc001, AY8910_write_port_1_w },
	{ 0x0000, 0x1fff, MWA_ROM },
	{ -1 }	/* end of table */
};



static struct InputPort input_ports[] =
{
	{	/* IN0 */
		0xff,
		{ OSD_KEY_1, OSD_KEY_2, 0, 0, OSD_KEY_3, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{	/* IN1 */
		0xff,
		{ OSD_KEY_RIGHT, OSD_KEY_LEFT, OSD_KEY_DOWN, OSD_KEY_UP,
				OSD_KEY_CONTROL, OSD_KEY_ALT, 0, 0 },
		{ OSD_JOY_RIGHT, OSD_JOY_LEFT, OSD_JOY_DOWN, OSD_JOY_UP,
				OSD_JOY_FIRE1, OSD_JOY_FIRE2, 0, 0 }
	},
	{	/* IN2 */
		0xff,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{	/* DSW1 */
		0xf7,
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{	/* DSW2 */
		0xff,
		{ 0, 0, 0, OSD_KEY_F2, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{ -1 }	/* end of table */
};


static struct KEYSet keys[] =
{
        { 1, 3, "MOVE UP" },
        { 1, 1, "MOVE LEFT"  },
        { 1, 0, "MOVE RIGHT" },
        { 1, 2, "MOVE DOWN" },
        { 1, 4, "FIRE" },
        { 1, 5, "MISSILE" },
        { -1 }
};


static struct DSW dsw[] =
{
	{ 3, 0x03, "LIVES", { "5", "1", "2", "3" }, 1 },
	{ 4, 0x30, "BONUS", { "30000 70000", "10000 60000", "20000 70000", "20000 60000" }, 1 },
	{ 4, 0x03, "DIFFICULTY", { "HARDEST", "HARD", "NORMAL", "EASY" }, 1 },
/* not sure about difficulty. Code perform a read and (& 0x03). NdMix */
	{ -1 }
};



static struct GfxLayout charlayout =
{
	8,8,	/* 8*8 characters */
	512,	/* 512 characters */
	2,	/* 2 bits per pixel */
	{ 4, 0 },
	{ 0*16, 1*16, 2*16, 3*16, 4*16, 5*16, 6*16, 7*16 },
	{ 8+3, 8+2, 8+1, 8+0, 3, 2, 1, 0 },
	16*8	/* every char takes 16 consecutive bytes */
};
static struct GfxLayout tilelayout =
{
	16,16,	/* 16*16 tiles */
	256,	/* 256 tiles */
	3,	/* 3 bits per pixel */
	{ 0, 0x2000*8, 0x4000*8 },	/* the bitplanes are separated */
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8,
			8*8, 9*8, 10*8, 11*8, 12*8, 13*8, 14*8, 15*8 },
	{ 16*8+7, 16*8+6, 16*8+5, 16*8+4, 16*8+3, 16*8+2, 16*8+1, 16*8+0,
			7, 6, 5, 4, 3, 2, 1, 0 },
	32*8	/* every tile takes 32 consecutive bytes */
};
static struct GfxLayout spritelayout =
{
	16,16,	/* 16*16 sprites */
	128,	/* 128 sprites */
	4,	/* 4 bits per pixel */
	{ 0x2000*8+4, 0x2000*8, 4, 0 },
	{ 0*16, 1*16, 2*16, 3*16, 4*16, 5*16, 6*16, 7*16,
			8*16, 9*16, 10*16, 11*16, 12*16, 13*16, 14*16, 15*16 },
	{ 33*8+3, 33*8+2, 33*8+1, 33*8+0, 32*8+3, 32*8+2, 32*8+1, 32*8+0,
			8+3, 8+2, 8+1, 8+0, 3, 2, 1, 0 },
	64*8	/* every sprite takes 64 consecutive bytes */
};



static struct GfxDecodeInfo gfxdecodeinfo[] =
{
	{ 1, 0x00000, &charlayout,           0, 64 },
	{ 1, 0x02000, &tilelayout,  64*4+16*16, 32*4 },
	{ 1, 0x08000, &tilelayout,  64*4+16*16, 32*4 },
	{ 1, 0x0e000, &spritelayout,      64*4, 16 },
	{ 1, 0x12000, &spritelayout,      64*4, 16 },
	{ -1 } /* end of array */
};



/* these are NOT the original color PROMs */
static unsigned char color_prom[] =
{
	/* 08E_SB-5: palette red component */
	0x00,0x00,0x00,0x04,0x06,0x07,0x00,0x08,0x05,0x00,0x08,0x00,0x07,0x00,0x00,0x00,
	0x00,0x00,0x00,0x04,0x06,0x07,0x00,0x08,0x05,0x06,0x08,0x07,0x00,0x05,0x00,0x00,
	0x00,0x09,0x08,0x06,0x05,0x04,0x07,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x07,0x06,0x07,0x00,0x08,0x05,0x06,0x08,0x00,0x07,0x00,0x00,0x00,
	0x00,0x0B,0x09,0x07,0x05,0x03,0x0C,0x0A,0x08,0x06,0x04,0x0E,0x0E,0x0D,0x07,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x08,0x05,0x0B,0x0A,0x0A,0x0B,0x0C,0x0C,0x0E,0x0C,0x00,0x03,0x04,0x06,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x08,0x05,0x0B,0x0A,0x0A,0x0B,0x0C,0x0C,0x0E,0x0C,0x00,0x03,0x04,0x06,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* 09E_SB-6: palette green component */
	0x00,0x05,0x08,0x06,0x05,0x06,0x07,0x08,0x04,0x05,0x07,0x07,0x07,0x06,0x05,0x06,
	0x00,0x05,0x08,0x06,0x05,0x06,0x07,0x08,0x05,0x06,0x07,0x07,0x04,0x06,0x05,0x06,
	0x00,0x09,0x08,0x07,0x06,0x05,0x06,0x07,0x05,0x06,0x04,0x00,0x00,0x00,0x05,0x06,
	0x00,0x05,0x08,0x06,0x05,0x06,0x07,0x08,0x04,0x05,0x07,0x07,0x07,0x06,0x05,0x06,
	0x00,0x0E,0x0C,0x0A,0x07,0x05,0x0C,0x0A,0x08,0x06,0x04,0x0C,0x09,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x0A,0x09,0x0B,0x05,0x09,0x00,0x05,0x09,0x0E,0x0C,0x0A,0x0D,0x06,0x0A,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x0A,0x09,0x0B,0x05,0x09,0x00,0x05,0x09,0x0E,0x0C,0x0A,0x0D,0x06,0x0A,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* 10E_SB-7: palette blue component */
	0x00,0x00,0x00,0x00,0x04,0x05,0x00,0x08,0x03,0x00,0x06,0x08,0x07,0x07,0x08,0x09,
	0x00,0x00,0x00,0x00,0x04,0x05,0x00,0x08,0x05,0x06,0x06,0x07,0x05,0x06,0x08,0x09,
	0x00,0x09,0x08,0x07,0x06,0x05,0x05,0x06,0x08,0x09,0x07,0x00,0x00,0x00,0x08,0x09,
	0x00,0x00,0x00,0x05,0x04,0x05,0x00,0x08,0x03,0x04,0x06,0x08,0x07,0x07,0x08,0x09,
	0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x07,0x05,0x03,0x01,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x0D,0x0B,0x0B,0x08,0x0F,0x00,0x00,0x00,0x0E,0x00,0x00,0x0B,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x0D,0x0B,0x0B,0x08,0x0F,0x00,0x00,0x00,0x0E,0x00,0x00,0x0B,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* F01_SB-0: char lookup table */
	0x0F,0x01,0x02,0x03,0x0F,0x02,0x03,0x04,0x0F,0x03,0x04,0x05,0x0F,0x04,0x05,0x06,
	0x0F,0x05,0x06,0x07,0x0F,0x06,0x07,0x08,0x0F,0x07,0x08,0x09,0x0F,0x08,0x09,0x0A,
	0x0F,0x09,0x0A,0x0B,0x0F,0x0A,0x0B,0x0C,0x0F,0x0B,0x0C,0x0D,0x0F,0x0C,0x0D,0x0E,
	0x0F,0x0D,0x0E,0x0F,0x0F,0x0E,0x0F,0x01,0x0F,0x0F,0x01,0x02,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x00,0x00,0x00,0x0F,0x01,0x01,0x01,0x0F,0x02,0x02,0x02,0x0F,0x03,0x03,0x03,
	0x0F,0x04,0x04,0x04,0x0F,0x05,0x05,0x05,0x0F,0x06,0x06,0x06,0x0F,0x07,0x07,0x07,
	0x0F,0x08,0x08,0x08,0x0F,0x09,0x09,0x09,0x0F,0x0A,0x0A,0x0A,0x0F,0x0B,0x0B,0x0B,
	0x0F,0x0C,0x0C,0x0C,0x0F,0x0D,0x0D,0x0D,0x0F,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x03,0x01,0x04,0x0F,0x04,0x03,0x05,0x0F,0x05,0x04,0x06,0x0F,0x06,0x05,0x07,
	0x0F,0x07,0x06,0x08,0x0F,0x08,0x07,0x0A,0x0F,0x0A,0x08,0x0B,0x0F,0x0B,0x0A,0x0E,
	0x0F,0x0E,0x0B,0x0D,0x0F,0x0D,0x0E,0x0C,0x0F,0x0C,0x0D,0x09,0x0F,0x09,0x0C,0x02,
	0x0F,0x02,0x09,0x01,0x0F,0x01,0x02,0x03,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x09,0x0D,0x0E,0x0F,0x04,0x0D,0x0A,0x0F,0x01,0x02,0x09,0x0F,0x01,0x00,0x03,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x09,0x09,0x09,0x0F,0x0A,0x0A,0x0A,0x0F,0x0A,0x06,0x02,0x0F,0x0C,0x0F,0x0F,
	/* 03K_SB-8: sprite lookup table */
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x06,0x08,0x0D,0x0E,0x0F,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x08,0x06,0x0D,0x0E,0x0F,
	0x00,0x06,0x07,0x08,0x09,0x0A,0x06,0x07,0x08,0x09,0x0A,0x06,0x08,0x0D,0x0E,0x0F,
	0x00,0x06,0x07,0x08,0x09,0x0A,0x06,0x07,0x08,0x09,0x0A,0x08,0x06,0x0D,0x0E,0x0F,
	0x00,0x0B,0x0C,0x0D,0x0E,0x0E,0x06,0x07,0x08,0x09,0x0A,0x06,0x08,0x0D,0x0E,0x0F,
	0x00,0x0B,0x0C,0x0D,0x0E,0x0E,0x06,0x07,0x08,0x09,0x0A,0x08,0x06,0x0D,0x0E,0x0F,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x00,0x06,0x07,0x08,0x09,0x0A,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x00,0x06,0x08,0x09,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x00,0x08,0x06,0x09,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	/* 06D_SB-4: tile lookup table */
	0x00,0x01,0x02,0x03,0x04,0x05,0x08,0x0A,0x00,0x01,0x07,0x0C,0x04,0x05,0x0E,0x0F,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x0A,0x00,0x01,0x0D,0x03,0x04,0x0E,0x09,0x0F,
	0x00,0x01,0x0A,0x03,0x04,0x05,0x06,0x08,0x00,0x01,0x02,0x0E,0x0F,0x05,0x06,0x07,
	0x00,0x01,0x08,0x03,0x0B,0x0E,0x06,0x0F,0x00,0x01,0x07,0x0B,0x09,0x0E,0x0D,0x0F,
	0x00,0x01,0x0F,0x0C,0x04,0x05,0x06,0x07,0x00,0x01,0x0A,0x0F,0x04,0x0E,0x06,0x07,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x09,0x00,0x0A,0x02,0x09,0x08,0x05,0x06,0x07,
	0x00,0x08,0x0A,0x03,0x04,0x05,0x0D,0x0B,0x00,0x01,0x02,0x03,0x0C,0x05,0x06,0x0D,
	0x00,0x01,0x02,0x03,0x05,0x08,0x09,0x0A,0x00,0x08,0x02,0x09,0x04,0x05,0x06,0x07,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x00,0x0D,0x08,0x09,0x0A,0x0B,0x0C,0x0E,
};



static struct MachineDriver machine_driver =
{
	/* basic machine hardware */
	{
		{
			CPU_Z80,
			4000000,	/* 4 Mhz (?) */
			0,
			readmem,writemem,0,0,
			c1942_interrupt,2
		},
		{
			CPU_Z80 | CPU_AUDIO_CPU,
			3000000,	/* 3 Mhz ??? */
			2,	/* memory region #2 */
			sound_readmem,sound_writemem,0,0,
			interrupt,8
		}
	},
	60,
	0,

	/* video hardware */
	32*8, 32*8, { 2*8, 30*8-1, 0*8, 32*8-1 },
	gfxdecodeinfo,
	256,64*4+16*16+4*32*8,
	vulgus_vh_convert_color_prom,

	0,
	vulgus_vh_start,
	vulgus_vh_stop,
	vulgus_vh_screenrefresh,

	/* sound hardware */
	0,
	0,
	c1942_sh_start,
	AY8910_sh_stop,
	AY8910_sh_update
};



/***************************************************************************

  Game driver(s)

***************************************************************************/

ROM_START( vulgus_rom )
	ROM_REGION(0x1c000)	/* 64k for code + 3*16k for the banked ROMs images */
	ROM_LOAD( "1-4n.bin", 0x0000, 0x2000 )
	ROM_LOAD( "1-5n.bin", 0x2000, 0x2000 )
	ROM_LOAD( "1-6n.bin", 0x4000, 0x2000 )
	ROM_LOAD( "1-7n.bin", 0x6000, 0x2000 )
	ROM_LOAD( "1-8n.bin", 0x8000, 0x2000 )

	ROM_REGION(0x16000)	/* temporary space for graphics (disposed after conversion) */
	ROM_LOAD( "1-3d.bin", 0x00000, 0x2000 )	/* characters */
	ROM_LOAD( "2-2a.bin", 0x02000, 0x2000 )	/* tiles */
	ROM_LOAD( "2-4a.bin", 0x04000, 0x2000 )	/* tiles */
	ROM_LOAD( "2-6a.bin", 0x06000, 0x2000 )	/* tiles */
	ROM_LOAD( "2-3a.bin", 0x08000, 0x2000 )	/* tiles */
	ROM_LOAD( "2-5a.bin", 0x0a000, 0x2000 )	/* tiles */
	ROM_LOAD( "2-7a.bin", 0x0c000, 0x2000 )	/* tiles */
	ROM_LOAD( "2-2n.bin", 0x0e000, 0x2000 )	/* sprites */
	ROM_LOAD( "2-4n.bin", 0x10000, 0x2000 )	/* sprites */
	ROM_LOAD( "2-3n.bin", 0x12000, 0x2000 )	/* sprites */
	ROM_LOAD( "2-5n.bin", 0x14000, 0x2000 )	/* sprites */

	ROM_REGION(0x10000)	/* 64k for the audio CPU */
	ROM_LOAD( "1-11c.bin", 0x0000, 0x2000 )
ROM_END



static int hiload(const char *name)
{
	/* get RAM pointer (this game is multiCPU, we can't assume the global */
	/* RAM pointer is pointing to the right place) */
	unsigned char *RAM = Machine->memory_region[0];


	/* check if the hi score table has already been initialized */
	if (memcmp(&RAM[0xee00],"\x00\x50\x00",3) == 0 &&
			memcmp(&RAM[0xee34],"\x00\x32\x50",3) == 0)
	{
		FILE *f;


		if ((f = fopen(name,"rb")) != 0)
		{
			fread(&RAM[0xee00],1,13*5,f);
			RAM[0xee47] = RAM[0xee00];
			RAM[0xee48] = RAM[0xee01];
			RAM[0xee49] = RAM[0xee02];
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
		fwrite(&RAM[0xee00],1,13*5,f);
		fclose(f);
	}
}



struct GameDriver vulgus_driver =
{
	"Vulgus",
	"vulgus",
	"PAUL LEAMAN\nMIRKO BUFFONI\nNICOLA SALMORIA",
	&machine_driver,

	vulgus_rom,
	0, 0,
	0,

	input_ports, dsw, keys,

	color_prom, 0, 0,
	8*13, 8*16,

	hiload, hisave
};
