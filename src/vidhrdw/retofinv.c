/***************************************************************************

  vidhrdw.c

  Functions to emulate the video hardware of the machine.

***************************************************************************/

#include "driver.h"

unsigned int retofinv_videoram_size;
unsigned char *retofinv_sprite_ram1;
unsigned char *retofinv_sprite_ram2;
unsigned char *retofinv_sprite_ram3;
unsigned char *retofinv_fg_char_bank;
unsigned char *retofinv_bg_char_bank;
unsigned char *retofinv_bg_videoram;
unsigned char *retofinv_fg_videoram;
unsigned char *retofinv_bg_colorram;
unsigned char *retofinv_fg_colorram;

static unsigned char flipscreen=0;
static unsigned char *bg_dirtybuffer;
static struct osd_bitmap *bitmap_bg;



void retofinv_vh_convert_color_prom(unsigned char *palette, unsigned short *colortable,const unsigned char *color_prom)
{
	int i;
	#define TOTAL_COLORS(gfxn) (Machine->gfx[gfxn]->total_colors * Machine->gfx[gfxn]->color_granularity)
	#define COLOR(gfxn,offs) (colortable[Machine->drv->gfxdecodeinfo[gfxn].color_codes_start + offs])

	for (i = 0; i<Machine->drv->total_colors; i++)
	{
		int bit0,bit1,bit2,bit3;

		bit0 = (color_prom[2*256] >> 0) & 0x01;
		bit1 = (color_prom[2*256] >> 1) & 0x01;
		bit2 = (color_prom[2*256] >> 2) & 0x01;
		bit3 = (color_prom[2*256] >> 3) & 0x01;
		*(palette++) = 0x0e * bit0 + 0x1f * bit1 + 0x43 * bit2 + 0x8f * bit3;

		bit0 = (color_prom[1*256] >> 0) & 0x01;
		bit1 = (color_prom[1*256] >> 1) & 0x01;
		bit2 = (color_prom[1*256] >> 2) & 0x01;
		bit3 = (color_prom[1*256] >> 3) & 0x01;

		*(palette++) = 0x0e * bit0 + 0x1f * bit1 + 0x43 * bit2 + 0x8f * bit3;
		bit0 = (color_prom[0*256] >> 0) & 0x01;
		bit1 = (color_prom[0*256] >> 1) & 0x01;
		bit2 = (color_prom[0*256] >> 2) & 0x01;
		bit3 = (color_prom[0*256] >> 3) & 0x01;
		*(palette++) = 0x0e * bit0 + 0x1f * bit1 + 0x43 * bit2 + 0x8f * bit3;

		color_prom++;
	}

	/* foreground colors */

	for (i = 0; i<TOTAL_COLORS(0);i++)
	{
		if (i % 2)
			COLOR(0,i) = i/2;
		else
			COLOR(0,i) = 0;
	}

/* 	background & sprites ! WRONG !

	for (i=0; i<TOTAL_COLORS(1); i++)
	{
		COLOR(1,i) = color_prom[i];
		COLOR(2,i) = color_prom[i+1024];
	}
*/
}


int retofinv_vh_start(void)
{
	if ((bg_dirtybuffer = malloc(retofinv_videoram_size)) == 0)
	{
		return 1;
	}
	if ((bitmap_bg = osd_create_bitmap(Machine->drv->screen_width,Machine->drv->screen_height)) == 0)
	{
		free(bg_dirtybuffer);
		return 1;
	}
	memset(bg_dirtybuffer,1,retofinv_videoram_size);
	fillbitmap(bitmap_bg,0,0);
        return 0;
}

void retofinv_vh_stop(void)
{
	free(bg_dirtybuffer);
	osd_free_bitmap(bitmap_bg);
}

void retofinv_flip_screen_w(int offset, int data)
{
	flipscreen = data;
	memset(bg_dirtybuffer,1,retofinv_videoram_size);
	fillbitmap(bitmap_bg,0,0);
}

