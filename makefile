ROMFILE=sonicus1.bin
ASMFILE=sonic.asm
# /k use ifeq, etc. directives, /p - Pure binary output
# /o ae- assembler option for no automatic even on dc/dcb/ds/rs .w/l
ASMFLAGS=/k /p /o ae-
PTS_DEMO_BIN=segademo/out/pts_demo.bin

all: $(ROMFILE)

.PHONY: $(ROMFILE)
$(ROMFILE): $(ASMFILE) $(PTS_DEMO_BIN)
	wine asm68k $(ASMFLAGS) $(ASMFILE), $(ROMFILE)
# Does the console check whether the rom integrity is checked?
#	wine fixheadr.exe $(ROMFILE)


$(PTS_DEMO_BIN):
	make -C segademo out/pts_demo.bin

.PHONY: run-ra
run-ra:
	retroarch --libretro /usr/lib/x86_64-linux-gnu/libretro/genesis_plus_gx_libretro.so $(ROMFILE)

.PHONY: clean
clean:
	rm -f $(ROMFILE)
# This will fail if this is being used from some other machine without gendev
	-make -C segademo clean

