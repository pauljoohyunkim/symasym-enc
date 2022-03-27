#ifndef AESBCM_H
#define AESBCM_H

// Function declarations
void iv16byte(uint8_t* iv);
void ecb_aes_enc(uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile);

#endif