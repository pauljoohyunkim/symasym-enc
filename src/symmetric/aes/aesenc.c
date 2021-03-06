#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>


#include "aes.h"
#include "aesenc.h"
#include "../bcm/bcm.h"
#include "../../misc/hash.h"
#include "../../misc/file.h"
#include "../keygen/keygen.h"

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
	int keylen;			//keylen = 4 * n
	int configuration_num;		// 1 for AES128, 2 for AES192, 3 for AES256

	// Options
	/*

	Mandatory options are starred.
	t: AES type* (1: AES128, 2: AES192, 3:AES256)
	i: Input*
	k: Key file*
	p: Enter password instead of inputting a key file.* (Mutually exclusive with -k option.)
	b: Block mode*
	o: Output file
	h: Help
	s: Password check skip
	f: File integrity check skip
	d: Delete file after encryption.
	n: Do not delete the file after encryption. (Mutually exclusive with -d option.)
	(If neither -d nor -n option is used, then you will be prompted.)
	*/

	char* inputFileName;
	char* inputKeyFileName;
	char* outputFileName;
	uint8_t bcm;		//Block cipher mode of operation. (ecb = 0, cbc = 1, ctr = 2)

	bool optT = false, optI = false, optK = false, optP = false, optB = false, optO = false, optS = false , optF = false, optD = false, optN = false;
	while((opt = getopt(argc, argv, ":t:i:k:pb:o:hsfdn")) != -1)
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
						configuration_num = 1;
						break;
					case '2':
						printf("[INFO] AES-192 selected.\n");
						n = 6;
						configuration_num = 2;
						break;
					case '3':
						printf("[INFO] AES-256 selected.\n");
						n = 8;
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
			case 'd':
				optD = true;
				printf("[INFO] Input file is set to be deleted after encryption.\n");
				break;
			case 'n':
				optN = true;
				printf("[INFO] Input file is set not to be deleted after encryption.\n");
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
	if(!((optT && optI) && ((optK || optP) && optB)))
	{
		printf("[ERROR] One of the required options is missing.\n");
		return 1;
	}

	// -k and -p are mutually exclusive.
	if(optK && optP)
	{
		printf("[ERROR] Make up your mind! Use either -k or -p!\n");
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

	if(optK)
	{
		// Key reading
		if((keyFile = fopen(inputKeyFileName, "rb")) == NULL)
		{
			printf("[ERROR] Key file could not be read.\n");
			return 1;
		}
		read_bytes = fread(key, 1, keylen, keyFile);
		if(read_bytes < keylen)
		{
			printf("[INFO] Key file was shorter than the length required by the specified AES type, so it was padded with zeros. Make sure you use the same key file for decryption.\n");
		}	
		fclose(keyFile);
		free(inputKeyFileName);
	}

	// Integrated keygen mode
	if(optP)
	{
		keygen(keylen, NULL, key, true);
	}

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



	// README: 0x00 (A); AES encrypted files starts with two zero bytes.
	fwrite("\x00\x00",1,2,outputFile);

	// README: (B) 0x00 for AES-128, 0x01 for AES-192, 0x02 for AES-256
	if(keylen == 16)
	{
		fwrite("\x00",1,1,outputFile);
	}
	else if(keylen == 24)
	{
		fwrite("\x01",1,1,outputFile);
	}
	else
	{
		fwrite("\x02",1,1,outputFile);
	}

	// README: Configuration (C)
	// c0, c1, c2 is from BCM
	// c3, c4, c5 is left zero.
	// c6 from -s option	
	// c7 from -f option
	uint8_t c = bcm << 2;
	if(optS)
	{
		c += 2;
	}
	if(optF)
	{
		c += 1;
	}
	fwrite(&c,1,1,outputFile);

	//Initialization Vector (Does not exist if ECB.)
	uint8_t iv[16];
	ivbytes(iv,16);		//IV generation
	fwrite(iv,1,16,outputFile);
	

	uint8_t temphash[32] = { 0 };
	// README: sha256(iv + password)
	if(!(optS))
	{
		sha256A(keylen,key,16,iv,temphash);
		fwrite(temphash,1,32,outputFile);
	}

	// README: sha256(key + file)
	if(!(optF))
	{
		sha256F(inputFile, keylen, key, temphash);
		fwrite(temphash, 1,32,outputFile);
	}
	
	// Number of needed bytes for a full 16 byte block (NOT NEEDED FOR CTR)
	uintmax_t filesize = sizeF(inputFile);
	uint8_t stuffing = (- filesize % 16);

	switch(bcm)
	{
		case 0:
			//ECB
			fwrite(&stuffing, 1, 1, outputFile);
			ecb_enc(key, 16, inputFile, outputFile, configuration_num, aes);
			break;
		case 1:
			//CBC
			fwrite(&stuffing, 1, 1, outputFile);
			cbc_enc(iv, key, 16, inputFile, outputFile, configuration_num, aes);
			break;
		case 2:
			//CTR
			ctr_enc(iv, key, 16, inputFile, outputFile, configuration_num, aes);
			break;
		
	}



	fclose(inputFile);
	fclose(outputFile);

	printf("[INFO] Finished.\n");



	// Ask whether to delete the input file or not.
	// Question only triggered when indeterminant.
	if(optD == optN)
	{
		printf("Delete the input file? [y/N]\n");
		char deleteprompt = getchar();
		if(deleteprompt == 'y' || deleteprompt == 'Y')
		{
			remove(inputFileName);
			printf("[INFO] Input file removed.\n");
		}
		else
		{
			printf("[INFO] Input file not removed.\n");
		}
	}
	else if (optD)
	{
		remove(inputFileName);
		printf("[INFO] Input file removed.\n");
	}
	else if (optN)
	{
		printf("[INFO] Input file not removed.\n");
	}

	// Free names
	free(inputFileName);
	free(outputFileName);


	return 0;
}

void showHelp()
{
	char helpText[]
		= "\n"
		" usage: aesenc [options]"
		"\n"
		" Mandatory options:\n"
		"\n"
		"\t-t <1,2,3> : AES type (1: AES-128, 2: AES-192, 3: AES-256)\n"
		"\t-i <file>  : file to encrypt\n"
		"\t-k <file>  : key file. Depending on the type, only the first few bytes will be used.\n"
		"\t-p         : use integrated keygen instead of a key file. (Mutually exclusive with -k option)\n"
		"\t-b <mode>  : block cipher mode of operation; (ecb, cbc, ctr)\n"
		"\n"
		" Common options:\n"
		"\n"
		"\t-o <file>  : specify output file. (default: <input file>.aes)\n"
		"\t-s         : skip password check during decryption.\n"
		"\t-f         : do not include file integrity hash.\n"
		"\t-d         : Delete file after encryption.\n"
		"\t-n         : Do not delete the file after encryption. (Mutually exclusive with -d option.)\n"
		"\t             (If neither -d nor -n option is used, then you will be prompted.)\n"
		"\t-h         : show help.\n"
		"\n";
	printf("%s",helpText);
}
