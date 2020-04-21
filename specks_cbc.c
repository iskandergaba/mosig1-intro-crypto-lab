#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "specks.h"

uint8_t IV[8];

// Generate a random IV
void iv_gen(size_t len)
{
    for (int i = 0; i < len; i++)
    {
        IV[i] = rand();
    }
}

size_t cbc_enc_s32_64(uint16_t key[4], uint8_t *pt, uint8_t *ct, size_t ptlen)
{
    for (int i = 0; i < ptlen; i += 4)
    {
        // XOR the first pt block with a random IV
        if (i == 0)
        {
            for (int j = 0; j < 4; j++)
            {
                pt[j] ^= IV[j];
            }
        }
        // XOR the current pt block with the previous ct block
        else
        {
            for (int j = 0; j < 4; j++)
            {
                pt[i + j] ^= ct[i - 4 + j];
            }
        }
        speck32_64(key, pt + i, ct + i);
    }
    return 0;
}

size_t cbc_dec_s32_64(uint16_t key[4], uint8_t *ct, uint8_t *pt, size_t ctlen)
{
    for (int i = 0; i < ctlen; i += 4)
    {
        speck32_64_inv(key, ct + i, pt + i);
        // XOR the first pt block with IV
        if (i == 0)
        {
            for (int j = 0; j < 4; j++)
            {
                pt[j] ^= IV[j];
            }
        }
        // XOR the current pt block with the previous ct block
        else
        {
            for (int j = 0; j < 4; j++)
            {
                pt[i + j] ^= ct[i - 4 + j];
            }
        }
    }
    return 0;
}

size_t cbc_enc_s48_96(uint32_t key[4], uint8_t *pt, uint8_t *ct, size_t ptlen)
{
    for (int i = 0; i < ptlen; i += 6)
    {
        // XOR the first pt block with a random IV
        if (i == 0)
        {
            for (int j = 0; j < 6; j++)
            {
                pt[j] ^= IV[j];
            }
        }
        // XOR the current pt block with the previous ct block
        else
        {
            for (int j = 0; j < 6; j++)
            {
                pt[i + j] ^= ct[i - 6 + j];
            }
        }
        speck48_96(key, pt + i, ct + i);
    }
    return 0;
}

size_t cbc_dec_s48_96(uint32_t key[4], uint8_t *ct, uint8_t *pt, size_t ctlen)
{
    for (int i = 0; i < ctlen; i += 6)
    {
        speck48_96_inv(key, ct + i, pt + i);
        // XOR the first pt block with IV
        if (i == 0)
        {
            for (int j = 0; j < 6; j++)
            {
                pt[j] ^= IV[j];
            }
        }
        // XOR the current pt block with the previous ct block
        else
        {
            for (int j = 0; j < 6; j++)
            {
                pt[i + j] ^= ct[i - 6 + j];
            }
        }
    }
    return 0;
}

size_t cbc_enc_s64_128(uint32_t key[4], uint8_t *pt, uint8_t *ct, size_t ptlen)
{
    for (int i = 0; i < ptlen; i += 8)
    {
        // XOR the first pt block with a random IV
        if (i == 0)
        {
            for (int j = 0; j < 8; j++)
            {
                pt[j] ^= IV[j];
            }
        }
        // XOR the current pt block with the previous ct block
        else
        {
            for (int j = 0; j < 8; j++)
            {
                pt[i + j] ^= ct[i - 8 + j];
            }
        }
        speck64_128(key, pt + i, ct + i);
    }
    return 0;
}

size_t cbc_dec_s64_128(uint32_t key[4], uint8_t *ct, uint8_t *pt, size_t ctlen)
{
    for (int i = 0; i < ctlen; i += 8)
    {
        speck64_128_inv(key, ct + i, pt + i);
        // XOR the first pt block with IV
        if (i == 0)
        {
            for (int j = 0; j < 8; j++)
            {
                pt[j] ^= IV[j];
            }
        }
        // XOR the current pt block with the previous ct block
        else
        {
            for (int j = 0; j < 8; j++)
            {
                pt[i + j] ^= ct[i - 8 + j];
            }
        }
    }
    return 0;
}

