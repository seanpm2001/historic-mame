#include "driver.h"


data16_t *pirates_tx_tileram, *pirates_spriteram;
data16_t *pirates_fg_tileram,  *pirates_bg_tileram;

static struct tilemap *tx_tilemap, *fg_tilemap, *bg_tilemap;

/* Video Hardware */

/* tilemaps */

static void get_tx_tile_info(int tile_index)
{
	int code = pirates_tx_tileram[tile_index*2];
	int colr = pirates_tx_tileram[tile_index*2+1];

	SET_TILE_INFO(0,code,colr,0)
}

static void get_fg_tile_info(int tile_index)
{
	int code = pirates_fg_tileram[tile_index*2];
	int colr = pirates_fg_tileram[tile_index*2+1]+0x80;

	SET_TILE_INFO(0,code,colr,0)
}

static void get_bg_tile_info(int tile_index)
{
	int code = pirates_bg_tileram[tile_index*2];
	int colr = pirates_bg_tileram[tile_index*2+1]+ 0x100;

	SET_TILE_INFO(0,code,colr,0)
}


/* video start / update */

VIDEO_START(pirates)
{
	tx_tilemap = tilemap_create(get_tx_tile_info,tilemap_scan_cols,TILEMAP_TRANSPARENT,8,8,36,32);
	fg_tilemap = tilemap_create(get_fg_tile_info,tilemap_scan_cols,TILEMAP_TRANSPARENT,8,8,36,32);
	bg_tilemap = tilemap_create(get_bg_tile_info,tilemap_scan_cols,TILEMAP_OPAQUE,     8,8,36,32);

	if (!tx_tilemap || !fg_tilemap || !bg_tilemap)
		return 1;

	tilemap_set_transparent_pen(tx_tilemap,0);
	tilemap_set_transparent_pen(fg_tilemap,0);

	return 0;
}



WRITE16_HANDLER( pirates_tx_tileram_w )
{
	COMBINE_DATA(pirates_tx_tileram+offset);
	tilemap_mark_tile_dirty(tx_tilemap,offset/2);
}

WRITE16_HANDLER( pirates_fg_tileram_w )
{
	COMBINE_DATA(pirates_fg_tileram+offset);
	tilemap_mark_tile_dirty(fg_tilemap,offset/2);
}

WRITE16_HANDLER( pirates_bg_tileram_w )
{
	COMBINE_DATA(pirates_bg_tileram+offset);
	tilemap_mark_tile_dirty(bg_tilemap,offset/2);
}



static void draw_sprites(struct mame_bitmap *bitmap, const struct rectangle *cliprect)
{
	const struct GfxElement *gfx = Machine->gfx[1];
	data16_t *source = pirates_spriteram + 4;
	data16_t *finish = source + 0x800/2-4;

	while( source<finish )
	{
		int xpos, ypos, flipx, flipy, code, color;

		xpos = source[1] - 32;
		ypos = source[-1];	// indeed...

		if (ypos & 0x8000) break;	/* end-of-list marker */

		code = source[2] >> 2;
		color = source[0] & 0xff;
		flipx = source[2] & 2;
		flipy = source[2] & 1;

		ypos = 0xf2 - ypos;

		drawgfx(bitmap,gfx,
				code,
				color,
				flipx,flipy,
				xpos,ypos,
				cliprect,TRANSPARENCY_PEN,0);

		source+=4;
	}
}

VIDEO_UPDATE(pirates)
{
	tilemap_draw(bitmap,cliprect,bg_tilemap,0,0);
	tilemap_draw(bitmap,cliprect,fg_tilemap,0,0);
	draw_sprites(bitmap,cliprect);
	tilemap_draw(bitmap,cliprect,tx_tilemap,0,0);
}