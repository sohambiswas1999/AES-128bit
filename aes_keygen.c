#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define Nb 4
#define Nr 10
#define Nk 4

// staging the round constants for later use
static const uint8_t Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

// staging the sbox for later ref
static const uint8_t sbox[256] = {
    // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,  // 0//
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,  // 1//
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,  // 2//
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,  // 3//
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,  // 4//
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,  // 5//
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,  // 6//
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,  // 7//
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,  // 8//
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,  // 9//
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,  // a//
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,  // b//
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,  // c//
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,  // d//
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,  // e//
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}; // f//

static const uint8_t invsbox[256] = {
    // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};
// the key used
const uint8_t key[16] = {(uint8_t)0x2b, (uint8_t)0x7e, (uint8_t)0x15, (uint8_t)0x16,
                         (uint8_t)0x28, (uint8_t)0xae, (uint8_t)0xd2, (uint8_t)0xa6,
                         (uint8_t)0xab, (uint8_t)0xf7, (uint8_t)0x15, (uint8_t)0x88,
                         (uint8_t)0x09, (uint8_t)0xcf, (uint8_t)0x4f, (uint8_t)0x3c};

// array to store the round keys
static uint8_t roundkey[176];

// array to store the states
static uint8_t state[4][4] = {{0x32, 0x88, 0x31, 0xe0},
                              {0x43, 0x5a, 0x31, 0x37},
                              {0xf6, 0x30, 0x98, 0x07},
                              {0xa8, 0x8d, 0xa2, 0x34}};

static void
key_scheudling(uint8_t *roundkey, uint8_t *key)
{
    char *str1 = (char *)malloc(sizeof(uint8_t) * 4);
    printf("starting kye\n");
    for (int i = 0; i < Nk; ++i)
    {
        // first round key iteration is the default key//
        roundkey[(i * 4) + 0] = key[(i * 4) + 0];
        roundkey[(i * 4) + 1] = key[(i * 4) + 1];
        roundkey[(i * 4) + 2] = key[(i * 4) + 2];
        roundkey[(i * 4) + 3] = key[(i * 4) + 3];

        sprintf(str1, "%x,%x,%x,%x", roundkey[(i * 4) + 0], roundkey[(i * 4) + 1], roundkey[(i * 4) + 2], roundkey[(i * 4) + 3]);
        printf("%s\n", str1);
    }

    uint8_t word[4]; // array to store the word

    char *str_word = (char *)malloc(sizeof(uint8_t) * 4);
    char *str_subword = (char *)malloc(sizeof(uint8_t) * 4);
    char *str_rcon = (char *)malloc(sizeof(uint8_t) * 4);
    char *str_rcon1 = (char *)malloc(sizeof(uint8_t) * 1);

    for (int i = Nk; i < (Nr + 1) * Nb; ++i)
    {
        // defining the 1st word of ith round by last word of last round
        word[0] = roundkey[((i - 1) * 4) + 0];
        word[1] = roundkey[((i - 1) * 4) + 1];
        word[2] = roundkey[((i - 1) * 4) + 2];
        word[3] = roundkey[((i - 1) * 4) + 3];
        sprintf(str_word, "%x,%x,%x,%x", word[0], word[1], word[2], word[3]);
        printf("%d:%s\n", i, str_word);

        // printf("%d,%d,%d,%d", &word[0], &word[1], &word[2], &word[3]);

        if (i % Nk == 0)
        {

            // rotating the word

            const uint8_t t = word[0];

            word[0] = word[1];
            word[1] = word[2];
            word[2] = word[3];
            word[3] = t;

            // sub bytes

            word[0] = sbox[word[0]];
            word[1] = sbox[word[1]];
            word[2] = sbox[word[2]];
            word[3] = sbox[word[3]];
            sprintf(str_subword, "%x,%x,%x,%x", word[0], word[1], word[2], word[3]);
            printf("%s\n", str_subword);

            // adding the round const

            word[0] = word[0] ^ Rcon[(i / Nk) - 1];
            sprintf(str_rcon1, "%x", Rcon[(i / Nk) - 1]);
            printf("%s\n", str_rcon1);

            sprintf(str_rcon, "%x,%x,%x,%x", word[0], word[1], word[2], word[3]);
            printf("%s\n", str_rcon);
        }

        int j = i * 4;        // index for accessing current round key positions
        int k = (i - Nk) * 4; // index to access the last round words

        // xoring the word with the word of the pevious round
        roundkey[j + 0] = word[0] ^ roundkey[k + 0];
        roundkey[j + 1] = word[1] ^ roundkey[k + 1];
        roundkey[j + 2] = word[2] ^ roundkey[k + 2];
        roundkey[j + 3] = word[3] ^ roundkey[k + 3];

        char *str = (char *)malloc(sizeof(uint8_t) * 4);
        sprintf(str, "%x,%x,%x,%x", roundkey[j + 0], roundkey[j + 1], roundkey[j + 2], roundkey[j + 3]);
        printf("%s \n", str);
    }
}

// Function for xtimes
uint8_t xtimes(uint8_t a)
{
    return ((uint16_t)a << 1) ^ (0x1b * (a >> 7 & 0x01));
}

// Function  for multiplication
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

// Function For mix columns
static void mixcolumn(uint8_t s[4][4])
{
    uint8_t a, b, c, d;
    for (int i = 0; i < 4; i++)
    {
        a = s[0][i];
        b = s[1][i];
        c = s[2][i];
        d = s[3][i];

        // Explicit defination of the mix column matrix multiplication
        s[0][i] = multi(0x02, a) ^ multi(0x03, b) ^ c ^ d;
        s[1][i] = a ^ multi(0x02, b) ^ multi(0x03, c) ^ d;
        s[2][i] = a ^ b ^ multi(0x02, c) ^ multi(0x03, d);
        s[3][i] = multi(0x03, a) ^ b ^ c ^ multi(0x02, d);
    }
    /* for (int i = 0; i < 4; i++)
     {
         printf("%X,%x,%x,%x\n", s[i][0], s[i][1], s[i][2], s[i][3]);
     }*/
}

// Function to add round key
static void Add_roundkey(uint8_t state[4][4], uint8_t roundkey[176], int round)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = state[j][i] ^ roundkey[(round * Nb * 4) + (i * Nb) + j];
        }
    }
}