void test_nondeterminism()
{
    int len = 24, iterations = 3;

    uint16_t key[4];
    for (int i = 0; i < 4; i++)
    {
        key[i] = rand();
    }

    uint8_t *pt = malloc(len * sizeof(uint8_t));
    uint8_t *ct = malloc(len * sizeof(uint8_t));
    for (int i = 0; i < len; i++)
    {
        pt[i] = rand();
    }

    printf("SPECK 32/64 Encryption Non-determinism Test:\n");
    printf("\tKey:\t");
    for (int i = 0; i < 4; i++)
    {
        printf("%d ", key[i]);
    }
    printf("\n");
    printf("\tPT:\t");
    for (int i = 0; i < len; i++)
    {
        printf("%d ", pt[i]);
    }
    printf("\n\n");

    for (int i = 0; i < iterations; i++)
    {
        iv_gen(4);
        printf("\tIV %d:\t", i);
        for (int i = 0; i < 4; i++)
        {
            printf("%d ", IV[i]);
        }
        printf("\n");

        uint8_t *pt_temp = malloc(len * sizeof(uint8_t));
        for (int i = 0; i < len; i++)
        {
            pt_temp[i] = pt[i];
        }

        cbc_enc_s32_64(key, pt_temp, ct, len);

        printf("\tCT %d:\t", i);
        for (int i = 0; i < len; i++)
        {
            printf("%d ", ct[i]);
        }
        printf("\n\n");
    }
}

void test_enc_dec()
{
    printf("SPECK 32/64 Encryption-Decryption Test:\n");
    int len = 24;
    uint16_t key0[4];
    uint8_t *pt0 = malloc(len * sizeof(uint8_t));
    uint8_t *ct0 = malloc(len * sizeof(uint8_t));

    iv_gen(4);
    printf("\tIV:\t\t\t");
    for (int i = 0; i < 4; i++)
    {
        printf("%d ", IV[i]);
    }
    printf("\n");

    for (int i = 0; i < len; i++)
    {
        pt0[i] = rand();
    }
    printf("\tPT:\t\t\t");
    for (int i = 0; i < len; i++)
    {
        printf("%d ", pt0[i]);
    }
    printf("\n");

    cbc_enc_s32_64(key0, pt0, ct0, len);

    printf("\tCT (PT encrypted):\t");
    for (int i = 0; i < len; i++)
    {
        printf("%d ", ct0[i]);
    }
    printf("\n");

    cbc_dec_s32_64(key0, ct0, pt0, len);

    printf("\tPT (CT decrypted):\t");
    for (int i = 0; i < len; i++)
    {
        printf("%d ", pt0[i]);
    }
    printf("\n\n");

    printf("SPECK 48/96 Encryption-Decryption Test:\n");
    uint32_t key1[4];
    uint8_t *pt1 = malloc(len * sizeof(uint8_t));
    uint8_t *ct1 = malloc(len * sizeof(uint8_t));

    iv_gen(6);
    printf("\tIV:\t\t\t");
    for (int i = 0; i < 6; i++)
    {
        printf("%d ", IV[i]);
    }
    printf("\n");

    for (int i = 0; i < len; i++)
    {
        pt1[i] = rand();
    }
    printf("\tPT:\t\t\t");
    for (int i = 0; i < len; i++)
    {
        printf("%d ", pt1[i]);
    }
    printf("\n");

    cbc_enc_s48_96(key1, pt1, ct1, len);

    printf("\tCT (PT encrypted):\t");
    for (int i = 0; i < len; i++)
    {
        printf("%d ", ct1[i]);
    }
    printf("\n");

    cbc_dec_s48_96(key1, ct1, pt1, len);

    printf("\tPT (CT decrypted):\t");
    for (int i = 0; i < len; i++)
    {
        printf("%d ", pt1[i]);
    }
    printf("\n\n");

    printf("SPECK 64/128 Encryption-Decryption Test:\n");
    uint32_t key2[4];
    uint8_t *pt2 = malloc(len * sizeof(uint8_t));
    uint8_t *ct2 = malloc(len * sizeof(uint8_t));

    iv_gen(8);
    printf("\tIV:\t\t\t");
    for (int i = 0; i < 8; i++)
    {
        printf("%d ", IV[i]);
    }
    printf("\n");

    for (int i = 0; i < len; i++)
    {
        pt2[i] = rand();
    }
    printf("\tPT:\t\t\t");
    for (int i = 0; i < len; i++)
    {
        printf("%d ", pt2[i]);
    }
    printf("\n");

    cbc_enc_s48_96(key2, pt2, ct2, len);

    printf("\tCT (PT encrypted):\t");
    for (int i = 0; i < len; i++)
    {
        printf("%d ", ct2[i]);
    }
    printf("\n");

    cbc_dec_s48_96(key2, ct2, pt2, len);

    printf("\tPT (CT decrypted):\t");
    for (int i = 0; i < len; i++)
    {
        printf("%d ", pt2[i]);
    }
    printf("\n");
}

int main(void)
{
    // Make the random number generation truly pseudorandom
    srand((unsigned)time(NULL));

    // Test non-determinism
    test_nondeterminism();

    // Test ecnryption-decryption
    test_enc_dec();

    return 0;
}