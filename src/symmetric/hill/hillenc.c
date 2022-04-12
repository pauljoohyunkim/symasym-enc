#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "hillenc.h"
#include "hill.h"

int main(int argc, char** argv)
{
	int opt;
    
	int configuration_num;      //configuration_num = sqrt(keylen)
	// Options
	/*

	Mandatory options are starred.
	i: Input*
	k: Key file*
	p: Enter password instead of inputting a key file.* (Mutually exclusive with -k option.)
	b: Block mode*
	o: Output file
	h: Help
	s: Password check skip
	f: File integrity check skip
	*/

	char* inputFileName;
	char* inputKeyFileName;
	char* outputFileName;
	uint8_t bcm;		//Block cipher mode of operation. (ecb = 0, cbc = 1, ctr = 2)

	bool optT = false, optI = false, optK = false, optP = false, optB = false, optO = false, optH = false, optS = false , optF = false;
	while((opt = getopt(argc, argv, ":t:i:k:pb:o:hsf")) != -1)
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
						configuration_num = 1;
						break;
					case '2':
						printf("[INFO] AES-192 selected.\n");
						n = 6;
						r = 13;
						nRound = 12;
						configuration_num = 2;
						break;
					case '3':
						printf("[INFO] AES-256 selected.\n");
						n = 8;
						r = 15;
						nRound = 14;
						configuration_num = 3;
						break;
					default:
						printf("[ERROR] Unknown AES mode.\n");
						return 1;
				}
				keylen = 4 * n;				
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
			case 'p':
				optP = true;
				printf("[INFO] Integrated keygen to be used.\n");
				break;
			case 'b':
				optB = true;
				//ecb, cbc, ctr,...
				if(strncmp(optarg,"ecb",3) == 0)
				{
					printf("[INFO] Block cipher mode of operation: ECB\n");
					bcm = 0;
				}
				else if(strncmp(optarg,"cbc", 3) == 0)
				{
					printf("[INFO] Block cipher mode of operation: CBC\n");
					bcm = 1;
				}
				else if(strncmp(optarg,"ctr", 3) == 0)
				{
					printf("[INFO] Block cipher mode of operation: CTR\n");
					bcm = 2;
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
			case 's':
				optS = true;

				printf("[INFO] Password check will be skipped during decryption.\n");
				break;
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