void retofinv_bg_videoram_w(int offset,int data)
{
	if (retofinv_bg_videoram[offset] != data)
	{
		bg_dirtybuffer[offset] = 1;
		retofinv_bg_videoram[offset] = data;
	}
}

void retofinv_fg_videoram_w(int offset,int data)
{
	if (retofinv_fg_videoram[offset] != data)
		retofinv_fg_videoram[offset] = data;

}

void retofinv_bg_colorram_w(int offset,int data)
{
	if (retofinv_bg_colorram[offset] != data)
	{

		bg_dirtybuffer[offset] = 1;
		retofinv_bg_colorram[offset] = data;
	}
}

void retofinv_fg_colorram_w(int offset,int data)
{
	if (retofinv_fg_colorram[offset] != data)
		retofinv_fg_colorram[offset] = data;
}

void retofinv_render_sprites(struct osd_bitmap *bitmap)
{
	int offs,sx,sy,flipx,flipy,tile,palette,size;
	int tileofs0,tileofs1,tileofs2,tileofs3;

	for (offs = 0; offs<127; offs+=2)
	{
		{
			sx = 311-(((retofinv_sprite_ram2[offs+1] & 127) << 1) +
			     	  ((retofinv_sprite_ram3[offs+1] & 128) >> 7) +
			     	  ((retofinv_sprite_ram2[offs+1] & 128) << 1));

			sy = 	  ((retofinv_sprite_ram2[offs] & 127) << 1) +
			     	  ((retofinv_sprite_ram3[offs] & 128) >> 7) +
			     	  ((retofinv_sprite_ram2[offs] & 128) << 1);

			tile    = retofinv_sprite_ram1[offs];
			size	= retofinv_sprite_ram3[offs];
			palette = retofinv_sprite_ram1[offs+1] & 0x3f;

			flipx = 0;
			flipy = 0;
			tileofs0 = 0;
			tileofs1 = 1;
			tileofs2 = 2;
			tileofs3 = 3;

			if (flipscreen)
			{
				tileofs0 = 2;
				tileofs2 = 0;
				tileofs1 = 3;
				tileofs3 = 1;
				flipx = flipy = 1;
			}

			if (!(size & 12))
			{
				/* Patch for disappearing invadres' missile,
					     could it be Z80 bug ? */
				if (tile==0x98) tile--;

				drawgfx(bitmap,Machine->gfx[2],
							tile,
							palette,
							flipx,flipy,
							sx,sy,
							&Machine->drv->visible_area,TRANSPARENCY_PEN,0);
			}
			if (size & 4)
			{
				if ((size & 8) && (flipscreen)) sx-=16;
				drawgfx(bitmap,Machine->gfx[2],
							tile+tileofs0,
							palette,
							flipx,flipy,
							sx,sy+16,
							&Machine->drv->visible_area,TRANSPARENCY_PEN,0);

				drawgfx(bitmap,Machine->gfx[2],
							tile+tileofs2,
							palette,
							flipx,flipy,
							sx,sy,
							&Machine->drv->visible_area,TRANSPARENCY_PEN,0);
			}
			if (size & 8)
			{
				if (flipscreen) sx+=32;
				drawgfx(bitmap,Machine->gfx[2],
							tile+tileofs1,
							palette,
							flipx,flipy,
							sx-16,sy+16,
							&Machine->drv->visible_area,TRANSPARENCY_PEN,0);

				drawgfx(bitmap,Machine->gfx[2],
							tile+tileofs3,
							palette,
							flipx,flipy,
							sx-16,sy,
							&Machine->drv->visible_area,TRANSPARENCY_PEN,0);
			}
		}
	}
}


