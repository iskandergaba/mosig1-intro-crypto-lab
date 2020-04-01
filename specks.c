#include "specks.h"

#define ROTL16_9(x) ((((x) << 9) ^ ((x) >> 7)) & 0xFFFF)
#define ROTL16_2(x) ((((x) << 2) ^ ((x) >> 14)) & 0xFFFF)

#define ROTL16_7(x) ((((x) << 7) ^ ((x) >> 9)) & 0xFFFF)
#define ROTL16_14(x) ((((x) << 14) ^ ((x) >> 2)) & 0xFFFF)

#define ROTL24_16(x) ((((x) << 16) ^ ((x) >> 8)) & 0xFFFFFF)
#define ROTL24_3(x) ((((x) << 3) ^ ((x) >> 21)) & 0xFFFFFF)

#define ROTL24_8(x) ((((x) << 8) ^ ((x) >> 16)) & 0xFFFFFF)
#define ROTL24_21(x) ((((x) << 21) ^ ((x) >> 3)) & 0xFFFFFF)

#define ROTL32_24(x) ((((x) << 24) ^ ((x) >> 8)) & 0xFFFFFFFF)
#define ROTL32_3(x) ((((x) << 3) ^ ((x) >> 29)) & 0xFFFFFFFF)

#define ROTL32_8(x) ((((x) << 8) ^ ((x) >> 24)) & 0xFFFFFFFF)
#define ROTL32_29(x) ((((x) << 29) ^ ((x) >> 3)) & 0xFFFFFFFF)

/*** 32/64 ***/

void speck32_64(const uint16_t k[4], const uint8_t p[4], uint8_t c[4])
{
	uint16_t rk[22];
	uint16_t ell[3] = {k[1], k[2], k[3]};

	rk[0] = k[0];

    uint16_t ctmp[2];

	ctmp[0] = p[0] ^ ((uint16_t)p[1] << 8);
	ctmp[1] = p[2] ^ ((uint16_t)p[3] << 8);

	for (unsigned i = 0; i < 21; i++)
	{
		uint32_t new_ell = ((ROTL16_9(ell[0]) + rk[i]) ^ i) & 0xFFFF;
		rk[i+1] = ROTL16_2(rk[i]) ^ new_ell;
		ell[0] = ell[1];
		ell[1] = ell[2];
		ell[2] = new_ell;
	}

	for (unsigned i = 0; i < 22; i++)
	{
		ctmp[0] = ((ROTL16_9(ctmp[0]) + ctmp[1]) ^ rk[i]) & 0xFFFF;
		ctmp[1] = ROTL16_2(ctmp[1]) ^ ctmp[0];
	}

    c[0] = ctmp[0] & 0xFF;
    c[1] = (ctmp[0] >> 8) & 0xFF;
    c[2] = ctmp[1] & 0xFF;
    c[3] = (ctmp[1] >> 8) & 0xFF;

	return;
}

void speck32_64_inv(const uint16_t k[4], const uint8_t c[4], uint8_t p[4])
{
	uint16_t rk[23];
	uint16_t ell[3] = {k[1], k[2], k[3]};

	rk[0] = k[0];

    uint16_t ptmp[2];

	ptmp[0] = c[0] ^ ((uint16_t)c[1] << 8);
	ptmp[1] = c[2] ^ ((uint16_t)c[3] << 8);

	for (unsigned i = 0; i < 21; i++)
	{
		uint16_t new_ell = ((ROTL16_9(ell[0]) + rk[i]) ^ i) & 0xFFFF;
		rk[i+1] = ROTL16_2(rk[i]) ^ new_ell;
		ell[0] = ell[1];
		ell[1] = ell[2];
		ell[2] = new_ell;
	}

	for (int i = 21; i >= 0; i--)
	{
		ptmp[1] = ROTL16_14(ptmp[1] ^ ptmp[0]);
		ptmp[0] = ROTL16_7(((ptmp[0] ^ rk[i]) - ptmp[1]) & 0xFFFF);
	}

    p[0] = ptmp[0] & 0xFF;
    p[1] = (ptmp[0] >> 8) & 0xFF;
    p[2] = ptmp[1] & 0xFF;
    p[3] = (ptmp[1] >> 8) & 0xFF;

	return;
}

/*** 48/96 ***/

void speck48_96(const uint32_t k[4], const uint8_t p[6], uint8_t c[6])
{
	uint32_t rk[23];
	uint32_t ell[3] = {k[1], k[2], k[3]};

	rk[0] = k[0];

    uint32_t ctmp[2];

	ctmp[0] = p[0] ^ ((uint32_t)p[1] << 8) ^ ((uint32_t)p[2] << 16);
	ctmp[1] = p[3] ^ ((uint32_t)p[4] << 8) ^ ((uint32_t)p[5] << 16);

	for (unsigned i = 0; i < 22; i++)
	{
		uint32_t new_ell = ((ROTL24_16(ell[0]) + rk[i]) ^ i) & 0xFFFFFF;
		rk[i+1] = ROTL24_3(rk[i]) ^ new_ell;
		ell[0] = ell[1];
		ell[1] = ell[2];
		ell[2] = new_ell;
	}

	for (unsigned i = 0; i < 23; i++)
	{
		ctmp[0] = ((ROTL24_16(ctmp[0]) + ctmp[1]) ^ rk[i]) & 0xFFFFFF;
		ctmp[1] = ROTL24_3(ctmp[1]) ^ ctmp[0];
	}

    c[0] = ctmp[0] & 0xFF;
    c[1] = (ctmp[0] >> 8) & 0xFF;
    c[2] = (ctmp[0] >>16) & 0xFF;
    c[3] = ctmp[1] & 0xFF;
    c[4] = (ctmp[1] >> 8) & 0xFF;
    c[5] = (ctmp[1] >>16) & 0xFF;

	return;
}

