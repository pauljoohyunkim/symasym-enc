#ifndef AES_H
#define AES_H


// Function declarations
void rotword(uint8_t* word);
void subword(uint8_t* word);
void addword(uint8_t* word1, uint8_t* word2, uint8_t* word3);
void add3word(uint8_t* word1, uint8_t* word2, uint8_t* word3, uint8_t* word4);
void cpword(uint8_t* word1, uint8_t* word2);
void aes_key_schedule(int n, uint8_t* key, int r);



#endif
