#include <genesis.h>
#include <vdp.h>
#include "interrupt_test.h"
#include "gfx.h"

int current_scroll = 0;
int scanline_number = 0;
int sine_factor_wave = 0;
int sine_factor_ripple = 0;
int sine_wave = 0;
int sine_ripple = 0;
int ukk_y_pos = 0;
int ukk_x_pos = 140;
int ascension_happened = FALSE;
int ascension_limit = 240;
int wave_height = 85;
int ukk_x_pos_counter = 0;
int ukk_y_pos_counter = 0;
int bobbing_counter = 0;
int descent_began = FALSE;
int bobbing_limit = 5;
int background_scroll = 0;

int ripple_sine_table[] = {
    2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,
    2,2,2,2,2,3,3,3,
    3,3,3,3,3,3,3,3,
    3,3,3,3,3,3,3,3,
    3,3,3,3,3,3,3,3,
    3,3,3,3,3,3,3,3,
    3,3,3,3,3,3,3,3,
    3,3,3,3,3,3,2,2,
    2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,
    2,2,2,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,2
};

int ukk_y_pos_sine_table[] = {
    1,1,1,1,1,1,
    2,2,2,2,2,3,3,3,
    3,4,4,4,4,5,5,5,
    6,6,6,7,7,7,8,
    8,8,8,8,9,9,9,10,
    10,10,11,11,11,11,12,12,
    12,12,13,13,13,13,13,14,
    14,14,14,14,14,15,15,15,
    15,15,15,15,15,15,15,15,
    15,15,15,15,15,15,15,14,
    14,14,14,14,14,13,13,13,
    13,13,12,12,12,12,11,11,
    11,11,10,10,10,9,9,9,
    8,8,8,8,7,7,7,6,
    6,6,5,5,5,4,4,4,
    4,3,3,3,3,2,2,2,
    2,2,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0
};

int ukk_x_pos_sine_table[] = {
    70,71,71,72,73,73,74,74,
    75,76,76,77,78,78,79,79,
    80,81,81,82,82,83,84,84,
    85,86,86,87,87,88,89,89,
    90,90,91,92,92,93,93,94,
    95,95,96,96,97,98,98,99,
    99,100,100,101,101,102,103,103,
    104,104,105,105,106,106,107,108,
    108,109,109,110,110,111,111,112,
    112,113,113,114,114,115,115,116,
    116,117,117,117,118,118,119,119,
    120,120,121,121,121,122,122,123,
    123,124,124,124,125,125,126,126,
    126,127,127,127,128,128,128,129,
    129,129,130,130,130,131,131,131,
    132,132,132,133,133,133,133,134,
    134,134,134,135,135,135,135,136,
    136,136,136,136,137,137,137,137,
    137,137,138,138,138,138,138,138,
    139,139,139,139,139,139,139,139,
    139,139,140,140,140,140,140,140,
    140,140,140,140,140,140,140,140,
    140,140,140,140,140,140,140,140,
    140,140,140,140,140,139,139,139,
    139,139,139,139,139,139,139,138,
    138,138,138,138,138,137,137,137,
    137,137,137,136,136,136,136,136,
    135,135,135,135,134,134,134,134,
    133,133,133,133,132,132,132,131,
    131,131,130,130,130,129,129,129,
    128,128,128,127,127,127,126,126,
    126,125,125,124,124,124,123,123,
    122,122,121,121,121,120,120,119,
    119,118,118,117,117,117,116,116,
    115,115,114,114,113,113,112,112,
    111,111,110,110,109,109,108,108,
    107,106,106,105,105,104,104,103,
    103,102,101,101,100,100,99,99,
    98,98,97,96,96,95,95,94,
    93,93,92,92,91,90,90,89,
    89,88,87,87,86,86,85,84,
    84,83,82,82,81,81,80,79,
    79,78,78,77,76,76,75,74,
    74,73,73,72,71,71,70,69,
    69,68,67,67,66,66,65,64,
    64,63,62,62,61,61,60,59,
    59,58,58,57,56,56,55,54,
    54,53,53,52,51,51,50,50,
    49,48,48,47,47,46,45,45,
    44,44,43,42,42,41,41,40,
    40,39,39,38,37,37,36,36,
    35,35,34,34,33,32,32,31,
    31,30,30,29,29,28,28,27,
    27,26,26,25,25,24,24,23,
    23,23,22,22,21,21,20,20,
    19,19,19,18,18,17,17,16,
    16,16,15,15,14,14,14,13,
    13,13,12,12,12,11,11,11,
    10,10,10,9,9,9,8,8,
    8,7,7,7,7,6,6,6,
    6,5,5,5,5,4,4,4,
    4,4,3,3,3,3,3,3,
    2,2,2,2,2,2,1,1,
    1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,1,1,1,
    1,1,1,1,1,2,2,2,
    2,2,2,3,3,3,3,3,
    3,4,4,4,4,4,5,5,
    5,5,6,6,6,6,7,7,
    7,7,8,8,8,9,9,9,
    10,10,10,11,11,11,12,12,
    12,13,13,13,14,14,14,15,
    15,16,16,16,17,17,18,18,
    19,19,19,20,20,21,21,22,
    22,23,23,23,24,24,25,25,
    26,26,27,27,28,28,29,29,
    30,30,31,31,32,32,33,34,
    34,35,35,36,36,37,37,38,
    39,39,40,40,41,41,42,42,
    43,44,44,45,45,46,47,47,
    48,48,49,50,50,51,51,52,
    53,53,54,54,55,56,56,57,
    58,58,59,59,60,61,61,62,
    62,63,64,64,65,66,66,67,
    67,68,69,69,70
};


