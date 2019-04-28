#!/usr/bin/env python3

def torgb(val):
    b = (val[0] >> 1) & 0b111
    g = (val[1] >> 5) & 0b111
    r = (val[1] >> 1) & 0b111
    return r,g,b

def fromrgb(r,g,b):
    return bytes([(b << 1), (g << 5) | (r << 1)])

def darken(r,g,b):
    r = max(0, r-5)
    g = max(0, g-5)
    b = max(0, b-5)
    return r,g,b

with open('palette/Marble Zone.bin', 'rb') as ghz, open('palette/DarkMZ.bin', 'wb') as dghz:
    for n, val in enumerate(iter(lambda: ghz.read(2), b'')):
        r,g,b = darken(*torgb(val))
        dghz.write(fromrgb(r,g,b))
