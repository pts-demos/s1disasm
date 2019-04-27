#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Tool for getting one's head aroudn the VDP CRAM color registers
// Prints out 16-bit binary format representation of a color register
// by converting R G and B color codes into a single 16-bit integer
// Each color channel must be less than 8 in value

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

void print_u16_bin(size_t const size, void const* const ptr)
{
	unsigned char* b = (unsigned char*)ptr;
	unsigned char byte;
	int i, j;
	for (i = size-1; i >= 0; i--)
	{
		for (j = 7; j >= 0; j--)
		{
			byte = (b[i] >> j) & 1;
			fprintf(stderr, "%u", byte);
		}
		fprintf(stderr, " ");
	}
	fprintf(stderr, "\n");
}

/**
  * Converts RGB color values into a single u16 color
  * as supported by the VDP color ram
  */
u16 rgbToU16(u8 r, u8 g, u8 b)
{
    // There are 64 * 9 bits of color ram on chip
    // They are accessed as 64 16-bit words
    // each word has the format:
    // ----bbb-ggg-rrr-
    // r = red component
    // g = green component
    // b = blue component
    // as each channel has 3 bits of information, each of them can have 8 unique values (0-7)
    // 8 * 8 * 8 = 512 possible colors

	if (r > 7 || g > 7 || b > 7) {
		fprintf(stderr, "max individual channel value is 7. got: %d %d %d\n", r, g, b);
		return 0;
	}

    u16 out = 0;
	out += (b << 9) + (g << 5) + (r << 1);
	return out;
}

int main(int argc, char** argv)
{
	u8 r, g, b = 0;
	r = 7;
	g = 7;
	b = 7;
	u16 VDP_col = rgbToU16(r, g, b);

	//print_u16_bin(sizeof(VDP_col), &VDP_col);

	u32 test_chan = 0x11111111;

	u16 channel_index = 1;
	u32 all_chans = (channel_index << 28)
		+ (channel_index << 24)
		+ (channel_index << 20)
		+ (channel_index << 16)
		+ (channel_index << 12)
		+ (channel_index << 8)
		+ (channel_index << 4)
		+ (channel_index);

	print_u16_bin(sizeof(u32), &all_chans);
	print_u16_bin(sizeof(u32), &test_chan);
	return 0;
}

