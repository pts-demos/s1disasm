# PTSD by pts #
## Released at Simulaatio 8

Placed 4 / 6 in the oldschool compo

* [Pouet link](http://www.pouet.net/prod.php?which=76304)
* [Youtube link](https://www.youtube.com/watch?v=a-dsYxuuVTw)

### Compiling

* Get [gendev](https://github.com/kubilus1/gendev)
* Check out gendev revision `d3506bf389214b0133c3b1e9ba8ccd873efa8d29` (Sun Aug 6 14:24:32 2017 -0400)
* Modify gendev SGDK cloning task to checkout SGDK revision `bd013e9eec94e37baac1b4351beee2a2f2661c74` (Thu Jul 20 22:07:06 2017 +0200) in `gendev/sgdk/Makefile`
* Run `make`, wait until the next ice age begins
* Add `export GENDEV=/opt/gendev` in your .bashrc or whatever
* Checkout this repo, cd in it
* `make`
* The rom image is: `out/rom.bin`

### Music howto

* Get [DefleMask](http://www.deflemask.com/)
* On Linux you need 32-bit. I use a chroot
* Open `res/pts_1989.dmf` in DefleMask
* From file menu -> save as VGM

### Credits

* owb: code
* Sonicus: code, graphics, hardware
* lotheac: code, build chain, fixing gekko's code
* Gekko: code, music, graphics, bug writing

### Useful links ###
* [SGDK](https://github.com/Stephane-D/SGDK)
* [gendev](https://github.com/kubilus1/gendev)
* [SGDK Docs](https://jerenevalainen.fi/~sonicus/sgdk/doc/)
* [Genesis docs](https://emu-docs.org/Genesis/sega2f.htm)
* [VDP docs](https://emudocs.org/Genesis/Graphics/genvdp.txt)
* [Genesis hardware notes](https://emu-docs.org/Genesis/gen-hw.txt)
* [Gendev forum](http://gendev.spritesmind.net/forum/)
* [SGDK @Gendev forum](http://gendev.spritesmind.net/forum/viewforum.php?f=19)
* [SEGA Mega Drive / Genesis hardware notes by Titan](https://docs.google.com/document/d/1ST9GbFfPnIjLT5loytFCm3pB0kWQ1Oe34DCBBV8saY8/pub)

