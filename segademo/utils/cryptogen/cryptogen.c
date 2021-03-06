#include <stdio.h>
#include <stdlib.h>
#include "pts.h"

#define NUM_LAYERS 5

#define ORIG   0
#define NOISE  1
#define XOR    2
#define MASKED1 3
#define MASKED2 4

unsigned char *img = (unsigned char*)header_data;
unsigned char *images[NUM_LAYERS];
unsigned char *layers[NUM_LAYERS];

void print_buffer(unsigned char*, int w, int h);
void patternize(unsigned char*, unsigned char*, int w, int h);
void print_array(unsigned char*, int size, char last);

int main(void) {	
	int i,j;
	for (i=0;i<NUM_LAYERS;i++) {
		images[i] = malloc(width*height);
		layers[i] = malloc(width*height/4);
	}

	srand(123);

	for (i=0;i<width*height;i++) {
		images[ORIG][i]   = img[i]^1;
		images[NOISE][i]  = rand()/(RAND_MAX/2);
		images[XOR][i]    = images[NOISE][i]^images[ORIG][i];
		images[MASKED1][i] = images[NOISE][i]&images[ORIG][i];
		images[MASKED2][i] = images[XOR][i]&images[ORIG][i];
	}

	print_buffer(images[ORIG], width, height);
	fprintf(stderr,"noise:\n");
	print_buffer(images[NOISE], width, height);
	fprintf(stderr,"xor:\n");
	print_buffer(images[XOR], width, height);
	fprintf(stderr,"masked:\n");
	print_buffer(images[MASKED1], width, height);
	print_buffer(images[MASKED2], width, height);
	fprintf(stderr,"result:\n");
	for (i=0;i<56;i++) {
		for (j=0;j<80;j++) {
			fprintf(stderr, "%c", images[XOR][i*80+j]||images[NOISE][i*80+j] ? '#'  : 32);
		}
		fprintf(stderr, "\n");
	}

	for (i=0;i<NUM_LAYERS;i++) 
		patternize(images[i], layers[i], width, height);

	printf("/* Generated by cryptogen.c. Do not modify directly. */\n");
	printf("#include <genesis.h>\n");
	printf("#include \"crypto_pts_images.h\"\n");
	printf("static const u8 width = %d;\nstatic const u8 height = %d;\n\nconst u8 images[%d][%d] = {\n", width/2, height/2,NUM_LAYERS,width*height/4);
	for (i=0;i<NUM_LAYERS;i++)
		print_array(layers[i], width*height/4, i==NUM_LAYERS-1);

	printf("};\n");
	printf("u8 crypto_get_width() {return width;}\n");
	printf("u8 crypto_get_height() {return height;}\n");
	printf("const u8 *crypto_get_image(enum crypto_image_name image_id) {return images[image_id];}\n");

	for (i=0;i<NUM_LAYERS;i++) {
		free(images[i]);
		free(layers[i]);
	}

	return 0;
}

void print_buffer(unsigned char* buffer, int w, int h) {
	int i,j;
	for (i=0;i<h;i++) {
		for (j=0;j<w;j++) {
			fprintf(stderr, "%c", buffer[i*w+j] ? '#'  : 32);
		}
		fprintf(stderr, "\n");
	}

}

void patternize(unsigned char* in, unsigned char* out, int w, int h) {
	int i,j, k;
	for (i=0;i<h;i+=2) {
		for (j=0;j<w;j+=2) {
			k = in[ i   *w+j  ] 
			  | in[ i   *w+j+1]<<1
		    	  | in[(i+1)*w+j  ]<<2
		          | in[(i+1)*w+j+1]<<3;
			out[(i/2)*w/2+j/2] = k;
		}	
	}
}

void print_array(unsigned char* array, int size, char last) {
	int i;
	printf ("{\n");
	for (i=0;i<size-1;i++) {
		printf("0x%01X,", array[i]);
		if ((i+1)%8==0)
			printf("\n");
	}
	printf ("0x%01X\n}",array[i]);
	if (!last)
		printf(",");
	else
		printf("\n");
}	