#define RIPPLE_SINE_COUNT (sizeof(ripple_sine_table) / sizeof(ripple_sine_table[0]))
#define UKK_X_SINE_COUNT (sizeof(ukk_x_pos_sine_table) / sizeof(ukk_x_pos_sine_table[0]))
#define UKK_Y_SINE_COUNT (sizeof(ukk_y_pos_sine_table) / sizeof(ukk_y_pos_sine_table[0]))


void h_interrupt_cb(void){

    if(scanline_number > wave_height){
        current_scroll -= 7;
        background_scroll -= 7;
        VDP_setVerticalScroll(PLAN_A, current_scroll);
        VDP_setVerticalScroll(PLAN_B, background_scroll);
    }
    
    if(scanline_number > wave_height && scanline_number%2 == 0){
        VDP_setHorizontalScroll(PLAN_A, ripple_sine_table[sine_ripple] + ukk_x_pos);
        VDP_setHorizontalScroll(PLAN_B, ripple_sine_table[sine_ripple]);        
    } else if(scanline_number > wave_height && scanline_number%2 == 1){
        VDP_setHorizontalScroll(PLAN_A, -ripple_sine_table[sine_ripple] + ukk_x_pos);   
        VDP_setHorizontalScroll(PLAN_B, -ripple_sine_table[sine_ripple]);                     
    }
    
    scanline_number++;
    if(++sine_factor_wave > 70){
        sine_factor_wave = 0;
        if(++sine_wave >= RIPPLE_SINE_COUNT - 3){
            sine_wave = 0;
        } 
    }
    if(++sine_factor_ripple > 120){
        sine_factor_ripple = 0;
        if(++sine_ripple >= RIPPLE_SINE_COUNT - 3){
            sine_ripple = 0;
        } 
    }
}


void init_interrupt_test(void)
{
	SYS_disableInts();
    
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    SYS_setHIntCallback(h_interrupt_cb);
    VDP_setHInterrupt(TRUE);
    VDP_setPlanSize(64, 64);

    int ind = TILE_USERINDEX;
    VDP_drawImageEx(PLAN_A, &bga_image, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 10, 30, FALSE, TRUE);
    VDP_setPalette(PAL1, bga_image.palette->data);
    VDP_drawImageEx(PLAN_B, &bgb_image, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind + 400), -1, -1, FALSE, TRUE);
    VDP_setPalette(PAL2, bgb_image.palette->data);

    SYS_enableInts();
}

void interrupt_test(void)
{
	VDP_waitVSync();
    ukk_x_pos = 70 - ukk_x_pos_sine_table[++ukk_x_pos_counter];
    if(ukk_x_pos_counter >= UKK_X_SINE_COUNT - 2){
        ukk_x_pos_counter = 0;
    }

    current_scroll = 0 + ukk_y_pos;
    if(ascension_happened == FALSE){
        ukk_y_pos++;
        if(ukk_y_pos > ascension_limit){
            ascension_happened = TRUE;
        }
    }

    if(ascension_happened == TRUE && descent_began == FALSE){
        ukk_y_pos = ascension_limit - ukk_y_pos_sine_table[++ukk_y_pos_counter];
        if(ukk_y_pos_counter >= UKK_Y_SINE_COUNT - 2){
            ukk_y_pos_counter = 0;
            bobbing_counter++;
        }
    }

    if(descent_began == TRUE){
        ukk_y_pos--;
    }

    if(bobbing_counter > bobbing_limit){
        descent_began = TRUE;
    }
    
    scanline_number = 0;
    background_scroll = 0;
    VDP_setVerticalScroll(PLAN_A, ukk_y_pos);
    VDP_setVerticalScroll(PLAN_B, 0);
    VDP_setHorizontalScroll(PLAN_A, ukk_x_pos);
    VDP_setHorizontalScroll(PLAN_B, 0);
}

int end_interrupt_test(void)
{
    SYS_disableInts();
    SPR_end();
    VDP_setHInterrupt(FALSE);
    SYS_enableInts();
    return (0);
}
