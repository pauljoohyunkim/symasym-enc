#include <stdio.h>
#include <openssl/sha.h>
#include <stdint.h>
#include "hash.h"

// Hashes an uint8_t array with given prepend salt.
// Requires dest to be larger than 32 bytes.
void sha256A(int bufferlen, uint8_t* buffer, int saltlen, uint8_t* salt, uint8_t* dest)
{
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    if(saltlen > 0)
    {
        SHA256_Update(&ctx,salt,saltlen);
    }
    if(bufferlen > 0)
    {
        SHA256_Update(&ctx,buffer,bufferlen);
    }
    SHA256_Final(dest, &ctx);
}

// Hashes a file with given prepend salt.
// Requires dest to be larger than 32 bytes.
void sha256F(FILE* fp, int saltlen, uint8_t* salt, uint8_t* dest)
{
    
    uint8_t buffer[BUFSIZ] = { 0 };

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, salt, saltlen);

    if(fp != NULL)
    {
        uintmax_t posoff = ftell(fp);        //Stores the current position.
        fseek(fp,0,SEEK_SET);

        int read_bytes = 0;

        do
        {
            read_bytes = fread(buffer,1,BUFSIZ,fp);
            SHA256_Update(&ctx, buffer,read_bytes);
        } while (read_bytes != 0);

        fseek(fp,posoff,SEEK_SET);  //Restores position
    }

    SHA256_Final(dest, &ctx);   //Copies hash to dest

    

}