#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main()
{
    /* uint8_t word[4] = {0x18, 0xfe, 0x1a, 0xab};
     char *str = (char *)malloc(sizeof(uint8_t) * 4);
     sprintf(str, "%x,%x,%x,%x", word[0], word[1], word[2], word[3]);
     // sprintf(str, "%x", word[1]);
     // sprintf(str, "%x", word[2]);
     // sprintf(str, "%x", word[3]);
     printf("%s", str);
     // printf("%s", &str[1]);
     //  printf("%s", &str[2]);
     //  printf("%s", &str[3]);*/

    printf("%x", 0x8a ^ 1);

    return 0;
}