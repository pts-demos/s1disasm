GENDEV=/opt/gendev
export GENDEV
.PHONY: clean all
CLEANFILES=	out/* res/gj*

all: out/pts_demo.bin

clean:
	$(MAKE) -f $(GENDEV)/sgdk/mkfiles/makefile.gen clean
	rm -rf $(CLEANFILES)

res/gjtext.res: gjtext.png
	./makescroller gjtext
res/gjlocfi.res: gjlocfi.png
	./makescroller gjlocfi
res/gjlocpl.res: gjlocpl.png
	./makescroller gjlocpl

out/rom.bin: src/*.c res/gjtext.res res/gjlocfi.res res/gjlocpl.res
	$(MAKE) -f $(GENDEV)/sgdk/mkfiles/makefile.gen all

out/pts_demo.bin: out/rom.bin
	# This is the dirty hack solution.
	# The correct way would be to change the section start point to 0x90000
	# so that the generated image would start from correc location without
	# manual cuttingi. However, it seems that changing this from the linker
	# script screws up something else too.
	split -b 589824 out/rom.bin
	cat xab xac > out/pts_demo.bin
	rm xaa xab xac
