/***************************************************************************

    vidhrdw.c

    Functions to emulate the video hardware of the machine.

***************************************************************************/

#include "driver.h"
#include "vidhrdw/generic.h"

UINT8 *tp84_videoram2, *tp84_colorram2;

int col0;

/*
sprites are multiplexed, so we have to buffer the spriteram
scanline by scanline.
*/
static UINT8 *sprite_mux_buffer;
static int scanline;

static tilemap *bg_tilemap, *fg_tilemap;

/*
-The colortable is divided in 2 part:
 -The characters colors
 -The sprites colors

-The characters colors are indexed like this:
 -2 bits from the characters
 -4 bits from the attribute in colorram
 -2 bits from col0 (d3-d4)
 -3 bits from col0 (d0-d1-d2)
-So, there is 2048 bytes for the characters

-The sprites colors are indexed like this:
 -4 bits from the sprites (16 colors)
 -4 bits from the attribute of the sprites
 -3 bits from col0 (d0-d1-d2)
-So, there is 2048 bytes for the sprites

*/
/*
     The RGB signals are generated by 3 proms 256X4 (prom 2C, 2D and 1E)
        The resistors values are:
            1K  ohm
            470 ohm
            220 ohm
            100 ohm
*/
PALETTE_INIT( tp84 )
{
	int i;
	#define TOTAL_COLORS(gfxn) (Machine->gfx[gfxn]->total_colors * Machine->gfx[gfxn]->color_granularity)
	#define COLOR(gfxn,offs) (colortable[Machine->drv->gfxdecodeinfo[gfxn].color_codes_start + offs])


	for (i = 0;i < Machine->drv->total_colors;i++)
	{
		int bit0,bit1,bit2,bit3,r,g,b;

		/* red component */
		bit0 = (color_prom[0] >> 0) & 0x01;
		bit1 = (color_prom[0] >> 1) & 0x01;
		bit2 = (color_prom[0] >> 2) & 0x01;
		bit3 = (color_prom[0] >> 3) & 0x01;
		r = 0x0e * bit0 + 0x1f * bit1 + 0x42 * bit2 + 0x90 * bit3;
		/* green component */
		bit0 = (color_prom[Machine->drv->total_colors] >> 0) & 0x01;
		bit1 = (color_prom[Machine->drv->total_colors] >> 1) & 0x01;
		bit2 = (color_prom[Machine->drv->total_colors] >> 2) & 0x01;
		bit3 = (color_prom[Machine->drv->total_colors] >> 3) & 0x01;
		g = 0x0e * bit0 + 0x1f * bit1 + 0x42 * bit2 + 0x90 * bit3;
		/* blue component */
		bit0 = (color_prom[2*Machine->drv->total_colors] >> 0) & 0x01;
		bit1 = (color_prom[2*Machine->drv->total_colors] >> 1) & 0x01;
		bit2 = (color_prom[2*Machine->drv->total_colors] >> 2) & 0x01;
		bit3 = (color_prom[2*Machine->drv->total_colors] >> 3) & 0x01;
		b = 0x0e * bit0 + 0x1f * bit1 + 0x42 * bit2 + 0x90 * bit3;

		palette_set_color(i,r,g,b);

		color_prom++;
	}

	color_prom += 2*Machine->drv->total_colors;
	/* color_prom now points to the beginning of the lookup table */


	/* characters use colors 128-255 */
	for (i = 0;i < TOTAL_COLORS(0)/8;i++)
	{
		int j;


		for (j = 0;j < 8;j++)
			COLOR(0,i+256*j) = *color_prom + 128 + 16*j;

		color_prom++;
	}

	/* sprites use colors 0-127 */
	for (i = 0;i < TOTAL_COLORS(1)/8;i++)
	{
		int j;


		for (j = 0;j < 8;j++)
		{
			if (*color_prom) COLOR(1,i+256*j) = *color_prom + 16*j;
			else COLOR(1,i+256*j) = 0;	/* preserve transparency */
		}

		color_prom++;
	}
}


WRITE8_HANDLER( tp84_videoram_w )
{
	if (videoram[offset] != data)
	{
		videoram[offset] = data;
		tilemap_mark_tile_dirty(bg_tilemap, offset);
	}
}

WRITE8_HANDLER( tp84_colorram_w )
{
	if (colorram[offset] != data)
	{
		colorram[offset] = data;
		tilemap_mark_tile_dirty(bg_tilemap, offset);
	}
}

WRITE8_HANDLER( tp84_videoram2_w )
{
	if (tp84_videoram2[offset] != data)
	{
		tp84_videoram2[offset] = data;
		tilemap_mark_tile_dirty(fg_tilemap, offset);
	}
}

WRITE8_HANDLER( tp84_colorram2_w )
{
	if (tp84_colorram2[offset] != data)
	{
		tp84_colorram2[offset] = data;
		tilemap_mark_tile_dirty(fg_tilemap, offset);
	}
}

WRITE8_HANDLER( tp84_scroll_x_w )
{
	tilemap_set_scrollx(bg_tilemap, 0, data);
}

