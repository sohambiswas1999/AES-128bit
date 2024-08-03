#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main()
{
    /*
     unsigned char a[8] = {1, 1, 0, 0, 1, 1, 0, 0};
      unsigned char b[8] = {1, 1, 0, 1, 0, 0, 1, 1};
     char *c = (char *)malloc(sizeof(unsigned char)*16);

         for (int i = 0; i < 7; i++)
     {

     }

     char *str = (char *)malloc(sizeof(uint16_t) * 1);
     sprintf(str, "%x", a);
     printf("%s", str);*/

    uint16_t result = 0x00;
    uint8_t b = 0xcc;
    uint16_t a = 0xd8;
    result = result ^ a;

    for (int i = 0; i < 8; i++)
    {
        result = result ^ ((a << i) * ((b >> i) & 0x01));
    }
    char *str = (char *)malloc(sizeof(uint8_t) * 1);
    sprintf(str, "%x", result);
    printf("%x", result);
}