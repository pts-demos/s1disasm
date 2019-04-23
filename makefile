

all: full.bin

.PHONY: s1built.bin
s1built.bin: sonic.asm
	wine asm68k /k /p /o ae- sonic.asm, s1built.bin
	wine fixheadr.exe s1built.bin

full.bin: s1built.bin demo.bin
	cat s1built.bin demo.bin > full.bin

.PHONY:
run:
	retroarch --libretro /usr/lib/x86_64-linux-gnu/libretro/genesis_plus_gx_libretro.so full.bin
