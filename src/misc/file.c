#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>


#include "file.h"

// File size by FILE*
uintmax_t sizeF(FILE* fp)
{
    struct stat st;
    fstat(fileno(fp), &st);
    return st.st_size;
}

// File size by name
uintmax_t sizeN(char* filename)
{
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}