#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <search.h>
#include <inttypes.h>

#include "specks.h"

// Initialization vector
uint8_t IV[8];

// Generate a random IV
int iv_gen(size_t len)
{
    for (int i = 0; i < len; i++)
    {
        IV[i] = rand();
    }
}

size_t cbc_enc_s32_64(uint16_t key[4], uint8_t *pt, uint8_t *ct, size_t ptlen)
{
    iv_gen(4);
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

uint32_t attack_s32_64(uint8_t *ct, size_t ctlen)
{
    int len = ctlen / 4;
    hcreate(len);
    uint32_t *cblocks = (uint32_t *)ct;
    for (int i = 0; i < len; i++)
    {
        // Converting the key into a char array
        int len_tmp = snprintf(NULL, 0, "%" PRIu32, cblocks[i]);
        char *key = malloc(len_tmp + 1);
        snprintf(key, len_tmp + 1, "%" PRIu32, cblocks[i]);

        // Creating the hashtable item
        ENTRY item;
        item.key = key;
        ENTRY *found = hsearch(item, FIND);

        // Item not found, insert the index in the hashtable
        if (found == NULL)
        {
            int *data = malloc(sizeof(int));
            *data = i;
            item.data = data;
            hsearch(item, ENTER);
        }
        // Item found, i.e. Collision. Return the XOR of the appropriate CT blocks
        else
        {
            int j = *(int *)found->data;
            hdestroy();
            return cblocks[i - 1] ^ cblocks[j - 1];
        }
    }
    return UINT32_MAX;
}

size_t cbc_enc_s48_96(uint32_t key[4], uint8_t *pt, uint8_t *ct, size_t ptlen)
{
    iv_gen(6);
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

uint16_t *attack_s48_96(uint8_t *ct, size_t ctlen)
{
    int len = ctlen / 6;
    hcreate(len);
    uint16_t *cblocks = (uint16_t *)ct;
    uint16_t *leak = malloc(3 * sizeof(uint16_t));
    leak[0] = UINT16_MAX, leak[1] = UINT16_MAX, leak[2] = UINT16_MAX;
    for (int i = 0; i < len; i += 3)
    {
        // Converting the key into a char array
        int len_tmp = 0;
        for (int j = 0; j < 3; j++)
        {
            len_tmp += snprintf(NULL, 0, "%" PRIu16, cblocks[i + j]);
        }
        char *key = malloc(len_tmp + 1);
        snprintf(key, len_tmp + 1, "%" PRIu16 "%" PRIu16 "%" PRIu16, cblocks[i], cblocks[i + 1], cblocks[i + 2]);

        // Creating the hashtable item
        ENTRY item;
        item.key = key;
        ENTRY *found = hsearch(item, FIND);

        // Item not found, insert the index in the hashtable
        if (found == NULL)
        {
            int *data = malloc(sizeof(int));
            *data = i;
            item.data = data;
            hsearch(item, ENTER);
        }
        // Item found, i.e. Collision. Return the XOR of the appropriate CT blocks
        else
        {
            int j = *(int *)found->data;
            hdestroy();
            uint16_t *cii = cblocks + i - 3;
            uint16_t *cjj = cblocks + j - 3;
            leak[0] = cii[0] ^ cjj[0];
            leak[1] = cii[1] ^ cjj[1];
            leak[2] = cii[2] ^ cjj[2];
            break;
        }
    }
    return leak;
}

size_t cbc_enc_s64_128(uint32_t key[4], uint8_t *pt, uint8_t *ct, size_t ptlen)
{
    iv_gen(8);
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

uint32_t *attack_s64_128(uint8_t *ct, size_t ctlen)
{
    int len = ctlen / 8;
    hcreate(len);
    uint32_t *cblocks = (uint32_t *)ct;
    uint32_t *leak = malloc(2 * sizeof(uint32_t));
    leak[0] = UINT32_MAX, leak[1] = UINT32_MAX;
    for (int i = 2; i < len; i += 2)
    {
        // Converting the key into a char array
        int len_tmp = 0;
        for (int j = 0; j < 2; j++)
        {
            len_tmp += snprintf(NULL, 0, "%" PRIu32, cblocks[i + j]);
        }
        char *key = malloc(len_tmp + 1);
        snprintf(key, len_tmp + 1, "%" PRIu32 "%" PRIu32, cblocks[i], cblocks[i + 1]);

        // Creating the hashtable item
        ENTRY item;
        item.key = key;
        ENTRY *found = hsearch(item, FIND);

        // Item not found, insert the index in the hashtable
        if (found == NULL)
        {
            int *data = malloc(sizeof(int));
            *data = i;
            item.data = data;
            hsearch(item, ENTER);
        }
        // Item found, i.e. Collision. Return the XOR of the appropriate CT blocks
        else
        {
            int j = *(int *)found->data;
            hdestroy();
            uint32_t *cii = cblocks + i - 2;
            uint32_t *cjj = cblocks + j - 2;
            leak[0] = cii[0] ^ cjj[0];
            leak[1] = cii[1] ^ cjj[1];
            break;
        }
    }
    return leak;
}

int test_nondeterminism()
{
    int len = 24;

    uint16_t key[4];
    for (int i = 0; i < 4; i++)
    {
        key[i] = rand();
    }

    uint8_t *pt = malloc(len * sizeof(uint8_t));
    uint8_t **cts = malloc(len * sizeof(uint8_t *));
    for (int i = 0; i < len; i++)
    {
        pt[i] = rand();
    }

    printf("SPECK 32/64 Encryption Non-determinism Test:\n");
    printf("\tKey:\t");
    for (int i = 0; i < 4; i++)
    {
        printf("%" PRIu16 " ", key[i]);
    }
    printf("\n");
    printf("\tPT:\t");
    for (int i = 0; i < len; i++)
    {
        printf("%" PRIu8 " ", pt[i]);
    }
    printf("\n\n");

    for (int i = 0; i < 3; i++)
    {
        cts[i] = malloc(len * sizeof(uint8_t));
        uint8_t *ct = cts[i];
        uint8_t *pt_temp = malloc(len * sizeof(uint8_t));
        for (int i = 0; i < len; i++)
        {
            pt_temp[i] = pt[i];
        }

        cbc_enc_s32_64(key, pt_temp, ct, len);

        printf("\tCT %d:\t", i);
        for (int i = 0; i < len; i++)
        {
            printf("%" PRIu8 " ", ct[i]);
        }
        printf("\n");
        free(pt_temp);
    }
    bool determin = true;
    uint8_t *ct0 = cts[0];
    uint8_t *ct1 = cts[1];
    uint8_t *ct2 = cts[2];
    for (int i = 0; i < len; i++)
    {
        if (ct0[i] != ct1[i] || ct0[i] != ct2[i] || ct1[i] != ct2[i])
        {
            determin = false;
            break;
        }
    }
    if (determin)
    {
        printf("Test failed. The encryptiom appears to be deterministic.\n");
    }
    else
    {
        printf("Test successful. The encryption is non-deterministic.\n");
    }
    printf("\n");

    free(pt);
    free(ct0);
    free(ct1);
    free(ct2);
    free(cts);
    return 0;
}

int test_attack()
{
    printf("SPECK 32/64 Collision Attack Test:\n");
    // 2^18 blocks (Birthday bound is 2^16 blocks)
    int blocks = 1 << 18;
    size_t len = 4 * blocks;
    printf("\tPT/CT blocks: %d\n", blocks);
    uint16_t key0[4];
    uint8_t *pt0 = malloc(len * sizeof(uint8_t));
    uint8_t *ct0 = malloc(len * sizeof(uint8_t));
    for (int i = 0; i < 4; i++)
    {
        key0[i] = rand();
    }
    for (int i = 0; i < len; i++)
    {
        pt0[i] = rand();
    }
    printf("\tEncryption in progress...\n");
    cbc_enc_s32_64(key0, pt0, ct0, len);
    printf("\tEncryption Done.\n");
    printf("\tAttacking...\n");
    uint32_t xor0 = attack_s32_64(ct0, len);
    if (xor0 == UINT32_MAX)
    {
        printf("\tAttack failed! No collision found.\n\n");
    }
    else
    {
        printf("\tAttack succeeded! XOR of two plain-text blocks: %" PRIu32 "\n\n", xor0);
    }

    printf("SPECK 48/96 Collision Attack Test:\n");
    // 2^26 blocks (Birthday bound is 2^24 blocks)
    blocks = 1 << 26;
    len = 6 * blocks;
    printf("\tPT/CT blocks: %d\n", blocks);
    uint32_t key1[4];
    uint8_t *pt1 = malloc(len * sizeof(uint8_t));
    uint8_t *ct1 = malloc(len * sizeof(uint8_t));
    for (int i = 0; i < 4; i++)
    {
        key1[i] = rand();
    }
    for (int i = 0; i < len; i++)
    {
        pt1[i] = rand();
    }
    printf("\tEncryption in progress...\n");
    cbc_enc_s48_96(key1, pt1, ct1, len);
    printf("\tEncryption Done.\n");
    printf("\tAttacking...\n");
    uint16_t *xor1 = attack_s48_96(ct1, len);
    if (xor1[0] == UINT16_MAX && xor1[1] == UINT16_MAX && xor1[2] == UINT16_MAX)
    {
        printf("\tAttack failed! No collision found.\n\n");
    }
    else
    {
        printf("\tAttack succeeded! XOR of two plain-text blocks: %" PRIu16 "%" PRIu16 "%" PRIu16 "\n\n", xor1[0], xor1[1], xor1[2]);
    }
    free(pt0);
    free(ct0);
    free(pt1);
    free(ct1);
    free(xor1);
    return 0;
}

int test_enc_dec()
{
    printf("SPECK 32/64 Encryption-Decryption Test:\n");
    int len = 24;
    uint16_t key0[4];
    uint8_t *pt0 = malloc(len * sizeof(uint8_t));
    uint8_t *pt0_print = malloc(len * sizeof(uint8_t));
    uint8_t *ct0 = malloc(len * sizeof(uint8_t));

    for (int i = 0; i < len; i++)
    {
        pt0[i] = rand();
        pt0_print[i] = pt0[i];
    }

    cbc_enc_s32_64(key0, pt0, ct0, len);

    printf("\tIV:\t\t\t");
    for (int i = 0; i < 4; i++)
    {
        printf("%" PRIu8 " ", IV[i]);
    }
    printf("\n");

    printf("\tPT:\t\t\t");
    for (int i = 0; i < len; i++)
    {
        printf("%" PRIu8 " ", pt0_print[i]);
    }
    printf("\n");

    printf("\tCT (PT encrypted):\t");
    for (int i = 0; i < len; i++)
    {
        printf("%" PRIu8 " ", ct0[i]);
    }
    printf("\n");

    cbc_dec_s32_64(key0, ct0, pt0, len);

    printf("\tPT (CT decrypted):\t");
    for (int i = 0; i < len; i++)
    {
        printf("%" PRIu8 " ", pt0[i]);
    }
    printf("\n\n");

    printf("SPECK 48/96 Encryption-Decryption Test:\n");
    uint32_t key1[4];
    uint8_t *pt1 = malloc(len * sizeof(uint8_t));
    uint8_t *pt1_print = malloc(len * sizeof(uint8_t));
    uint8_t *ct1 = malloc(len * sizeof(uint8_t));

    for (int i = 0; i < len; i++)
    {
        pt1[i] = rand();
        pt1_print[i] = pt1[i];
    }

    cbc_enc_s48_96(key1, pt1, ct1, len);

    printf("\tIV:\t\t\t");
    for (int i = 0; i < 6; i++)
    {
        printf("%" PRIu8 " ", IV[i]);
    }
    printf("\n");

    printf("\tPT:\t\t\t");
    for (int i = 0; i < len; i++)
    {
        printf("%" PRIu8 " ", pt1_print[i]);
    }
    printf("\n");

    printf("\tCT (PT encrypted):\t");
    for (int i = 0; i < len; i++)
    {
        printf("%" PRIu8 " ", ct1[i]);
    }
    printf("\n");

    cbc_dec_s48_96(key1, ct1, pt1, len);

    printf("\tPT (CT decrypted):\t");
    for (int i = 0; i < len; i++)
    {
        printf("%" PRIu8 " ", pt1[i]);
    }
    printf("\n\n");

    printf("SPECK 64/128 Encryption-Decryption Test:\n");
    uint32_t key2[4];
    uint8_t *pt2 = malloc(len * sizeof(uint8_t));
    uint8_t *pt2_print = malloc(len * sizeof(uint8_t));
    uint8_t *ct2 = malloc(len * sizeof(uint8_t));

    for (int i = 0; i < len; i++)
    {
        pt2[i] = rand();
        pt2_print[i] = pt2[i];
    }

    cbc_enc_s64_128(key2, pt2, ct2, len);

    printf("\tIV:\t\t\t");
    for (int i = 0; i < 8; i++)
    {
        printf("%" PRIu8 " ", IV[i]);
    }
    printf("\n");

    printf("\tPT:\t\t\t");
    for (int i = 0; i < len; i++)
    {
        printf("%" PRIu8 " ", pt2_print[i]);
    }
    printf("\n");

    printf("\tCT (PT encrypted):\t");
    for (int i = 0; i < len; i++)
    {
        printf("%" PRIu8 " ", ct2[i]);
    }
    printf("\n");

    cbc_dec_s64_128(key2, ct2, pt2, len);

    printf("\tPT (CT decrypted):\t");
    for (int i = 0; i < len; i++)
    {
        printf("%" PRIu8 " ", pt2[i]);
    }
    printf("\n");
    free(pt0);
    free(pt0_print);
    free(ct0);
    free(pt1);
    free(pt1_print);
    free(ct1);
    free(pt2);
    free(pt2_print);
    free(ct2);
    return 0;
}

int main(void)
{
    // Make the random number generation truly pseudorandom
    srand((unsigned)time(NULL));

    // Test non-determinism
    test_nondeterminism();

    // Test collision attack
    test_attack();

    // Test ecnryption-decryption
    test_enc_dec();

    return 0;
}