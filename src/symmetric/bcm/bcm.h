#ifndef BCM_H
#define BCM_H

#include <stdio.h>
#include <stdint.h>

// Function Declaration
void ivbytes(uint8_t* iv, int length);
void counter_inc(uint8_t* counter, int length);
void ecb_enc(uint8_t* key, int blocklen, FILE* inputFile, FILE* outputFile, int configuration_num, void (*encfun)(uint8_t*, uint8_t*, int));
void ecb_dec(uint8_t* key, int blocklen, FILE* inputFile, FILE* outputFile, int configuration_num, void (*decfun)(uint8_t*, uint8_t*, int));
void cbc_enc(uint8_t* iv, uint8_t* key, int blocklen, FILE* inputFile, FILE* outputFile, int configuration_num, void (*encfun)(uint8_t*, uint8_t*, int));
void cbc_dec(uint8_t* iv, uint8_t* key, int blocklen, FILE* inputFile, FILE* outputFile, int configuration_num, void (*decfun)(uint8_t*, uint8_t*, int));
void ctr_enc(uint8_t* iv, uint8_t* key, int blocklen, FILE* inputFile, FILE* outputFile, int configuration_num, void (*encfun)(uint8_t*, uint8_t*, int));

#endif