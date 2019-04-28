#include <genesis.h>
#include <timer.h>
#include <z80_ctrl.h>
#include <joy.h>
#include "music.h"
#include "sin_bar.h"
#include "cryptopts.h"
#include "scroll.h"
#include "wave1.h"
#include "effectswitcher.h"
#include "transitions.h"
#include "interrupt_test.h"
#include "prerendered_cube.h"
#include "gj.h"

void
end(void)
{
	SND_stopPlay_VGM();
	VDP_resetScreen();
}

void black() {
}

void black_init() {
}

struct effect effects[] = {
	/* render func, init func, transition, duration (sec) */
	{ gj, gj_init, clear_screen, 4700 },
	{ black, black_init, clear_screen, 100 },
	{ end, NULL, clear_screen, 0 },
	{ NULL, NULL, NULL, 0 },
};

void
joy_cb(u16 joy, u16 changed, u16 state)
{
	if (state & BUTTON_LEFT)
		prev_effect();
	else if (state & BUTTON_RIGHT)
		next_effect();
}

int
main(void)
{
	VDP_setScreenHeight240();
	PSG_init();
	JOY_init();
	SND_startPlay_VGM(polttaribiisi);

#if 0
	JOY_setEventHandler(joy_cb);
#endif
	effects_init();
	while (1)
		play_effect();
}