WRITE8_HANDLER( tp84_scroll_y_w )
{
	tilemap_set_scrolly(bg_tilemap, 0, data);
}

WRITE8_HANDLER( tp84_flipscreen_x_w )
{
	flip_screen_x_set(data & 0x01);
}

WRITE8_HANDLER( tp84_flipscreen_y_w )
{
	flip_screen_y_set(data & 0x01);
}

/*****
  col0 is a register to index the color Proms
*****/
WRITE8_HANDLER( tp84_col0_w )
{
	if (col0 != data)
	{
		col0 = data;
		tilemap_mark_all_tiles_dirty(ALL_TILEMAPS);
	}
}

/* Return the current video scan line */
READ8_HANDLER( tp84_scanline_r )
{
	return scanline;
}

static void get_bg_tile_info(int tile_index)
{
	int coloffs = ((col0 & 0x18) << 1) + ((col0 & 0x07) << 6);
	int attr = colorram[tile_index];
	int code = videoram[tile_index] + ((attr & 0x30) << 4);
	int color = (attr & 0x0f) + coloffs;
	int flags = ((attr & 0x40) ? TILE_FLIPX : 0) | ((attr & 0x80) ? TILE_FLIPY : 0);

	SET_TILE_INFO(0, code, color, flags)
}

static void get_fg_tile_info(int tile_index)
{
	int coloffs = ((col0 & 0x18) << 1) + ((col0 & 0x07) << 6);
	int attr = tp84_colorram2[tile_index];
	int code = tp84_videoram2[tile_index]+ ((attr & 0x30) << 4);
	int color = (attr & 0x0f) + coloffs;
	int flags = ((attr & 0x40) ? TILE_FLIPX : 0) | ((attr & 0x80) ? TILE_FLIPY : 0) | TILE_IGNORE_TRANSPARENCY;

	SET_TILE_INFO(0, code, color, flags)
}

VIDEO_START( tp84 )
{
	bg_tilemap = tilemap_create(get_bg_tile_info, tilemap_scan_rows,
		TILEMAP_OPAQUE, 8, 8, 32, 32);

	if ( !bg_tilemap )
		return 1;

	fg_tilemap = tilemap_create(get_fg_tile_info, tilemap_scan_rows,
		TILEMAP_TRANSPARENT, 8, 8, 32, 32);

	if ( !fg_tilemap )
		return 1;

	tilemap_set_transparent_pen(fg_tilemap, 0);

	sprite_mux_buffer = auto_malloc(256 * spriteram_size);

	if (!sprite_mux_buffer)
		return 1;

	return 0;
}

static void tp84_draw_sprites(mame_bitmap *bitmap)
{
	const gfx_element *gfx = Machine->gfx[1];
	rectangle clip = Machine->visible_area;
	int offs;
	int line;
	int coloffset = ((col0&0x07) << 4);

	for (line = 0;line < 256;line++)
	{
		if (line >= Machine->visible_area.min_y && line <= Machine->visible_area.max_y)
		{
			UINT8 *sr;

			sr = sprite_mux_buffer + line * spriteram_size;
			clip.min_y = clip.max_y = line;

			for (offs = spriteram_size - 4;offs >= 0;offs -= 4)
			{
				int code,color,sx,sy,flipx,flipy;

				sx = sr[offs];
				sy = 240 - sr[offs + 3];

				if (sy > line-16 && sy <= line)
				{
					code = sr[offs + 1];
					color = (sr[offs + 2] & 0x0f) + coloffset;
					flipx = ~sr[offs + 2] & 0x40;
					flipy = sr[offs + 2] & 0x80;

					drawgfx(bitmap,gfx,
							code,
							color,
							flipx,flipy,
							sx,sy,
							&clip,TRANSPARENCY_COLOR,0);
				}
			}
		}
	}
}

VIDEO_UPDATE( tp84 )
{
	rectangle clip;

	tilemap_draw(bitmap, &Machine->visible_area, bg_tilemap, 0, 0);
	tp84_draw_sprites(bitmap);

	/* draw top fg_tilemap status layer part */
	clip.min_x = Machine->visible_area.min_x;
	clip.max_x = Machine->visible_area.min_x+15;
	clip.min_y = Machine->visible_area.min_y;
	clip.max_y = Machine->visible_area.max_y;
	tilemap_draw(bitmap, &clip, fg_tilemap, 0, 0);

	/* the middle part of fg_tilemap seems to be used as normal ram and is skipped */

	/* draw bottom fg_tilemap status layer part */
	clip.min_x = Machine->visible_area.max_x-15;
	clip.max_x = Machine->visible_area.max_x;
	clip.min_y = Machine->visible_area.min_y;
	clip.max_y = Machine->visible_area.max_y;
	tilemap_draw(bitmap, &clip, fg_tilemap, 0, 0);
}

INTERRUPT_GEN( tp84_6809_interrupt )
{
	scanline = 255 - cpu_getiloops();

	memcpy(sprite_mux_buffer + scanline * spriteram_size,spriteram,spriteram_size);

	if (scanline == 255)
		irq0_line_hold();
}
