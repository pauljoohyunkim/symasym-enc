#ifndef HASH_H
#define HASH_H

void sha256F(FILE* fp, int saltlen, uint8_t* salt, uint8_t* dest);

#endif