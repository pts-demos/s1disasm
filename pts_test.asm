
PTS_Test:
		sfx	bgm_Stop,0,1,1
		move.w	#-1, d0
@bl2
		nop
		dbf	d0, @bl2
		trap	#5
		sfx	sfx_Sega,0,1,1
		move.w	#-1, d0
@bl
		nop
		dbf	d0, @bl	
		move.w #$1000, d0
@bl3
		move.w #5, d1
@bl4
		dbf	d1, @bl4
		nop
		dbf	d0, @bl3
		bsr.w	PaletteFadeOut
		disable_ints
		jmp PTSDemo
