#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>

#include "symkeygen.h"
#include "../../misc/hash.h"

/*
    This program works by taking the password from the user and generating SHA256 hashes.

    More specifically, given user input password (password), the key file generated would be:
    h101(password) h201(password) h301(password) ... (truncated to the length specified.)
    where hn(.) is applying SHA256 n times.
*/



int main(int argc, char** argv)
{
    int opt;
    unsigned int len = SIZE;

    // l: Length, h: Help
    bool optL = false, optH = false;
    while((opt = getopt(argc, argv, ":l:h")) != -1)
    {
        switch(opt)
        {
            case 'l': ;
                char* strptr;
                len = strtol(optarg, &strptr, 10);
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
    if(optind < argc)
    {
        printf("[INFO] Key file to be generated: %s\n",argv[optind]);
        keyFileNameLen = strlen(argv[optind]);
        keyFileName = (char*) calloc(keyFileNameLen + 2, sizeof(char));
    }

    
    // len = HASHLEN * nBlocks + nTrailing
    unsigned int nBlocks = len / HASHLEN;
    unsigned int nTrailing = len % HASHLEN;

    char password1[SIZE] = { 0 };
    char password2[SIZE] = { 0 };

    printf("Enter the password: ");
    getPassword(password1);
    printf("Enter the password again: ");
    getPassword(password2);

    if(strcmp(password1,password2) != 0)
    {
        printf("[ERROR] Passwords do not match.\n");
        return 1;
    }

    

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
		"\n";
	printf("%s",helpText);
}


// Silently get password into an array.
// Source: https://stackoverflow.com/questions/1786532/c-command-line-password-input
void getPassword(char *password)
{
    struct termios oldt, newt;
    int i = 0;
    int c;

    tcgetattr(STDIN_FILENO, &oldt);         // Get the current terminal setting
    newt = oldt;

    newt.c_lflag &= ~(ECHO);                // Turn off echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);    // Set it to the new terminal setting

    while((c = getchar()) != '\n' && c != EOF && i < SIZE)
    {
        password[i++] = c;
    }

    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);   // Turn it back to the old terminal setting
    printf("\n");
}