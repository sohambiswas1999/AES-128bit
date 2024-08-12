#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t xtimes(uint8_t a)
{
    return ((uint16_t)a << 1) ^ (0x1b * (a >> 7 & 0x01));
}

uint8_t multi(uint8_t a, uint8_t b)
{
    uint8_t res = 0x00;
    while (a != 0 && b != 0)
    {

        res = res ^ (a * ((b & 0x01)));

        a = (a << 1) ^ (0x1b * (a >> 7 & 0x01));

        b = b >> 1;
        printf("%x\n", (a >> 7 & 0x01));
    }
    return res;
}

int main()
{

    /* uint16_t result = 0x00;
     uint8_t b = 0x0d;
     uint16_t a = 0x39;
     result = result ^ a;

     for (int i = 0; i < 8; i++)
     {
         result = result ^ ((a << i) * ((b >> i) & 0x01));
     }
     char *str = (char *)malloc(sizeof(uint8_t) * 1);
     sprintf(str, "%x", result);
     printf("%x\n", result);
    */
    uint8_t c = 0x57;
    uint8_t xtim1 = xtimes(c);
    char *str = (char *)malloc(sizeof(uint8_t) * 1);
    sprintf(str, "%x", xtim1);
    printf("%x", c);
    printf("%x\n", xtim1);

    uint8_t a = 0x57;
    uint8_t b = 0x13;
    uint8_t d = multi(a, b);
    // char *str1 = (char *)malloc(sizeof(uint8_t) * 1);
    // sprintf(str1, "%x", d);
    printf("%x", d);
}