#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void ivbytes(uint8_t* iv, int length)
{
    srandom(time(NULL));
    for(int i = 0; i < length; i++)
    {
        iv[i] = random() % 256;
    }
}

void counter_inc(uint8_t* counter, int length)
{
    for(int index = length - 1; index >= 0; index--)
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

// encfun is the encryption function for one block
// decfun is the analogous decryption function for one block
// Format: encfun(uint8_t* key, uint8_t* buffer, int configuration_num) (decfun takes the same parameters)

// Electronic code book mode (NOT SECURE)
void ecb_enc(uint8_t* key, int blocklen, FILE* inputFile, FILE* outputFile, int configuration_num, void (*encfun)(uint8_t*, uint8_t*, int))
{
    uint8_t* buffer;
    uint8_t* clean_buffer;
    buffer = calloc(blocklen, 1);
    clean_buffer = calloc(blocklen, 1);         //For potential last block   
    
    int read_bytes = fread(buffer,1,16,inputFile);
    
    //Reading, applying encryption, and writing
    while(read_bytes == blocklen)
    {
        (*encfun)(key, buffer, configuration_num);
        // Old: aes(key,buffer,n,r,nRound);
        fwrite(buffer,1,blocklen,outputFile);
        read_bytes = fread(buffer,1,16,inputFile);
    }

    //Potential last block
    if(read_bytes != 0)
    {
        memcpy(clean_buffer, buffer, read_bytes);
        (*encfun)(key, clean_buffer, configuration_num);
        //aes(key,clean_buffer,n,r,nRound);
        fwrite(clean_buffer,1,blocklen,outputFile);
    }

    free(buffer);
    free(clean_buffer);
}

void ecb_dec(uint8_t* key, int blocklen, FILE* inputFile, FILE* outputFile, int configuration_num, void (*decfun)(uint8_t*, uint8_t*, int))
{
    uint8_t* buffer;
    buffer = calloc(blocklen, 1);
    
    int read_bytes = fread(buffer,1,16,inputFile);

    //Reading, applying encryption, and writing
    while(read_bytes == 16)
    {
        (*decfun)(key, buffer, configuration_num);
        fwrite(buffer,1,16,outputFile);
        read_bytes = fread(buffer,1,16,inputFile);
    }

    free(buffer);
}


// Cipher Block Chaining mode
void cbc_enc(uint8_t* iv, uint8_t* key, int blocklen, FILE* inputFile, FILE* outputFile, int configuration_num, void (*encfun)(uint8_t*, uint8_t*, int))
{
    uint8_t* buffer;
    uint8_t* clean_buffer;
    uint8_t* chain;
    buffer = calloc(blocklen, 1);
    clean_buffer = calloc(blocklen, 1);         //For potential last block
    chain = calloc(blocklen, 1);

    /*
    uint8_t buffer[16] = { 0 };
    uint8_t clean_buffer[16] = { 0 };       
    uint8_t chain[16] = { 0 };
    */

    memcpy(chain, iv, blocklen);

    //Reading and xoring
    int read_bytes = fread(buffer,1,blocklen,inputFile);
    while(read_bytes == blocklen)
    {
        for(int i = 0; i < blocklen; i++)
        {
            buffer[i] ^= chain[i];
        }
        (*encfun)(key, clean_buffer, configuration_num);
        //aes(key,buffer, n, r, nRound);
        fwrite(buffer, 1, blocklen, outputFile);
        memcpy(chain, buffer, blocklen);
        read_bytes = fread(buffer,1,blocklen,inputFile);
    }
    

    if(read_bytes != 0)
    {
        memcpy(clean_buffer, buffer, read_bytes);
        for(int i = 0; i < blocklen; i++)
        {
            clean_buffer[i] ^= chain[i];
        }
        (*encfun)(key, clean_buffer, configuration_num);
        fwrite(clean_buffer,1,blocklen, outputFile);
    }

    free(buffer);
    free(clean_buffer);
    free(chain);
}

// Inverse CBC
void cbc_dec(uint8_t* iv, uint8_t* key, int blocklen, FILE* inputFile, FILE* outputFile, int configuration_num, void (*decfun)(uint8_t*, uint8_t*, int))
{
    uint8_t* buffer;
    uint8_t* chain1;
    uint8_t* chain2;
    buffer = calloc(blocklen, 1);
    chain1 = calloc(blocklen, 1);         //For potential last block
    chain2 = calloc(blocklen, 1);

    /*
    uint8_t buffer[16] = { 0 };
    uint8_t chain1[16] = { 0 };
    uint8_t chain2[16] = { 0 };
    */


    //Reading and xoring
    int read_bytes = fread(buffer,1,blocklen,inputFile);
    memcpy(chain2, buffer, blocklen);

    (*decfun)(key, buffer, configuration_num);
    //invaes(key, buffer, n, r, nRound);
    for(int i = 0; i < blocklen; i++)
    {
        buffer[i] ^= iv[i];
    }
    fwrite(buffer, 1, read_bytes, outputFile);

    read_bytes = fread(buffer, 1, blocklen, inputFile);
    while(read_bytes == blocklen)
    {
        memcpy(chain1, buffer, blocklen);
        (*decfun)(key, buffer, configuration_num);
        for(int i = 0; i < blocklen; i++)
        {
            buffer[i] ^= chain2[i];
        }
        fwrite(buffer, 1, blocklen, outputFile);
        memcpy(chain2, chain1, blocklen);
        read_bytes = fread(buffer, 1, blocklen, inputFile);
    }

    free(buffer);
    free(chain1);
    free(chain2);
}

// Counter Mode (This is equal to its inverse)
void ctr_enc(uint8_t* iv, uint8_t* key, int blocklen, FILE* inputFile, FILE* outputFile, int configuration_num, void (*encfun)(uint8_t*, uint8_t*, int))
{
    uint8_t* counter;
    uint8_t* noncexorcounter;
    uint8_t* buffer;

    counter = calloc(blocklen, 1);
    noncexorcounter = calloc(blocklen, 1);      //nonce xor counter
    buffer = calloc(blocklen, 1);

    //Initially read bytes and hold. AES the nonce first.
    int read_bytes = fread(buffer,1,blocklen,inputFile);

    while(read_bytes == blocklen)
    {
        for(int i = 0; i < blocklen; i++)
        {
            noncexorcounter[i] = iv[i] ^ counter[i];
        }
        (*encfun)(key, buffer, configuration_num);
        for(int i = 0; i < blocklen; i++)
        {
            noncexorcounter[i] ^= buffer[i];
        }
        fwrite(noncexorcounter,1,blocklen,outputFile);
        counter_inc(counter, blocklen);
        read_bytes = fread(buffer,1,blocklen,inputFile);
    }

    //Potential last block
    if(read_bytes != 0)
    {
        for(int i = 0; i < blocklen; i++)
        {
            noncexorcounter[i] = iv[i] ^ counter[i];
        }
        (*encfun)(key, buffer, configuration_num);
        for(int i = 0; i < blocklen; i++)
        {
            noncexorcounter[i] ^= buffer[i];
        }
        fwrite(noncexorcounter,1,read_bytes,outputFile);
    }

    free(counter);
    free(noncexorcounter);
    free(buffer);
}