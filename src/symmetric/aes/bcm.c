#include <stdio.h>
#include <stdint.h>
#include "aes.h"
#include "bcm.h"

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