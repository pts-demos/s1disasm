#include <genesis.h>
#include <vdp.h>
#include "sprite_test.h"
#include "sprite.h"

#define SANIC_AMOUNT 18

/**
 * Sample main:
 * 
 #include <genesis.h>
 #include "sprite_test.h"

int main(void)
{
	init_sprite_test();
	while (1) {
		sprite_test();
	}
	return (0);
}
*/

Sprite *sprites[SANIC_AMOUNT];
int sanic_pos_x[SANIC_AMOUNT];
int sanic_pos_y[SANIC_AMOUNT];
int x_direction[SANIC_AMOUNT];
int y_direction[SANIC_AMOUNT];
int sanic_speeds[SANIC_AMOUNT];
int current_animation[SANIC_AMOUNT];

void init_sprite_test(void)
{
	// disable interrupt when accessing VDP
	SYS_disableInts();

	VDP_setScreenHeight240();
	VDP_setScreenWidth320();

	// init sprites engine
	SPR_init(SANIC_AMOUNT, 768, 768);

	// set all palette to black
	VDP_setPaletteColors(0, (u16 *)palette_black, 64);

	// VDP process done, we can re enable interrupts
	SYS_enableInts();

	// Init sanics
    for(int i = 0; i < SANIC_AMOUNT; i++){
        sprites[i] = SPR_addSprite(&sanic_sprite, 0 + i * 5, 0 + i * 5, TILE_ATTR(PAL0, TRUE, FALSE, FALSE));
        sanic_pos_x[i] = i + 8 * i;
        sanic_pos_y[i] = i + 6 * i;
        if(i % 2 == 0 ){
            x_direction[i] = 1;
            y_direction[i] = 1;
        } else {
            x_direction[i] = -1;
            y_direction[i] = -1;
            SPR_setHFlip(sprites[i], TRUE);
        }
        
        sanic_speeds[i] = i + 1;
        
        current_animation[i] = i % 7;
        SPR_setAnim(sprites[i], current_animation[i]);
    }

	VDP_setPalette(PAL0, sanic_sprite.palette->data);
}

int sprite_test(void)
{
    for(int i = 0; i < SANIC_AMOUNT; i++){
        sanic_pos_x[i] += x_direction[i] * sanic_speeds[i];
        sanic_pos_y[i] += y_direction[i] * sanic_speeds[i];
        SPR_setPosition(sprites[i], sanic_pos_x[i], sanic_pos_y[i]);
        if (sanic_pos_y[i] > screenHeight - sanic_sprite.maxNumTile || sanic_pos_y[i] < 0) {
            y_direction[i] *= -1;
            current_animation[i]++;
        }
        if (sanic_pos_x[i] > screenWidth - sanic_sprite.maxNumTile || sanic_pos_x[i] < 0) {
            x_direction[i] *= -1;
            SPR_setHFlip(sprites[i], x_direction[i] == -1);
            current_animation[i]++;
        }

        if(current_animation[i] > 7){
            current_animation[i] = 0;
        }

        SPR_setAnim(sprites[i], current_animation[i]);
    }

	SPR_update();
	VDP_waitVSync();
	return (0);
}

int end_sprite_test(void)
{
    SYS_disableInts();
    SPR_end();
    SYS_enableInts();
    return (0);
}