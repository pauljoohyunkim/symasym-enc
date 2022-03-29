#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "symkeygen.h"
#include "keygen.h"

/*
    This program works by taking the password from the user and generating SHA256 hashes.

    More specifically, given user input password (password), the key file generated would be:
    h101(password) h201(password) h301(password) ... (truncated to the length specified.)
    where hn(.) is applying SHA256 n times.
*/



int main(int argc, char** argv)
{
    int opt;
    unsigned int keylen = DEFAULT_KEYLEN;

    // l: Length, h: Help
    bool optL = false, optH = false;
    while((opt = getopt(argc, argv, ":l:h")) != -1)
    {
        switch(opt)
        {
            case 'l': ;
                char* strptr;
                keylen = strtol(optarg, &strptr, 10);
                break;

            case 'h':
                printf("[INFO] Showing help...\n\n");
                showHelp();
                return 0;
            case ':':
				printf("[ERROR] An option needs a value.\n");
				return 1;
			case '?':
				printf("[ERROR] Unknown option: %c\n", optopt);
				break;
        }
    }

    unsigned int keyFileNameLen = 0;
    char* keyFileName;


    if(optind < argc)       // key file argument is given.
    {
        printf("[INFO] Key file to be generated: %s\n",argv[optind]);
        keyFileNameLen = strlen(argv[optind]);
        keyFileName = (char*) calloc(keyFileNameLen + 2, sizeof(char));
        strcpy(keyFileName, argv[optind]);
    }
    else
    {
        printf("[INFO] Key file to be generated: keyfile\n");
        keyFileNameLen = strlen("keyfile");
        keyFileName = (char*) calloc(keyFileNameLen + 2, sizeof(char));
        strcpy(keyFileName, "keyfile");
    }

    // Keyfile opening
    FILE* keyFile = fopen(keyFileName, "wb");
    if(keyFile == NULL)
    {
        printf("[ERROR] Key file could not be opened for writing.\n");
        free(keyFileName);
        return 1;
    }
    
    int keygen_status = keygenF(keylen, keyFile);
    if(keygen_status == 1)      // Wrong password
    {
        fclose(keyFile);
        free(keyFileName);
        return 1;
    }

    printf("[INFO] %u byte key file %s generated.\n", keylen, keyFileName);
    return 0;
}

void showHelp()
{
    char helpText[]
		= "\n"
		" usage: symkeygen <key file>"
		"\n"
		" Common options:\n"
		"\n"
		"\t-l <length>: specify the number of bytes. (default: 32)\n"
		"\t-h         : show help.\n"
        "\n"
        " If <key file> argument is missing, it will be set to \"keyfile\" by default.\n"
		"\n";
	printf("%s",helpText);
}