#ifndef AESBCM_H
#define AESBCM_H

// Function declarations
void iv16byte(uint8_t* iv);
void counter_inc(uint8_t* counter);
void ecb_aes_enc(uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile);
void inv_ecb_aes_enc(uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile);
void cbc_aes_enc(uint8_t* iv, uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile);
void inv_cbc_aes_enc(uint8_t* iv, uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile);
void ctr_aes_enc(uint8_t* iv, uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile);

#endif