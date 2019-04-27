#include "pts_math.h"

// Found on the internet
u16 silly_sqrt(u32 x)
{
    u16 res = 0;
    u16 add = 0x8000;
    u8 i;
    for (i = 0; i < 16; i++)
    {
        u16 temp = res | add;
        u32 g2 = temp * temp;
        if (x >= g2)
        {
            res = temp;
        }
        add >>= 1;
    }
    return res;
}

