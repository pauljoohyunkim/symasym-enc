#ifndef HASH_H
#define HASH_H

void sha256A(int bufferlen, uint8_t* buffer, int saltlen, uint8_t* salt, uint8_t* dest);
void sha256F(FILE* fp, int saltlen, uint8_t* salt, uint8_t* dest);

#endif