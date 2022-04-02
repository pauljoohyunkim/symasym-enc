#include <stdlib.h>
#include <stdint.h>

#include "linalg.h"

// w = Av
// A is a p * q matrix and B is a q * r matrix
// C is automatically p * r matrix
void matmulmodn(uint8_t **A, uint8_t** B, uint8_t** C, int p, int q, int r, int n)
{
    // Matrix multiplication definition
    // C_ij = (A_ik * B_kj) mod n (Einstein notation)
    for(int i = 0; i < p; i++)
    {
        for(int j = 0; j < r; j++)
        {
            C[i][j] = 0;
            for(int k = 0; k < q; k++)
            {
                C[i][j] = (C[i][j] + A[i][k] * B[k][j]) % n;
            }
        }
    }
}
//Example usage of matmulmodn
/*
uint8_t **A;
    uint8_t **B;
    uint8_t **C;

    // Matrix construction
    A = (uint8_t**)malloc(sizeof(uint8_t*) * 3);
    B = (uint8_t**)malloc(sizeof(uint8_t*) * 3);
    C = (uint8_t**)malloc(sizeof(uint8_t*) * 3);

    for(int i = 0; i < 3; i++)
    {
        A[i] = (uint8_t*)malloc(sizeof(uint8_t) * 3);
        B[i] = (uint8_t*)malloc(sizeof(uint8_t) * 1);
        C[i] = (uint8_t*)malloc(sizeof(uint8_t) * 1);
    }

    A[0][0] = 6;
    A[0][1] = 24;
    A[0][2] = 1;
    A[1][0] = 13;
    A[1][1] = 16;
    A[1][2] = 10;
    A[2][0] = 20;
    A[2][1] = 17;
    A[2][2] = 15;
    B[0][0] = 0;
    B[1][0] = 2;
    B[2][0] = 19;


    matmulmodn(A, B, C, 3, 3, 1, 26);

    //Garbage Collection for A, B, C
*/



// x such that ax \equiv 1 (mod m)
// NOTE: ONLY USE FOR hcf(a,m) = 1
// To find such x is equivalent to solving ax + my = 1 where (x,y) are integer pairs
// Refer to Bézout's identity
/*
Use of Euclid's algorithm
m = q0 * a + r2
a = q1 * r2 + r3
r2 = q2 * r3 + r4
...
ri = qi * r(i+1) + r(i+2)
...
rj = qj * r(j+1) + 1

From the last equation to first,
1 = rj - qj * r(j+1)
...
r4 = r2 - q2 * r3
r3 = a - q1 * r2
r2 = m - q0 * a

Back substitution, paying attention to the "coefficient" of a results in the modular inverse
*/
unsigned int modular_inverse(unsigned int a, unsigned int m)
{
    int* q;
    int* r;

    q = (int*) calloc(sizeof(int), m);
    r = (int*) calloc(sizeof(int), m);

    // Convenient to assign m = r0 and a = r1
    r[0] = m;
    r[1] = a;

    unsigned int num_of_equ = 0;

    while(1)
    {
        q[num_of_equ] = r[num_of_equ] / r[num_of_equ + 1];
        r[num_of_equ + 2] = r[num_of_equ] % r[num_of_equ + 1];
        num_of_equ++;
        if(r[num_of_equ + 1] <= 1)
        {
            break;
        }
    }

    // Computing x
    /*
        x0 = 0
        x1 = 1
        x(i+2) = xi - qi * x(i+1)
    */
    int* x;
    x = (int*) calloc(sizeof(unsigned), m);
    x[0] = 0;
    x[1] = 1;
    for(int i = 0; i < num_of_equ; i++)
    {
        x[i + 2] = modulo(x[i] - q[i] * x[i + 1],m);
    }
    unsigned int ret = x[num_of_equ + 1];

    //Garbage collection
    free(q);
    free(r);
    free(x);

    return ret;
}

// Modulo operation that allows x to be negative.
int modulo(int x, int m)
{
    return (x % m + m) % m;
}

