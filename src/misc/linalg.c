#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    if(a == 1)
    {
        return 1;
    }

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

// Determinant of a modular (square n*n) matrix (mod m)
int modular_det(uint8_t** A, int n ,int m)
{
    if(n == 1)
    {
        return modulo(A[0][0], m);
    }
    else if(n == 2)
    {
        return modulo(A[0][0] * A[1][1] - A[0][1] * A[1][0], m);
    }

    // Otherwise, use Laplace's expansion and recursion.
    
    // Create an (n-1)*(n-1) temporary matrix for Laplace's expansion
    uint8_t** temp;
    temp = (uint8_t**) malloc(sizeof(uint8_t*) * (n - 1));
    for(int i = 0; i < n - 1; i++)
    {
        temp[i] = (uint8_t*) malloc(sizeof(uint8_t) * (n - 1));
    }

    // Laplace's expansion
    int ret = 0;
    for(int i = 0; i < n; i++)          // Index through the 0th row.
    {
        // Populate (n-1)*(n-1) matrix (the 0th row and ith column should be missing)
        for(int j = 0; j < n - 1; j++)
        {
            int l = 0;
            for(int k = 0; k < n - 1; k++)
            {
                if(k == i)
                {
                    l++;
                }
                temp[j][k] = A[j + 1][l];
                l++;
            }
        }

        if(i%2 == 0)
        {
            ret += modulo(A[0][i] * modular_det(temp, n - 1, m), m);
        }
        else
        {
            ret -= modulo(A[0][i] * modular_det(temp, n - 1, m), m);
        }
    }


    // Garbage collection
    for(int i = 0; i < n-1; i++)
    {
        free(temp[i]);
    }
    free(temp);

    return ret;
}

// B = A^(-1) in mod m
// where A is an n*n square modular matrix.
void inverse_modular_matrix(uint8_t** A, uint8_t** B, int n, int m)
{
    // Consider n*(2n) matrix C = A|I (I is the n*n identity matrix.)
    // Applying Gaussian elimination to turn A into I would make I into inverse of A.
    uint8_t** C;
    C = (uint8_t**) malloc(sizeof(uint8_t*) * n);
    for(int i = 0; i < n; i++)
    {
        C[i] = (uint8_t*) calloc(2*n, sizeof(uint8_t));
        memcpy(C[i], A[i], n);
        C[i][n + i] = 1;        // Identity matrix
    }

    // GAUSSIAN ELIMINATION
    modular_ge_rre(C, n, 2*n, m);

    // Writing to output
    
    for(int i = 0; i < n; i++)
    {
        memcpy(B[i], C[i] + n, n);
    }





    for(int i = 0; i < n; i++)
    {
        free(C[i]);
    }
    free(C);
}

// Modular Gaussian Elimination to Reduced Row Echelon form
// A <= GERRE(A) (mod m) where A is of dimension p*q
// Applicable only if p <= q
void modular_ge_rre(uint8_t** A, int p, int q, int m)
{
    if(p == 1)
    {
        ERO2(A[0], modular_inverse(A[0][0], m), q, m);
        return;
    }


    // Check first entry.
    if(gcd(A[0][0], m) != 1)        // Not coprime, so no multiplicative modular inverse.
    {
        int swapindex = 0;
        while(swapindex < p)
        {
            if(gcd(A[swapindex][0], m) == 1)
            {
                //Swap row
                ERO1(A[0], A[swapindex], q);
                break;
            }
            swapindex++;
        }
    }

    // "Divide" the 0th row by the 0th entry.
    int inva00 = modular_inverse(A[0][0], m);
    ERO2(A[0], inva00, q, m);

    // A[0][0] = 1 now.
    for(int i = 1; i < p; i++)
    {
        ERO3(A[i], A[0], -A[i][0], q, m);
    }

    // Inductively progressing
    uint8_t** tempmat;
    tempmat = (uint8_t**) malloc(sizeof(uint8_t*) * (p - 1));
    for(int i = 0; i < p - 1; i++)
    {
        tempmat[i] = (uint8_t*) malloc(sizeof(uint8_t) * (q - 1));
        memcpy(tempmat[i], A[i + 1] + 1, q - 1);
    }
    modular_ge_rre(tempmat, p - 1, q - 1, m);
    for(int i = 0; i < p - 1; i++)
    {
        memcpy(A[i + 1] + 1, tempmat[i], q - 1);
    }
    
    // Now in echelon. We further transform to reduced row echelon

    for(int i = 1; i < p; i++)
    {
        for(int j = 0; j < i; j++)
        {
            ERO3(A[j], A[i], -A[j][i], q, m);
        }
    }


    // Garbage Collection
    for(int i = 0; i < p - 1; i++)
    {
        free(tempmat[i]);
    }
    free(tempmat);

}

// Greatest common divisor
int gcd(int a, int b)
{

    if (a == 0)
       return b;
    if (b == 0)
       return a;
 
    if (a == b)
        return a;
 
    if (a > b)
        return gcd(a-b, b);
    return gcd(a, b-a);
}

// Elementary row operation 1: Swap two rows
void ERO1(uint8_t* Ri, uint8_t* Rj, int rowlen)
{
    uint8_t temp;
    for(int i = 0; i < rowlen; i++)
    {
        temp = Ri[i];
        Ri[i] = Rj[i];
        Rj[i] = temp;
    }
}

// Elementary row operation 2: Ri -> lambda * Ri
void ERO2(uint8_t* Ri, int lambda, int rowlen, int m)
{
    for(int i = 0; i < rowlen; i++)
    {
        Ri[i] = modulo(Ri[i] * lambda, m);
    }
}

// Elementary row operation 3: Ri -> Ri + lambda * Rj
void ERO3(uint8_t* Ri, uint8_t* Rj, int lambda, int rowlen, int m)
{
    for(int i = 0; i < rowlen; i++)
    {
        Ri[i] = modulo(Ri[i] + lambda * Rj[i], m);
    }
}