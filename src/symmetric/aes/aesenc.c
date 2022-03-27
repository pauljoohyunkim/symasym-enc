#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include "aes.h"
#include "aesenc.h"

int main(int argc, char** argv)
{
	int opt;

	//AES Options
	/*
	n = 4 for AES-128
	n = 6 for AES-192
	n = 8 for AES-256

	r = 11 for AES-128
	r = 13 for AES-192
	r = 15 for AES-256

	nRound = 10,12,14 for AES128, AES192, AES256 resp.
	keylen = 16,24,32 for AES128, AES192, AES256 resp.
	*/
	int n;
	int r;
	int nRound;
	int keylen;

	// Options
	/*

	Mandatory options are starred.
	t: AES type* (1: AES128, 2: AES192, 3:AES256)
	i: Input*
	k: Key file*
	b: Block mode*
	o: Output file
	h: Help
	f: File integrity check skip
	*/

	char* inputFileName;
	char* inputKeyFileName;
	char* outputFileName;
	unsigned int bcm;		//Block cipher mode of operation. (ecb = 1, cbc = 2, ctr = 3)

	bool optT = false, optI = false, optK = false, optB = false, optO = false, optH = false, optF = false;
	while((opt = getopt(argc, argv, ":t:i:k:b:o:hf")) != -1)
	{
		switch(opt)
		{
			case 't':
				optT = true;
				switch(optarg[0])
				{
					case '1':
						printf("[INFO] AES-128 selected.\n");
						n = 4;
						r = 11;
						nRound = 10;
						keylen = 16;
						break;
					case '2':
						printf("[INFO] AES-192 selected.\n");
						n = 6;
						r = 13;
						nRound = 12;
						keylen = 24;
						break;
					case '3':
						printf("[INFO] AES-256 selected.\n");
						n = 8;
						r = 15;
						nRound = 14;
						keylen = 32;
						break;
					default:
						printf("[ERROR] Unknown AES mode.\n");
						return 1;
				}				
				break;
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
			case 'b':
				optB = true;
				//ecb, cbc, ctr,...
				if(strncmp(optarg,"ecb",3) == 0)
				{
					printf("[INFO] Block cipher mode of operation: ECB\n");
					bcm = 1;
				}
				else if(strncmp(optarg,"cbc", 3) == 0)
				{
					printf("[INFO] Block cipher mode of operation: CBC\n");
					bcm = 2;
				}
				else if(strncmp(optarg,"ctr", 3) == 0)
				{
					printf("[INFO] Block cipher mode of operation: CTR\n");
					bcm = 3;
				}
				else
				{
					printf("[ERROR] Unsupported block cipher mode of operation: %s\n", optarg);
					return 1;
				}
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
			case 'f':
				optF = true;
				printf("[INFO] File integrity hash will not be included.\n");
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
	if(!((optT && optI) && (optK && optB)))
	{
		printf("[ERROR] One of the required options is missing.\n");
		return 1;
	}

	// If -o not used, use default output file format: <input file>.aes
	if(!optO)
	{
		outputFileName = (char*) malloc((strlen(inputFileName) + 5) * sizeof(char));
		strcpy(outputFileName, inputFileName);
		strcat(outputFileName, ".aes");
		printf("[INFO] Output filename assigned: %s\n", outputFileName);
	}
	
	
	FILE* inputFile;
	FILE* keyFile;
	FILE* outputFile;
	unsigned int read_bytes = 0;		// Used for determining how many bytes read from fread

	uint8_t key[240] = { 0 };		// Zeroed out for padding.


	// Key reading
	if((keyFile = fopen(inputKeyFileName, "rb")) == NULL)
	{
		printf("[ERROR] Key file could not be read.\n");
		return 1;
	}
	read_bytes = fread(key, 1, keylen, keyFile);
	if(read_bytes < keylen)
	{
		printf("[INFO] Key file was shorter than the length required by the specified AES type, so it was padded with zeros.");
	}	
	fclose(keyFile);
	




	return 0;
}

void showHelp()
{
	char helpText[]
		= "\n"
		" usage: aesenc -t <1,2,3> -i <input file> -k <key file> -b <ecb, cbc, ctr> [options]"
		"\n"
		" Mandatory options:\n"
		"\n"
		"\t-t <1,2,3> : AES type (1: AES-128, 2: AES-192, 3: AES-256)\n"
		"\t-i <file>  : file to encrypt\n"
		"\t-k <file>  : key file. Depending on the type, only the first few bytes will be used.\n"
		"\t-b <mode>  : block cipher mode of operation; (ecb, cbc, ctr)\n"
		"\n"
		" Common options:\n"
		"\n"
		"\t-o <file>  : specify output file. (default: <input file>.aes)\n"
		"\t-f         : do not include file integrity hash.\n"
		"\n";
	printf("%s",helpText);
}
