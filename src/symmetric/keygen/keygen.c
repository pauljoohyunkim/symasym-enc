#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "keygen.h"
#include "../../misc/hash.h"


// Returns 1 if password does not match.
int keygenF(unsigned int keylen, FILE* fp)
{
    // len = HASHLEN * nBlocks + nTrailing
    unsigned int nBlocks = keylen / HASHLEN;
    unsigned int nTrailing = keylen % HASHLEN;

    // Password check
    char password1[SIZE + 1] = { 0 };
    char password2[SIZE + 1] = { 0 };

    printf("Enter the password (less than 100 characters): ");
    getPassword(password1);
    printf("Enter the password again: ");
    getPassword(password2);
    if(strcmp(password1,password2) != 0)
    {
        printf("[ERROR] Passwords do not match.\n");
        return 1;
    }

    unsigned int passlen = strlen(password1);

    uint8_t buffer[HASHLEN] = { 0 };

    // Initial hashing (once)
    sha256A(0, NULL, passlen, password1, buffer);

    for(unsigned int i = 0; i < nBlocks; i++)
    {
        for(int j = 0; j < HASHREP; j++)
        {
            sha256A(0, NULL, HASHLEN, buffer, buffer);
        }
        fwrite(buffer, 1, HASHLEN, fp);
    }

    if(nTrailing > 0)
    {
        for(int j = 0; j < HASHREP; j++)
        {
            sha256A(0, NULL, HASHLEN, buffer, buffer);
        }
        fwrite(buffer, 1, nTrailing, fp);
    }

    return 0;
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