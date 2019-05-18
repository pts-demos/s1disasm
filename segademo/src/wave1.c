#include <genesis.h>
#include <timer.h>

#include "sin_wave_data.h"
#include "sin_time_data.h"
#include "pts_math.h"
#include "sin_time_data.h"
#include "sin_wave_data.h"
#include "wave1.h"

#define columnsInTile 8
#define rowsInTile 8
#define screenTileWidth 40
#define screenTileWidthQuarter 20
#define screenTileHeight 28
#define screenTileHeightQuarter 14
#define screenPixelWidth 320
#define screenPixelWidthQuarter 160
#define screenPixelHeight 240
#define screenPixelHeightQuarter 120
#define screenPixelHalfX (screenPixelWidth / 2)
#define screenPixelHalfY (screenPixelHeight / 2)

u32* wave_tilebuffer = NULL;
extern u16 rgbToU16(u8 r, u8 g, u8 b);
// distance to the screen center point
u16 distance_x = 0;
u16 distance_y = 0;
u16 pixel_x = 0;
u16 pixel_y = 0;
u8 channel_index = 0;
u32 arrIndex = 0;
u16 distance = 0;
u8* wave1_sin_wave_data;
u16 wave1_sin_wave_count;
u8* wave1_sin_time_data;
u16 wave1_sin_time_count;

u8 wave1_r = 0;
u8 wave1_g = 0;
u8 wave1_b = 0;
u16 wave1_palettes[17];

void wave1_fade(void) {
    MEM_free(wave_tilebuffer);
    SYS_disableInts();
    VDP_setHInterrupt(FALSE);
    SYS_enableInts();
    VDP_fadeOutAll(60, 0);
    VDP_clearSprites();
    VDP_resetScreen();
    VDP_setHInterrupt(0);
    VDP_setPlanSize(64, 64);
}

static void wave1_nosync(void);

void
wave1_init(void)
{
	// Don't allocate this in .text segment as it eats up too much space
	// TODO: free this
	wave_tilebuffer = MEM_alloc(screenTileWidthQuarter * screenTileHeightQuarter *
		rowsInTile * sizeof(u32));

    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

    // Create a gradient color palette
    for (int i = 1; i < 16; i++)
    {
        wave1_r += 1;
        if (wave1_r > 7) {
            wave1_r = 7;
            wave1_g++;
            if (wave1_g > 7) {
                wave1_g = 7;
                wave1_b++;
                if (wave1_b > 6) {
                    wave1_b = 7;
                }
            }
        }
        wave1_palettes[i] = rgbToU16(wave1_r, wave1_g, wave1_b);
        VDP_setPaletteColor(i, wave1_palettes[i]);
    }

	wave1_sin_wave_data = get_sin_wave_ptr();
	wave1_sin_wave_count = get_sin_wave_count();
	wave1_sin_time_data = get_sin_time_ptr();
	wave1_sin_time_count = get_sin_time_count();
	/* draw the effect once, before placing the tiles */
	wave1_nosync();

	// Draw a sequence of tiles that will be updated every frame
    // Tile 0 is background, so start indexing at 1
	// The tiles are drawn so that only the top left quarter of the screen
	// is updated - the rest of the quarters are copied from these tiles
	// so the whole screen gets updated but only quarter of it needs to be
	// computed.
	u16 tileIndex = 0;
	s16 tile_y, tile_x;
	for (tile_y = 0; tile_y < screenTileHeight; tile_y++) {
		for (tile_x = 0; tile_x < screenTileWidth; tile_x++) {
			if (tile_y < screenTileHeightQuarter) {
				if (tile_x < screenTileWidthQuarter) {
					// top left, order is normal
					tileIndex = tile_y * screenTileWidthQuarter + tile_x;
				}
				else {
					// top right, mirror along x axis
					tileIndex = tile_y * screenTileWidthQuarter +
						(screenTileWidthQuarter - (tile_x - screenTileWidthQuarter)) -1;
				}
			} else {
				if (tile_x < screenTileWidthQuarter) {
					// bottom left, flip y
					tileIndex = (screenTileHeight - tile_y -1) * screenTileWidthQuarter +
						tile_x;
				}
				else {
					// bottom right, flip x and y
					tileIndex = (screenTileHeight - tile_y -1) *
						screenTileWidthQuarter + (screenTileWidth - tile_x) -1;
				}
			}
			tileIndex += 1;

			VDP_fillTileMapRect(PLAN_A, tileIndex, tile_x, tile_y, 1, 1);
		}
	}
}

void
wave1_update_palette()
{
}

void
wave1_nosync(void)
{
    static u16 counter = 0;
    static u16 sin_time = 0;

    static u16 line_to_draw = 0;

    counter++;
	counter = counter % wave1_sin_time_count;

    sin_time = wave1_sin_time_data[counter];
	static u32 loops = 0;
	loops++;

	u16 add_y = sin_time<<1;
	u16 add_x = loops<<1;
	// As the wave pattern is drawn in the center of screen, we only need to
	// calculate one quarter of the screen - the rest can be duplicated to the
	// other quarters

	wave1_update_palette();

    for (u32 y = line_to_draw; y < screenTileHeightQuarter; y++)
    {
		// multiply by 8
        pixel_y = y << 3;
        distance_y = abs(screenPixelHalfY - pixel_y - sin_time + add_y) + loops;

        for (u32 x = 0; x < screenTileWidthQuarter; x++)
        {
			/// multiply by 8
            pixel_x = x << 3;

            distance_x = abs(screenPixelHalfX - pixel_x - sin_time + add_x) + sin_time + loops;
			distance = ((distance_x*distance_x) + (distance_y*distance_y)) >> 7;
			distance = distance % wave1_sin_wave_count;

            channel_index = wave1_sin_wave_data[distance];
            arrIndex = (y * screenTileWidth + x) * rowsInTile;

			// This test is necessary for creating a beautiful effect
			// but it's not strictly needed if some artifacts are ok
			// Not clamping this produces some noise when the effect is "zoomed in"
            if (channel_index > 15)
                channel_index = 15;

            u32 all_chans = 0;
            all_chans += (channel_index << 28);
            all_chans += (channel_index << 24);
            all_chans += (channel_index << 20);
            all_chans += (channel_index << 16);
            all_chans += (channel_index << 12);
            all_chans += (channel_index << 8);
            all_chans += (channel_index << 4);
            all_chans += (channel_index);

            for (u32 p = 0; p < 8; p++) {
                wave_tilebuffer[arrIndex + p] = all_chans;
			}
        }
    }

	// Update the tile data for the top left quarter of the screen
	// As the same tiles are used everywher else, they will also update
	u16 tileIndex = 1;
    for (u32 y = 0; y < screenTileHeightQuarter; y++)
    {
        for (u32 x = 0; x < screenTileWidthQuarter; x++)
        {
            arrIndex = (y * screenTileWidth + x) * rowsInTile;
            VDP_loadTileData((const u32*)&wave_tilebuffer[arrIndex],
				tileIndex, 1, 0);
            tileIndex++;
        }
    }

}

void
wave1(void)
{
	wave1_nosync();
	VDP_waitVSync();
}

