GENDEV=/opt/gendev
export GENDEV
.PHONY: clean all
CLEANFILES=	src/crypto_pts_images.c

all: out/rom.bin

clean:
	$(MAKE) -f $(GENDEV)/sgdk/mkfiles/makefile.gen clean
	rm -f $(CLEANFILES)

res/scroller.res: scroller.png
	./makescroller scroller
res/greets.res: greets.png
	./makescroller greets
res/gjtext.res: gjtext.png
	./makescroller gjtext

out/rom.bin: src/*.c src/crypto_pts_images.c res/scroller.res res/greets.res res/gjtext.res
	$(MAKE) -f $(GENDEV)/sgdk/mkfiles/makefile.gen all

src/crypto_pts_images.c: utils/cryptogen/cryptogen
	utils/cryptogen/cryptogen > $@

utils/cryptogen/cryptogen:
	$(MAKE) -C utils/cryptogen cryptogen
