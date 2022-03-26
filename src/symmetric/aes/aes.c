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

// word3 = word1 xor word2
void addword(uint8_t* word1, uint8_t* word2, uint8_t* word3)
{
	word3[0] = word1[0] ^ word2[0];
	word3[1] = word1[1] ^ word2[1];
	word3[2] = word1[2] ^ word2[2];
	word3[3] = word1[3] ^ word2[3];
}

void add3word(uint8_t* word1, uint8_t* word2, uint8_t* word3, uint8_t* word4)
{
	word4[0] = word1[0] ^ word2[0] ^ word3[0];
	word4[1] = word1[1] ^ word2[1] ^ word3[1];
	word4[2] = word1[2] ^ word2[2] ^ word3[2];
	word4[3] = word1[3] ^ word2[3] ^ word3[3];
}

// Copies word1 to word2
void cpword(uint8_t* word1, uint8_t* word2)
{
	word2[0] = word1[0];
	word2[1] = word1[1];
	word2[2] = word1[2];
	word2[3] = word1[3];
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
void aes_key_schedule(int n, uint8_t* key, int r)
{
	uint8_t tempword[4];
	uint8_t rcon[4] = { 0 };

	for(int i = n; i < 4 * r; i++)
	{
		if(i % n == 0)
		{
			rcon[0] = rc[i / n - 1];
			cpword(key + 4*i - 4,tempword);
			rotword(tempword);
			subword(tempword);
			add3word(key + 4*i - 4*n, tempword, rcon, key + 4*i);
		}
		else if(i % n == 4)
		{
			cpword(key + 4*i - 4,tempword);
			subword(tempword);
			addword(key + 4*i - 4*n, tempword, key + 4*i);
		}
		else
		{
			addword(key + 4*i - 4*n, key + 4*i - 4, key + 4*i);
		}
	}
}

int main()
{
	uint8_t key[480] = { 0 };

	aes_key_schedule(4,key,11);

	return 0;
}