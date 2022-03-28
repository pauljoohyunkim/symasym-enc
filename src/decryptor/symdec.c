#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

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
		"\t-o <file>  : specify output file. (default: <input file> with encryption extension erased.)\n"
		"\t-s         : skip password check.\n"
		"\t-f         : skip file integrity check at the end.\n"
		"\n";
	printf("%s",helpText);
}