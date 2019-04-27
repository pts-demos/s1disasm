#ifndef CRYPTO_PTS_IMAGES_H
#define CRYPTO_PTS_IMAGES_H
#include <genesis.h>

enum crypto_image_name {
	ORIGINAL=0,
	NOISE,
	XOR,
	MASKED1,
	MASKED2
};

u8 crypto_get_width();
u8 crypto_get_height();

const u8* crypto_get_image(enum crypto_image_name image_id);

#endif
