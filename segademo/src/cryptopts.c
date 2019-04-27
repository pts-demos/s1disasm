#include <genesis.h>
#include "cryptopts.h"
#include "crypto_pts_images.h"
#include "owb_sin.h"

#define setColor(c) VDP_setPaletteColor(1,(c)<<1|(c)<<5|(c)<<9)
#define testForNextState(scvalue) if (state_counter==(scvalue)) {state_counter=0; state++;}

#define JITTER_LENGTH 10
u8 jitter_array[] = {0,2,0,0,1,0,2,1,0,1};

void generate_sprites();
void load_image(const u8*, VDPPlan, char);

void
crypto_pts_init(void)
{
	VDP_setPaletteColor(1,0);

	generate_sprites();
	load_image(crypto_get_image(NOISE), PLAN_A, 1);
	load_image(crypto_get_image(XOR), PLAN_B, 1);
}

void
crypto_pts(void)
{
	static u16 state_counter = 0;
	static int state = 0;
	static u16 color_counter = 0;
	static u16 rotation_counter = 0;
	static u8  jitter_counter = 0;
	static u8 rotation_scale = 20;

	static u16 rotation_speed = SIN_SPAN*10;
	static u8 jitter_speed = 1;
	static u8 color_speed = 6;

	static u16 jitter2;
	static u16 jitter;

	switch (state) {
		case 0:
			testForNextState(40);
			setColor(state_counter/10);
			state_counter++;
			break;
		case 1:
			color_counter=SIN_LEN/4;
			state = 2;
			break;
		case 2:
			testForNextState(50);
			setColor(sin(color_counter)/48+1+jitter);
			state_counter++;
			break;
		case 3:
			setColor(sin(color_counter)/48+1+jitter);
			if(rotation_speed>30)rotation_speed-=5;
			while(rotation_scale>200)rotation_scale--;
			if (rotation_speed<=30) {
				rotation_speed = 30;
				state = 4;
				state_counter = 0;
			}
			break;
		case 4:
			setColor(sin(color_counter)/48+1+jitter);
			if (color_counter<=3) {
				rotation_counter=SIN_LEN/8*5;
				state_counter++;
			}
			if (rotation_counter<4||rotation_counter>SIN_LEN-4)
				rotation_speed=5;
			else
				rotation_speed=50;
			testForNextState(4);
			break;
		case 5:
			setColor(1);
			state = 6;
		case 6:
			setColor(0);
			state = 7;
		case 7:
			setColor(sin(color_counter)/48+1+jitter);
			load_image(crypto_get_image(MASKED1), PLAN_A, 0);
			load_image(crypto_get_image(MASKED2), PLAN_B, 0);
			rotation_counter = 0;
			break;

	}

	color_counter = (color_counter+color_speed)%(SIN_LEN/2);
	jitter_counter = (jitter_counter+jitter_speed)%JITTER_LENGTH;
	rotation_counter = (rotation_counter+rotation_speed/10)%SIN_LEN;

	jitter2 = jitter_array[jitter_counter+(JITTER_LENGTH/2)%JITTER_LENGTH];
	jitter = jitter_array[jitter_counter];

	// Rotate the two planes around
	u8 rs = rotation_scale/10;
	VDP_setVerticalScroll(PLAN_A, sin(rotation_counter)/rs+jitter);
	VDP_setHorizontalScroll(PLAN_A, cos(rotation_counter)/rs-SIN_SPAN/rs+jitter2);
	VDP_setVerticalScroll(PLAN_B, -sin(rotation_counter)/rs-jitter2);
	VDP_setHorizontalScroll(PLAN_B, -cos(rotation_counter)/rs+SIN_SPAN/rs-jitter);

	VDP_waitVSync();
}

/**
 * Generates and loads to vram 16 patterns which are divided into four squares
 * representing 4bit numbers.
 */
void generate_sprites() {
	u8 spritedata[16*32];
	u16 i;
	for (i=0;i<16*32;i++) 
		spritedata[i] = (i>>(((i>>1&1)|((i>>3&2)))+5)&1)*17;
	for (i=0;i<16;i++) {
		VDP_loadTileData((const u32*)(spritedata+i*32),i,1 ,0);
	}
}

/**
 * Loads pattern tile map from specified source to specified plan
 */
void load_image(const u8* src, VDPPlan plan, char add_crap) {
	u16 temp[40*23];
	u16 i;
	for (i=0;i<40*23;i++) {
		temp[i] = TILE_ATTR_FULL(0,0,0,0,src[i]);
	}
	VDP_setTileMapDataRect(plan, (const u16*)temp,0,0,40,23);

	// Add same data to borders to fill out the extra empty space
	if (!add_crap) 
		for (i=0;i<40*23;i++)
			temp[i] = 0;
	VDP_setTileMapDataRect(plan, (const u16*)temp+1,0,23,40,10);
	VDP_setTileMapDataRect(plan, (const u16*)temp+2,40,0,25,33);
	
}