void retofinv_draw_background(struct osd_bitmap *bitmap)
{
	int x,y,offs;
	int sx,sy,tile,palette,flipx,flipy;

	/* for every character in the Video RAM, check if it has been modified */
	/* since last time and update it accordingly. */

	for (y = 31; y>=0; y--)
	{
		for (x = 31; x>=0; x--)
		{
			offs = y*32+x;
			sx = (31-x) << 3;
			sy = (31-y) << 3;

			flipx = flipy =0;

			if (flipscreen)
			{
				sx = 248 - sx;
				sy = 248 - sy;
				flipx = flipy = 1;
			}
			if (bg_dirtybuffer[offs])
			{
				bg_dirtybuffer[offs] = 0;
				tile = retofinv_bg_videoram[offs]+(retofinv_bg_char_bank[0]*256);
				palette = retofinv_bg_colorram[offs] & 15;

				drawgfx(bitmap,Machine->gfx[1],
							tile,
						  	palette,
						  	flipx,flipy,
						  	sx+16,sy,
						  	&Machine->drv->visible_area,TRANSPARENCY_NONE,0);

			}
		}
	}
}


void retofinv_draw_foreground(struct osd_bitmap *bitmap)
{
	int x,y,offs;
	int sx,sy,tile,palette,flipx,flipy;

	for (x=0; x<32; x++)
	{
		for (y = 30; y<=31; y++)
		{
			offs = y*32+x;

			sx = ((62-y)+3) << 3;
			sy = (31-x) << 3;

			flipx = flipy = 0;

			if (flipscreen)
			{
				sx = 280 - sx;
				sy = 248 - sy;
				flipx = flipy = 1;
			}

			tile = retofinv_fg_videoram[offs]+(retofinv_fg_char_bank[0]*256);
			palette = retofinv_fg_colorram[offs];

			drawgfx(bitmap,Machine->gfx[0],
						  tile,
						  palette,
						  flipx,flipy,
						  sx,sy,
						  &Machine->drv->visible_area,TRANSPARENCY_PEN,0);
		}
	}

	for (x = 29; x>=2 ;x--)
	{
		for (y = 31; y>=0; y--)
		{
			offs = x*32+y;
			sy = ((31-x) << 3);
			sx = ((33-y)) << 3;

			flipx = flipy = 0;

			if (flipscreen)
			{
				sx = 280 - sx;
				sy = 248 - sy;
				flipx = flipy = 1;
			}

			tile = retofinv_fg_videoram[offs]+(retofinv_fg_char_bank[0]*256);
			palette = retofinv_fg_colorram[offs];

			drawgfx(bitmap,Machine->gfx[0],
						  tile,
						  palette,
						  flipx,flipy,
						  sx,sy,
						  &Machine->drv->visible_area,TRANSPARENCY_PEN,0);

		}
	}

	for (x=0; x<32; x++)
	{
		for (y = 1; y>=0; y--)
		{
			offs = y*32+x;
			sx = (1-y) << 3;
			sy = (31-x) << 3;

			flipx = flipy = 0;

			if (flipscreen)
			{
				sx = 280 - sx;
				sy = 248 - sy;
				flipx = flipy = 1;
			}

			tile = retofinv_fg_videoram[offs]+(retofinv_fg_char_bank[0]*256);
			palette = retofinv_fg_colorram[offs];

			drawgfx(bitmap,Machine->gfx[0],
						  tile,
						  palette,
						  flipx,flipy,
						  sx,sy,
						  &Machine->drv->visible_area,TRANSPARENCY_PEN,0);
		}
	}
}


/***************************************************************************

  Draw the game screen in the given osd_bitmap.
  Do NOT call osd_update_display() from this function, it will be called by
  the main emulation engine.

***************************************************************************/

void retofinv_vh_screenrefresh(struct osd_bitmap *bitmap,int full_refresh)
{
	retofinv_draw_background(bitmap_bg);
	copybitmap(bitmap,bitmap_bg,0,0,0,0,&Machine->drv->visible_area,TRANSPARENCY_NONE,0);
	retofinv_render_sprites(bitmap);
	retofinv_draw_foreground(bitmap);
}
