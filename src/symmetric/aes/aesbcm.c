#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "aes.h"
#include "aesbcm.h"

void iv16byte(uint8_t* iv)
{
    srandom(time(NULL));
    for(int i = 0; i < 16; i++)
    {
        iv[i] = random() % 256;
    }
}

void counter_inc(uint8_t* counter)
{
    for(int index = 15; index >= 0; index--)
    {
        if(counter[index] != 255)
        {
            counter[index]++;
            break;
        }
        else
        {
            counter[index] = 0;
        }
    }
}

// Electronic code book mode (NOT SECURE)
void ecb_aes_enc(uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile)
{
    uint8_t buffer[16] = { 0 };
    uint8_t clean_buffer[16] = { 0 };       //For potential last block
    
    int read_bytes = fread(buffer,1,16,inputFile);
    
    //Reading, applying AES, and writing
    while(read_bytes == 16)
    {
        aes(key,buffer,n,r,nRound);
        fwrite(buffer,1,16,outputFile);
        read_bytes = fread(buffer,1,16,inputFile);
    }

    //Potential last block
    if(read_bytes != 0)
    {
        memcpy(clean_buffer, buffer, read_bytes);
        aes(key,clean_buffer,n,r,nRound);
        fwrite(clean_buffer,1,16,outputFile);
    }
}

void inv_ecb_aes_enc(uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile)
{
    uint8_t buffer[16] = { 0 };
    uint8_t clean_buffer[16] = { 0 };       //For potential last block
    
    int read_bytes = fread(buffer,1,16,inputFile);

    //Reading, applying AES, and writing
    while(read_bytes == 16)
    {
        invaes(key,buffer,n,r,nRound);
        fwrite(buffer,1,16,outputFile);
        read_bytes = fread(buffer,1,16,inputFile);
    }
}

// Cipher Block Chaining mode
void cbc_aes_enc(uint8_t* iv, uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile)
{
    uint8_t buffer[16] = { 0 };
    uint8_t clean_buffer[16] = { 0 };       //For potential last block
    uint8_t chain[16] = { 0 };

    memcpy(chain, iv, 16);

    //Reading and xoring
    int read_bytes = fread(buffer,1,16,inputFile);
    while(read_bytes == 16)
    {
        for(int i = 0; i < 16; i++)
        {
            buffer[i] ^= chain[i];
        }
        aes(key,buffer, n, r, nRound);
        fwrite(buffer, 1, 16, outputFile);
        memcpy(chain, buffer, 16);
        read_bytes = fread(buffer,1,16,inputFile);
    }
    

    if(read_bytes != 0)
    {
        memcpy(clean_buffer, buffer, read_bytes);
        for(int i = 0; i < 16; i++)
        {
            clean_buffer[i] ^= chain[i];
        }
        aes(key,clean_buffer,n,r,nRound);
        fwrite(clean_buffer,1,16, outputFile);
    }
}

// Inverse CBC
void inv_cbc_aes_enc(uint8_t* iv, uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile)
{
    uint8_t buffer[16] = { 0 };
    uint8_t clean_buffer[16] = { 0 };       //For potential last block
    uint8_t chain1[16] = { 0 };
    uint8_t chain2[16] = { 0 };

    //Reading and xoring
    int read_bytes = fread(buffer,1,16,inputFile);
    memcpy(chain2, buffer, 16);
    invaes(key, buffer, n, r, nRound);
    for(int i = 0; i < 16; i++)
    {
        buffer[i] ^= iv[i];
    }
    fwrite(buffer, 1, read_bytes, outputFile);

    read_bytes = fread(buffer, 1, 16, inputFile);
    while(read_bytes == 16)
    {
        memcpy(chain1, buffer, 16);
        invaes(key, buffer, n, r, nRound);
        for(int i = 0; i < 16; i++)
        {
            buffer[i] ^= chain2[i];
        }
        fwrite(buffer, 1, 16, outputFile);
        memcpy(chain2, chain1, 16);
        read_bytes = fread(buffer, 1, 16, inputFile);
    }

    /*
    if(read_bytes != 16)
    {
        strncpy(clean_buffer, buffer, read_bytes);
        invaes(key, clean_buffer, n, r, nRound);
        for(int i = 0; i < 16; i++)
        {
            clean_buffer[i] ^= chain2[i];
        }
        fwrite(clean_buffer, 1, read_bytes, outputFile);

    }

    */
}

// Counter Mode (This is equal to its inverse)
void ctr_aes_enc(uint8_t* iv, uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile)
{
    uint8_t counter[16] = { 0 };
    uint8_t noncexorcounter[16] = { 0 };        //nonce xor counter
    uint8_t buffer[16] = { 0 };

    //Initially read bytes and hold. AES the nonce first.
    int read_bytes = fread(buffer,1,16,inputFile);

    while(read_bytes == 16)
    {
        for(int i = 0; i < 16; i++)
        {
            noncexorcounter[i] = iv[i] ^ counter[i];
        }
        aes(key, noncexorcounter, n, r, nRound);
        for(int i = 0; i < 16; i++)
        {
            noncexorcounter[i] ^= buffer[i];
        }
        fwrite(noncexorcounter,1,16,outputFile);
        counter_inc(counter);
        read_bytes = fread(buffer,1,16,inputFile);
    }

    //Potential last block
    if(read_bytes != 0)
    {
        for(int i = 0; i < 16; i++)
        {
            noncexorcounter[i] = iv[i] ^ counter[i];
        }
        aes(key, noncexorcounter, n, r, nRound);
        for(int i = 0; i < 16; i++)
        {
            noncexorcounter[i] ^= buffer[i];
        }
        fwrite(noncexorcounter,1,read_bytes,outputFile);
    }
}