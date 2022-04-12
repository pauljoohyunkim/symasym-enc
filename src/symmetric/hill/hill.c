#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "hill.h"
#include "../../misc/hash.h"
#include "../../misc/linalg.h"

// configuration_num = sqrt(keylen)
// Note that keylen should be a square number since keys are entries of a square matrix.
// ie) configuration_num = n where key matrix is an n*n matrix. This is also the length of the message.
void hill(uint8_t* key, uint8_t* buffer, int configuration_num)
{
    // n*n Key matrix generation from key of length configuration_num
    uint8_t** keymatrix;
    keymatrix = (uint8_t**) malloc(sizeof(uint8_t*) * configuration_num);
    for(int i = 0; i < configuration_num; i++)
    {
        keymatrix[i] = (uint8_t*) malloc(sizeof(uint8_t) * configuration_num);
        memcpy(keymatrix[i], key + configuration_num * i, configuration_num);
    }

    // Plaintext and ciphertext vector
    uint8_t** plaintext;
    uint8_t** ciphertext;
    plaintext = (uint8_t**) malloc(sizeof(uint8_t*) * configuration_num);
    ciphertext = (uint8_t**) malloc(sizeof(uint8_t*) * configuration_num);
    for(int i = 0; i < configuration_num; i++)
    {
        plaintext[i] = (uint8_t*) malloc(sizeof(uint8_t));
        ciphertext[i] = (uint8_t*) malloc(sizeof(uint8_t));
        plaintext[i][0] = buffer[i];
    }

    // Multiplication
    matmulmodn(keymatrix, plaintext, ciphertext, configuration_num, configuration_num, 1, 256);

    // Copy the result to buffer
    for(int i = 0; i < configuration_num; i++)
    {
        buffer[i] = ciphertext[i][0];
    }

    // Garbage collection
    for(int i = 0; i < configuration_num; i++)
    {
        free(keymatrix[i]);
        free(plaintext[i]);
        free(ciphertext[i]);
    }
    free(keymatrix);
    free(plaintext);
    free(ciphertext);


}

// Hill cipher needs an invertible matrix. Seed generates an invertible matrix.
void hill_keygen(uint8_t* seed, int seedlen, int n, uint8_t* keydest)
{
    uint8_t** tempmat;
    tempmat = (uint8_t**) calloc(sizeof(uint8_t*), n);
    for(int i = 0; i < n; i++)
    {
        tempmat[i] = (uint8_t*) calloc(sizeof(uint8_t), n);
    }

    uint8_t* tempkey;
    tempkey = (uint8_t*) calloc(sizeof(tempkey), n * n);
    
    



    // Garbage Collection
    free(tempkey);
    for(int i = 0; i < n; i++)
    {
        free(tempmat[i]);
    }
    free(tempmat);
}