#ifndef KEYGEN_H
#define KEYGEN_H

#define SIZE 1000
#define HASHLEN 32          //sha256 hash length
#define HASHREP 100         //Hash repetition

int keygen(unsigned int keylen, FILE* fp);
void getPassword(char *password);

#endif