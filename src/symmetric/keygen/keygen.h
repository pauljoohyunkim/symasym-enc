#ifndef KEYGEN_H
#define KEYGEN_H

#define SIZE 1000
#define HASHLEN 32          //sha256 hash length
#define HASHREP 100         //Hash repetition

#include <stdint.h>
#include <stdbool.h>

int keygen(unsigned int keylen, FILE* fp, uint8_t* passwordbuffer, bool checkTwice);
void getPassword(char *password);

#endif