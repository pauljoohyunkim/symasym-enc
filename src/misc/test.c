#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "linalg.h"
int main()
{
    uint8_t** A;
    uint8_t** B;
    A = (uint8_t**) malloc(sizeof(uint8_t*) * 3);
    B = (uint8_t**) malloc(sizeof(uint8_t*) * 3);
    for(int i = 0; i < 3; i++)
    {
        A[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3);
        B[i] = (uint8_t*) malloc(sizeof(uint8_t) * 3);
    }
    A[0][0] = 1;
    A[0][1] = 1;
    A[0][2] = 1;
    A[1][0] = 1;
    A[1][1] = 1;
    A[1][2] = 2;
    A[2][0] = 25;
    A[2][1] = 20;
    A[2][2] = 46;

    int i = modular_det(A, 3, 48);
    printf("%d\n", i);

    inverse_modular_matrix(A, B, 3, 48);




    for(int i = 0; i < 3; i++)
    {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);
    return 0;
}