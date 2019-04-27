#include <genesis.h>
#include "transitions.h"

void
wipe_screen(void)
{
	VDP_setPaletteColor(0, 0);
	VDP_setBackgroundColor(0);
	VDP_setPlanSize(32, 32);
	for (int x = 0; x < planWidth; x += 1) {
		for (int y = 0; y < planHeight; y += 4) {
			VDP_clearTileMapRect(PLAN_WINDOW, 0, y, x, 2);
			VDP_clearTileMapRect(PLAN_A, 0, y, x, 2);
			VDP_clearTileMapRect(PLAN_B, 0, y, x, 2);

			VDP_clearTileMapRect(PLAN_WINDOW, planWidth - x, y+2, x, 2);
			VDP_clearTileMapRect(PLAN_A, planWidth - x, y+2, x, 2);
			VDP_clearTileMapRect(PLAN_B, planWidth - x, y+2, x, 2);
		}
		VDP_waitVSync();
	}
	clear_screen();
}

void
clear_preserve_sprites(void)
{
	VDP_resetScreen();
	VDP_setHInterrupt(0);
	VDP_setPlanSize(64, 64);
}

void
clear_screen(void)
{
	SYS_disableInts();
	VDP_setHInterrupt(FALSE);
	VDP_clearSprites();
	SPR_end();
	VDP_resetScreen();
	VDP_setPlanSize(64, 64);
	SYS_enableInts();
}

void fade_to_black(void)
{
	SYS_disableInts();
	VDP_setHInterrupt(FALSE);
	SYS_enableInts();
	VDP_fadeOutAll(60, 0);
	clear_screen();
}
