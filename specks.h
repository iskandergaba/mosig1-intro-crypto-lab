#ifndef SPECKS_H
#define SPECKS_H

#include <stdint.h>

/*** blocksize = 32; keysize = 64 ***/
// encrypts the 4-byte plaintext stored in p (left unchanged) to the ciphertext stored in c using the 64-bit key k
void speck32_64(const uint16_t k[4], const uint8_t p[4], uint8_t c[4])
// decrypts the 4-byte ciphertext stored in c (left unchanged) to the plaintext stored in p using the 64-bit key k
void speck32_64_inv(const uint16_t k[4], const uint8_t c[4], uint8_t p[4])

/*** blocksize = 48; keysize = 96 ***/
void speck48_96(const uint32_t k[4], const uint8_t p[6], uint8_t c[6])
void speck48_96_inv(const uint32_t k[4], const uint8_t c[6], uint8_t p[6])

/*** blocksize = 64; keysize = 128 ***/
void speck64_128(const uint32_t k[4], const uint8_t p[8], uint8_t c[8])
void speck64_128_inv(const uint32_t k[4], const uint8_t c[8], uint8_t p[8])

#endif // SPECKS_H
