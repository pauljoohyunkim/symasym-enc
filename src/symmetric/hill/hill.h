#ifndef HILL_H
#define HILL_H

#include <stdint.h>

#define HASHLEN 32

// Function declaration
void hill(uint8_t* key, uint8_t* buffer, int configuration_num);
void hill_keygen(uint8_t* seed, int seedlen, int n, uint8_t* keydest);

#endif