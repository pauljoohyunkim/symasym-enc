#ifndef AES_H
#define AES_H


// Function declarations
void rotword(uint8_t* word);
void subword(uint8_t* word);
void addword(uint8_t* word1, uint8_t* word2, uint8_t* word3);
void add3word(uint8_t* word1, uint8_t* word2, uint8_t* word3, uint8_t* word4);
void cpword(uint8_t* word1, uint8_t* word2);
void aes_key_schedule(int n, uint8_t* key, int r);
void add16bytes(uint8_t* b1, uint8_t* b2, uint8_t* b3);
void sub16bytes(uint8_t* b);
void isub16bytes(uint8_t* b);
void permutation(uint8_t* buffer);
void ipermutation(uint8_t* buffer);
void mult(uint8_t* buffer);
void aes(uint8_t* key, uint8_t* buffer, int n, int r, int nRound);
void invaes(uint8_t* key, uint8_t* buffer, int n, int r, int nRound);


#endif
