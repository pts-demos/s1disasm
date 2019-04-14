#!/usr/bin/env python3

def torgb(val):
    b = (val[0] >> 1) & 0b111
    g = (val[1] >> 5) & 0b111
    r = (val[1] >> 1) & 0b111
    return r,g,b

def fromrgb(r,g,b):
    return bytes([(b << 1), (g << 5) | (r << 1)])

def darken(r,g,b):
    r = max(0, r-4)
    g = max(0, g-4)
    b = max(0, b-4)
    return r,g,b

def makepurple(r,g,b):
    # add some red and blue for a purplish color
    r = min(7, r+2)
    b = min(7, b+2)
    return r,g,b

with open('palette/Green Hill Zone.bin', 'rb') as ghz, open('palette/DarkGHZ.bin', 'wb') as dghz:
    for n, val in enumerate(iter(lambda: ghz.read(2), b'')):
        # only modify:
        # - first 7 colors of palette line 3 (background; sky)
        # - 1st & 9th-11th colors of palette line 1 (non-cycling background
        # water)
        if not ((n >= 0x20 and n < 0x27) or n == 0x10 or (n >= 0x18 and n < 0x1c)):
            dghz.write(val)
            continue
        r,g,b = darken(*torgb(val))
        # only make the water purple, not the sky
        if (n == 0x10 or (n >= 0x18 and n < 0x1c)):
            r,g,b = makepurple(r,g,b)
        dghz.write(fromrgb(r,g,b))

with open('palette/Cycle - GHZ.bin', 'rb') as ghz, open('palette/Cycle-DarkGHZ.bin', 'wb') as dghz:
    for val in iter(lambda: ghz.read(2), b''):
        r,g,b = makepurple(*darken(*torgb(val)))
        dghz.write(fromrgb(r,g,b))
