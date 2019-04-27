all: cryptogen

cryptogen: cryptogen.c pts.h
	gcc cryptogen.c -o cryptogen
clean:
	rm -f cryptogen
	rm -f crypto_pts_images.c

.PHONY: clean