void speck48_96_inv(const uint32_t k[4], const uint8_t c[6], uint8_t p[6])
{
	uint32_t rk[23];
	uint32_t ell[3] = {k[1], k[2], k[3]};

	rk[0] = k[0];

    uint32_t ptmp[2];

	ptmp[0] = c[0] ^ ((uint32_t)c[1] << 8) ^ ((uint32_t)c[2] << 16);
	ptmp[1] = c[3] ^ ((uint32_t)c[4] << 8) ^ ((uint32_t)c[5] << 16);

	for (unsigned i = 0; i < 22; i++)
	{
		uint32_t new_ell = ((ROTL24_16(ell[0]) + rk[i]) ^ i) & 0xFFFFFF;
		rk[i+1] = ROTL24_3(rk[i]) ^ new_ell;
		ell[0] = ell[1];
		ell[1] = ell[2];
		ell[2] = new_ell;
	}

	for (int i = 22; i >= 0; i--)
	{
		ptmp[1] = ROTL24_21(ptmp[1] ^ ptmp[0]);
		ptmp[0] = ROTL24_8(((ptmp[0] ^ rk[i]) - ptmp[1]) & 0xFFFFFF);
	}

    p[0] = ptmp[0] & 0xFF;
    p[1] = (ptmp[0] >> 8) & 0xFF;
    p[2] = (ptmp[0] >>16) & 0xFF;
    p[3] = ptmp[1] & 0xFF;
    p[4] = (ptmp[1] >> 8) & 0xFF;
    p[5] = (ptmp[1] >>16) & 0xFF;

	return;
}

/*** 64/128 ***/

void speck64_128(const uint32_t k[4], const uint8_t p[8], uint8_t c[8])
{
	uint32_t rk[27];
	uint32_t ell[3] = {k[1], k[2], k[3]};

	rk[0] = k[0];

    uint32_t ctmp[2];

	ctmp[0] = p[0] ^ ((uint32_t)p[1] << 8) ^ ((uint32_t)p[2] << 16) ^ ((uint32_t)p[3] << 24);
	ctmp[1] = p[4] ^ ((uint32_t)p[5] << 8) ^ ((uint32_t)p[6] << 16) ^ ((uint32_t)p[7] << 24);

	for (unsigned i = 0; i < 26; i++)
	{
		uint32_t new_ell = ((ROTL32_24(ell[0]) + rk[i]) ^ i) & 0xFFFFFFFF;
		rk[i+1] = ROTL32_3(rk[i]) ^ new_ell;
		ell[0] = ell[1];
		ell[1] = ell[2];
		ell[2] = new_ell;
	}

	for (unsigned i = 0; i < 27; i++)
	{
		ctmp[0] = ((ROTL32_24(ctmp[0]) + ctmp[1]) ^ rk[i]) & 0xFFFFFFFF;
		ctmp[1] = ROTL32_3(ctmp[1]) ^ ctmp[0];
	}

    c[0] = ctmp[0] & 0xFF;
    c[1] = (ctmp[0] >> 8) & 0xFF;
    c[2] = (ctmp[0] >>16) & 0xFF;
    c[3] = (ctmp[0] >>24) & 0xFF;
    c[4] = ctmp[1] & 0xFF;
    c[5] = (ctmp[1] >> 8) & 0xFF;
    c[6] = (ctmp[1] >>16) & 0xFF;
    c[7] = (ctmp[1] >>24) & 0xFF;

	return;
}

void speck64_128_inv(const uint32_t k[4], const uint8_t c[8], uint8_t p[8])
{
	uint32_t rk[27];
	uint32_t ell[3] = {k[1], k[2], k[3]};

	rk[0] = k[0];

    uint32_t ptmp[2];

	ptmp[0] = c[0] ^ ((uint32_t)c[1] << 8) ^ ((uint32_t)c[2] << 16) ^ ((uint32_t)c[3] << 24);
	ptmp[1] = c[4] ^ ((uint32_t)c[5] << 8) ^ ((uint32_t)c[6] << 16) ^ ((uint32_t)c[7] << 24);

	for (unsigned i = 0; i < 26; i++)
	{
		uint32_t new_ell = ((ROTL32_24(ell[0]) + rk[i]) ^ i) & 0xFFFFFFFF;
		rk[i+1] = ROTL32_3(rk[i]) ^ new_ell;
		ell[0] = ell[1];
		ell[1] = ell[2];
		ell[2] = new_ell;
	}

	for (int i = 26; i >= 0; i--)
	{
		ptmp[1] = ROTL32_29(ptmp[1] ^ ptmp[0]);
		ptmp[0] = ROTL32_8(((ptmp[0] ^ rk[i]) - ptmp[1]) & 0xFFFFFFFF);
	}

    p[0] = ptmp[0] & 0xFF;
    p[1] = (ptmp[0] >> 8) & 0xFF;
    p[2] = (ptmp[0] >>16) & 0xFF;
    p[3] = (ptmp[0] >>24) & 0xFF;
    p[4] = ptmp[1] & 0xFF;
    p[5] = (ptmp[1] >> 8) & 0xFF;
    p[6] = (ptmp[1] >>16) & 0xFF;
    p[7] = (ptmp[1] >>24) & 0xFF;

	return;
}
