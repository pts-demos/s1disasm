#ifndef OWB_SIN_H
#define OWB_SIN_H

// Fun fact: It seems that SGDK math library uses tables to calculate these!

#define SIN_LEN 1024
#define SIN_SPAN 127

const char sin(u16 value); 
const char cos(u16 value);

#endif