// Function For the sub byte
static void Sub_Bytes(uint8_t state[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[i][j] = sbox[state[i][j]];
        }
    }
}

static void printstate(uint8_t state[4][4])
{
    for (int i = 0; i < 4; i++)
    {

        printf("%X,%x,%x,%x\n", state[i][0], state[i][1], state[i][2], state[i][3]);
    }
}

// Function For shiftrow

static void shiftrow(uint8_t state[4][4])
{
    uint8_t temp;

    // shift the row 1
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;

    // Shift row 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;

    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // shift row 3
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

static void cipher(uint8_t state[4][4], uint8_t roundkey[176])
{
    for (int round = 0; round <= Nr; round++)
    {
        printf("Round:%d\n", round);
        for (int i = 0; i < 4; i++)
        {

            printf("%x,%x,%x,%x", roundkey[(round * Nb * 4) + (i * Nb) + 0], roundkey[(round * Nb * 4) + (i * Nb) + 1], roundkey[(round * Nb * 4) + (i * Nb) + 2], roundkey[(round * Nb * 4) + (i * Nb) + 3]);
            printf("\n");
        }
    }
    printf("starting state:\n");
    printstate(state);
    Add_roundkey(state, roundkey, 0);
    printf("after first add round key:\n");
    printstate(state);
    for (int round = 1; round <= Nr - 1; round++)
    {
        printf("round:%d\n", round);
        Sub_Bytes(state);
        printf("after sub bytes:\n");
        printstate(state);

        shiftrow(state);
        printf("after shift column:\n");
        printstate(state);

        mixcolumn(state);
        printf("after mix column:\n");
        printstate(state);

        Add_roundkey(state, roundkey, round);
        printf("after add round key:\n");
        printstate(state);
    }

    Sub_Bytes(state);
    shiftrow(state);
    Add_roundkey(state, roundkey, Nr);
}

int main()
{

    key_scheudling(roundkey, key);
    /*for (int round = 0; round <= Nr; round++)
    {
        printf("Round:%d\n", round);
        for (int i = 0; i < 4; i++)
        {

            printf("%x,%x,%x,%x", roundkey[(round * Nb * 4) + (i * Nb) + 0], roundkey[(round * Nb * 4) + (i * Nb) + 1], roundkey[(round * Nb * 4) + (i * Nb) + 2], roundkey[(round * Nb * 4) + (i * Nb) + 3]);
            printf("\n");
        }
    }*/

    cipher(state, roundkey);
    printstate(state);
}