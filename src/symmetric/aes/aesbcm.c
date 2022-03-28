#include <stdio.h>
#include <stdint.h>
#include <time.h>
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
        aes(key,buffer,n,r,nRound);
        fwrite(buffer,1,read_bytes,outputFile);
    }
}

void inv_ecb_aes_enc(uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile)
{
    uint8_t buffer[16] = { 0 };
    
    int read_bytes = fread(buffer,1,16,inputFile);
    
    //Reading, applying AES, and writing
    while(read_bytes == 16)
    {
        invaes(key,buffer,n,r,nRound);
        fwrite(buffer,1,16,outputFile);
        read_bytes = fread(buffer,1,16,inputFile);
    }

    //Potential last block
    if(read_bytes != 0)
    {
        invaes(key,buffer,n,r,nRound);
        fwrite(buffer,1,read_bytes,outputFile);
    }
}

void cbc_aes_enc(uint8_t* iv, uint8_t* key, int n, int r, int nRound, FILE* inputFile, FILE* outputFile)
{
    uint8_t buffer[16] = { 0 };

    //Reading and xoring
    int read_bytes = fread(buffer,1,16,inputFile);

    while(read_bytes == 16)
    {
        for(int i = 0; i < 16; i++)
        {
            buffer[i] ^= iv[i];
        }
        aes(key,buffer, n, r, nRound);
        fwrite(buffer, 1, 16, outputFile);
        read_bytes = fread(buffer,1,16,inputFile);
    }
    
    if(read_bytes != 0)
    {
        for(int i = 0; i < 16; i++)
        {
            buffer[i] ^= iv[i];
        }
        aes(key,buffer,n,r,nRound);
        fwrite(buffer,1,read_bytes, outputFile);
    }
}

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
        fwrite(noncexorcounter,1,16,outputFile);
    }
}