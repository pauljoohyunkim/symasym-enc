#include <stdint.h>
#include "aes.h"





// Left circular shift of a word
void rotword(uint8_t* word)
{
	uint8_t temp = word[0];
	word[0] = word[1];
	word[1] = word[2];
	word[2] = word[3];
	word[3] = temp;
}

// AES S-box
void subword(uint8_t* word)
{
	word[0] = sbox[word[0]];
	word[1] = sbox[word[1]];
	word[2] = sbox[word[2]];
	word[3] = sbox[word[3]];
}

// word1 xor word2 -> word2
void addword(uint8_t* word1, uint8_t* word2)
{
	word2[0] ^= word1[0];
	word2[1] ^= word1[1];
	word2[2] ^= word1[2];
	word2[3] ^= word1[3];
}


// Key scheduling
/*
n = 4 for AES-128
n = 6 for AES-192
n = 8 for AES-256

k[0],...,k[n-1] are 32 bit words of the original key.
r = 11 for AES-128
r = 13 for AES-192
r = 15 for AES-256

w[0],...,w[4r-1] are 32 bit words of the expanded key.
*/
void aes_key_schedule(int n, uint8_t** k, int r, uint8_t** w)
{

}
