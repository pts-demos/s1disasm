#include <genesis.h>
#include <timer.h>
#include "effectswitcher.h"

u32 effect_started;
static int pending_transition = 0;

void
effects_init(void)
{
	current_effect = effects;
	if (current_effect->init)
		current_effect->init();
	effect_started = getTick();
}

void
play_effect(void)
{
	if (pending_transition) {
		struct effect *next = current_effect + pending_transition;
		pending_transition = 0;
		if (next < effects || !next->effect)
			return;
		if (current_effect->transition)
			current_effect->transition();
		if (next->init)
			next->init();
		current_effect = next;
		effect_started = getTick();
	}
	current_effect->effect();
	if (!current_effect->duration)
		return;
	u32 elapsed = getTick() - effect_started;
	if (elapsed >= 3 * current_effect->duration)
		next_effect();
}

void
prev_effect(void)
{
	pending_transition = -1;
}

void
next_effect(void)
{
	pending_transition = +1;
}
