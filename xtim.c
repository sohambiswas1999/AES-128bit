#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t xtime(uint8_t a)
{
    return ((uint16_t)a << 1) ^ (0x1b * (a >> 7 & 0x01));
}

void main()
{

    // uint8_t n = xtime(0x57);
    char *str2 = (char *)malloc(sizeof(uint8_t) * 1);
    sprintf(str2, "%x", xtime(0x57));
    printf("%x", xtime(0x8e));
    // printf("%x", *str2);
}