#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t multi(uint8_t a, uint8_t b)
{
    uint8_t res = 0x00;
    while (a != 0 && b != 0)
    {

        res = res ^ (a * ((b & 0x01)));

        a = (a << 1) ^ (0x1b * (a >> 7 & 0x01));

        b = b >> 1;
    }
    return res;
}
typedef uint8_t state_t[4][4];
uint8_t state[4][4] = {{0xd4, 0xe0, 0xb8, 0x1e},
                       {0xbf, 0xb4, 0x41, 0x27},
                       {0x5d, 0x52, 0x11, 0x98},
                       {0x30, 0xae, 0xf1, 0xe5}};

static void mixcolumn(uint8_t s[4][4])
{
    uint8_t a, b, c, d;
    for (int i = 0; i < 4; i++)
    {
        a = s[0][i];
        b = s[1][i];
        c = s[2][i];
        d = s[3][i];
        s[0][i] = multi(0x02, a) ^ multi(0x03, b) ^ c ^ d;
        s[1][i] = a ^ multi(0x02, b) ^ multi(0x03, c) ^ d;
        s[2][i] = a ^ b ^ multi(0x02, c) ^ multi(0x03, d);
        s[3][i] = multi(0x03, a) ^ b ^ c ^ multi(0x02, d);
    }
    for (int i = 0; i < 4; i++)
    {
        printf("%X,%x,%x,%x\n", s[i][0], s[i][1], s[i][2], s[i][3]);
    }
}

int main()
{
    /* uint8_t word[4] = {0x18, 0xfe, 0x1a, 0xab};
     char str = (char *)malloc(sizeof(uint8_t) * 4);
     sprintf(str, "%x,%x,%x,%x", word[0], word[1], word[2], word[3]);
     // sprintf(str, "%x", word[1]);
     // sprintf(str, "%x", word[2]);
     // sprintf(str, "%x", word[3]);
     printf("%s", str);
     // printf("%s", &str[1]);
     //  printf("%s", &str[2]);
     //  printf("%s", &str[3]);*/

    // printf("%x", 0x88 ^ 2);
    mixcolumn(state);
    for (int i = 0; i < 4; i++)
    {
        printf("%X,%x,%x,%x\n", state[i][0], state[i][1], state[i][2], state[i][3]);
    }
    return 0;
}