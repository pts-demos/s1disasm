#!/usr/bin/env python3

with open('palette/Green Hill Zone.bin', 'rb') as ghz, open('palette/DarkGHZ.bin', 'wb') as dghz:
    for n, val in enumerate(iter(lambda: ghz.read(2), b'')):
        # only modify palette line 3 (background)
        if n < 0x20 or n > 0x30:
            dghz.write(val)
            continue
        b = (val[0] >> 1) & 0b111
        g = (val[1] >> 5) & 0b111
        r = (val[1] >> 1) & 0b111
        r = max(0, r-4)
        g = max(0, g-4)
        b = max(0, b-4)
        newval = bytes([(b << 1), (g << 5) | (r << 1)])
        dghz.write(newval)
