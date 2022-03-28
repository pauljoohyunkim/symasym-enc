#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include "../misc/hash.h"

#define MAXKEYLEN 1000
#define TMPBUFSIZ 512
#define HASHLEN 32

int main(int argc, char** argv)
{
    int opt;

    // Filenames
    char* inputFileName;
    char* inputKeyFileName;
    char* outputFileName;

    //Options
    /*
    Mandatory options are starred.
    i: Input File*
    k: Key File*
    o: Output File
    h: Help
    s: Skip password check (not relevant when password hash is not included.)
    f: Skip file integrity check (not relevant when file integrity hash is not included.)

    */
    bool optI = false, optK = false, optO = false, optH = false, optS = false, optF = false;
    while((opt = getopt(argc, argv, ":i:k:o:hsf")) != -1)
    {
        switch(opt)
        {
            case 'i':
                optI = true;
				inputFileName = (char*) malloc((strlen(optarg) + 1) * sizeof(char));
				strcpy(inputFileName,optarg);
				printf("[INFO] Input file: %s\n", inputFileName);
                break;
            
            case 'k':
				optK = true;
				inputKeyFileName = (char*) malloc((strlen(optarg) + 1) * sizeof(char));
				strcpy(inputKeyFileName, optarg);
				printf("[INFO] Input key file: %s\n", inputKeyFileName);
				break;
            
            case 'o':
				optO = true;
				outputFileName = (char*) malloc((strlen(optarg) + 1) * sizeof(char));
				strcpy(outputFileName, optarg);
				printf("[INFO] Output file: %s\n", outputFileName);
				break;
            
			case 'h':
				optH = true;
	
				printf("[INFO] Showing help...\n\n");
				showHelp();
				return 0;

			case 's':
				optS = true;
				printf("[INFO] Password check will be skipped.\n");
				break;
			
			case 'f':
				optF = true;
				printf("[INFO] File integrity will be skipped.\n");
				break;
			case ':':
				printf("[ERROR] An option needs a value.\n");
				return 1;
			case '?':
				printf("[ERROR] Unknown option: %c\n", optopt);
				break;

        }
    }

	// Checking mandatory fields
	if(!(optI && optK))
	{
		printf("[ERROR] One of the required options is missing.\n");
		return 1;
	}

	// If -o not used, use default output file format: <input file> minus the default extension
	if(!optO)
	{
		outputFileName = (char*) malloc((strlen(inputFileName) + 11) * sizeof(char));
		strcpy(outputFileName, inputFileName);
		strcat(outputFileName, ".decrypted");
		printf("[INFO] Output filename assigned: %s\n", outputFileName);
	}

	FILE* inputFile;
	FILE* keyFile;
	FILE* outputFile;
	unsigned int keylen = 0;

	uint8_t key[MAXKEYLEN] = { 0 };
	// Key reading
	if((keyFile = fopen(inputKeyFileName, "rb")) == NULL)
	{
		printf("[ERROR] Key file could not be read.\n");
		return 1;
	}
	keylen = fread(key, 1, MAXKEYLEN, keyFile);
	fclose(keyFile);
	free(inputKeyFileName);

	// Input file opening
	if((inputFile = fopen(inputFileName, "rb")) == NULL)
	{
		printf("[ERROR] Input file could not be opened.\n");
		return 1;
	}

	// Output file opening
	if((outputFile = fopen(outputFileName, "wb")) == NULL)
	{
		printf("[ERROR] Output file could not be opened.\n");
		
		fclose(inputFile);		// If it reached this point, currently the inputfile is open.
		return 1;
	}

	free(inputFileName);
	free(outputFileName);

	unsigned int read_bytes = 0;		//Used for checking how much byte read, temporarily.
	uint8_t header[4] = { 0 };					//Default 4-byte header
	uint8_t tempbuffer[TMPBUFSIZ];		//For holding onto miscellaneous configurations
	
	// Read header.
	read_bytes = fread(header,1,4,inputFile);
	if(read_bytes <= 4)
	{
		printf("[ERROR] %u bytes read. This is not even the length of the expected header!\n", read_bytes);
		fclose(inputFile);
		fclose(outputFile);

		return 1;
	}

	// README: First byte 0x00
	if(header[0] != 0)
	{
		printf("[ERROR] This is not a file generated by symmetric encryption.\n");
		fclose(inputFile);
		fclose(outputFile);

		return 1;
	}

	bool passhash_included = true;
	bool file_integrity_include = true;
	bool to_read_passhash = true;
	bool to_read_file_integrity = true;

	//SHA256 hashes
	uint8_t passhash_read[HASHLEN];
	uint8_t passhash_computed[HASHLEN];
	uint8_t file_integrity_read[HASHLEN];
	uint8_t file_integrity_computed[HASHLEN];
	uint8_t* iv;

	//c6,c7
	if(header[3] && 2 != 0)
	{
		passhash_included = false;
	}
	if(header[3] && 1 != 0)
	{
		file_integrity_include = false;
	}



	// More extensive case tree coming up...
	// For each option, tempbuffer holds information that are relevant.
	switch(header[1])		//README: A
	{
		case 0x00:		//AES: A = 0x00
			// tempbuffer = { n, r, nRound }
			iv = (uint8_t*) malloc(16);		// AES is given 16 byte IV
			fread(iv, 1, 16, inputFile);

			switch(header[2])		//README: B
			{
				case 0x00:		// AES-128
					tempbuffer[0] = 4;
					tempbuffer[1] = 11;
					tempbuffer[2] = 10;
					break;
				case 0x01:
					tempbuffer[0] = 6;
					tempbuffer[1] = 13;
					tempbuffer[2] = 12;
					break;
				case 0x02:
					tempbuffer[0] = 8;
					tempbuffer[1] = 15;
					tempbuffer[2] = 14;
					break;
				default:
					printf("[ERROR] Unknown AES type.\n");
					fclose(inputFile);
					fclose(outputFile);
					free(iv);
					return 1;
			}
			
			// Password Hash
			if(passhash_included)
			{
				fread(passhash_read, 1, HASHLEN, inputFile);
				if(!optS)
				{
					sha256A(4 * tempbuffer[0], key, 16, iv, passhash_computed);
					if(strcmp(passhash_included,passhash_computed) != 0)
					{
						//Wrong password
						printf("[ERROR] Incorrect key file.\n");
						fclose(inputFile);
						fclose(outputFile);
						free(iv);
					}
				}
				else
				{
					printf("[INFO] Key check is skipped.\n");
				}
			}

			else		// Skip password check regardless of -s option
			{
				optS = true;
				printf("[INFO] Password hash is not included, so password check will be skipped.\n");
			}

			switch((header[3] && 0b11111100) >> 2)		//README: C; bcm
			{
				case 0b00000:			//ECB

					break;
				case 0b00001:			//CBC
					break;
				case 0b00010:			//CTR
					break;
				default:
					printf("[ERROR] Unknown block cipher mode of operation.\n");
					fclose(inputFile);
					fclose(outputFile);
					free(iv);
					return 1;
				
			}


			break;




		case 0x01:		//Blowfish

			break;

		default:
			printf("[ERROR] Unknown symmetric encryption algorithm.\n");
			fclose(inputFile);
			fclose(outputFile);
			return 1;

	}

	free(iv);


	return 0;
}


void showHelp()
{
    char helpText[]
		= "\n"
		" usage: symdec -i <input file> -k <key file> [options]"
		"\n"
		" Mandatory options:\n"
		"\n"
		"\t-i <file>  : file to decrypt\n"
		"\t-k <file>  : key file. Depending on the type, only the first few bytes will be used.\n"
		"\n"
		" Common options:\n"
		"\n"
		"\t-o <file>  : specify output file. (default: <input file>.decrypted)\n"
		"\t-s         : skip password check.\n"
		"\t-f         : skip file integrity check at the end.\n"
		"\n";
	printf("%s",helpText);
}