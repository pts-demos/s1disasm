#include <genesis.h>
#include <vdp.h>
#include "scroll.h"
#include "timer.h"
#include "greets.h"
#include "prerendered_cube.h"

extern u16 rgbToU16(u8 r, u8 g, u8 b);

/* ./generate_sin 550 1 180 */
static const u8 greets_vsin[] = { 90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,131,132,133,134,135,136,137,138,138,139,140,141,142,143,143,144,145,146,147,147,148,149,150,150,151,152,153,153,154,155,155,156,157,157,158,159,159,160,161,161,162,162,163,164,164,165,165,166,166,167,167,168,168,169,169,170,170,170,171,171,172,172,172,173,173,173,174,174,174,175,175,175,176,176,176,176,177,177,177,177,177,177,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,178,177,177,177,177,177,177,176,176,176,176,175,175,175,174,174,174,173,173,173,172,172,172,171,171,170,170,170,169,169,168,168,167,167,166,166,165,165,164,164,163,162,162,161,161,160,159,159,158,157,157,156,155,155,154,153,153,152,151,150,150,149,148,147,147,146,145,144,143,143,142,141,140,139,138,138,137,136,135,134,133,132,131,131,130,129,128,127,126,125,124,123,122,121,120,119,118,117,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,95,94,93,92,91,90,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,48,47,46,45,44,43,42,41,41,40,39,38,37,36,36,35,34,33,32,32,31,30,29,29,28,27,26,26,25,24,24,23,22,22,21,20,20,19,18,18,17,17,16,15,15,14,14,13,13,12,12,11,11,10,10,9,9,9,8,8,7,7,7,6,6,6,5,5,5,4,4,4,3,3,3,3,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,9,9,9,10,10,11,11,12,12,13,13,14,14,15,15,16,17,17,18,18,19,20,20,21,22,22,23,24,24,25,26,26,27,28,29,29,30,31,32,32,33,34,35,36,36,37,38,39,40,41,41,42,43,44,45,46,47,48,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88 };
#define VSIN_MAX ((sizeof(greets_vsin) / sizeof(greets_vsin[0])) - 1)

/* dummy sentinel value */
static const Image blank;

/* scroller images must be 512x16 pixels */
static const Image *scrolltext[] = {
	&greets_0,
	&greets_1,
	&greets_2,
	&greets_3,
	&greets_4,
	&blank,
	NULL,
};
static unsigned img = 0;

static void
load_next_image(void)
{
	/* draw at either 0 or 64 tiles (512px) */
	u16 xpos = (img % 2) * 64;
	VDP_clearTileMapRect(PLAN_A, xpos, 15, 64, 2);
	if (scrolltext[img] == NULL)
		return;
	u16 tileidx = TILE_USERINDEX + 40 + (img % 2) * 128;
	if (scrolltext[img] != &blank)
		VDP_drawImageEx(PLAN_A, scrolltext[img],
		    TILE_ATTR_FULL(PAL2, 0, FALSE, FALSE, tileidx),
		    xpos, 15, TRUE, TRUE);
	img++;
}

void
scroll_init(void)
{
	u16 col1 = rgbToU16(0,  0, 0);
	u16 col2 = rgbToU16(0,  0, 0);
	u16 col3 = rgbToU16(1,  1, 0);
	u16 col4 = rgbToU16(2,  2, 0);
	u16 col5 = rgbToU16(4,  2, 0);
	u16 col6 = rgbToU16(6,  3, 1);
	u16 col7 = rgbToU16(4,  3, 1);
	u16 col8 = rgbToU16(3,  5, 1);
	u16 col9 = rgbToU16(2,  3, 1);
	u16 col10 = rgbToU16(2, 2, 1);
	u16 col11 = rgbToU16(1, 1, 3);
	u16 col12 = rgbToU16(1, 1, 2);
	u16 col13 = rgbToU16(0, 1, 1);
	u16 col14 = rgbToU16(0, 1, 0);
	u16 col15 = rgbToU16(0, 0, 0);

	VDP_setPlanSize(128, 64);
	VDP_setHorizontalScroll(PLAN_B, 0);

	VDP_setPaletteColor(1, col1);
	VDP_setPaletteColor(2, col2);
	VDP_setPaletteColor(3, col3);
	VDP_setPaletteColor(4, col4);
	VDP_setPaletteColor(5, col5);
	VDP_setPaletteColor(6, col6);
	VDP_setPaletteColor(7, col7);
	VDP_setPaletteColor(8, col8);
	VDP_setPaletteColor(9, col9);
	VDP_setPaletteColor(10, col10);
	VDP_setPaletteColor(11, col11);
	VDP_setPaletteColor(12, col12);
	VDP_setPaletteColor(13, col13);
	VDP_setPaletteColor(14, col14);
	VDP_setPaletteColor(15, col15);

	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

	u8 set_tile;
	u32 empty[8] = {
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000,
		0x00000000
	};

	for (set_tile = 0; set_tile < 20; set_tile++) {
		VDP_loadTileData((const u32*)empty, set_tile, 1, 0);
	}

	cube_inc_x = 1;
	cube_inc_y = -3;

	load_next_image();
}

u8 render_tile = TILE_USERINDEX;
u8 tilebuffer[8 * 4 + 4];
s16 target_line = 0;
u8 dir = 1;

void
scroll(void) {
	static s16 scroll_count = 0;
	const s16 change_interval = 3;
	static s16 change_count = 0;
	static s16 msg_scrolloffset = 0;
	static u16 vsin_idx = 0;
	u16 assign_val = 0;
	s16 linediff;

	change_count++;

	prerendered_cube();

	if (change_count > change_interval) {
		u8 x, y;
		for (y = 0; y < 8; y++) {
			linediff = y - target_line;

			if (linediff < -3 || linediff > 3)
				assign_val = 0x00;
			else {
				assign_val = 10 + linediff;
			}

			for (x = 0; x < 4; x++) {
				tilebuffer[y * 4 + x] = (assign_val << 4) + assign_val;
			}
		}

		change_count = 0;
		render_tile++;
		if (render_tile > TILE_USERINDEX + 30)
			render_tile = TILE_USERINDEX;

		if (dir == 1)
			target_line++;
		else
			target_line--;

		if (target_line >= 7)
			dir = -1;
		else if (target_line == 0)
			dir = 1;

		VDP_loadTileData((const u32*)tilebuffer, render_tile, 1, 0);

		u16 first = VDP_getPaletteColor(1);
		u16 col;
		for (int i = 2; i < 15; i++)
		{
			col = VDP_getPaletteColor(i);
			VDP_setPaletteColor(i-1, col);
		}
		VDP_setPaletteColor(14, first);
	}

	if (scroll_count > -2048)
		VDP_fillTileMapRect(PLAN_B,
		    TILE_ATTR_FULL(PAL0, 0, FALSE, FALSE, render_tile),
		    42 - (scroll_count >> 3), 0, 1, 30);

	scroll_count -= 8;

	VDP_setHorizontalScroll(PLAN_B, scroll_count);
	msg_scrolloffset += 2;
	if (msg_scrolloffset % 512 == 0)
		load_next_image();
	VDP_setHorizontalScroll(PLAN_A, VDP_getScreenWidth()-msg_scrolloffset);
	vsin_idx = (vsin_idx + 1) % VSIN_MAX;
	VDP_setVerticalScroll(PLAN_A, -70 + greets_vsin[vsin_idx]);
	VDP_waitVSync();
}

