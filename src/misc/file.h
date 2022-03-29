#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#include <stdio.h>
// Function declaration
uintmax_t sizeF(FILE* fp);
uintmax_t sizeN(char* filename);

#endif