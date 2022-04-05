#ifndef LINALG_H
#define LINALG_H

// Function declarations
void matmulmodn(uint8_t** A, uint8_t** B, uint8_t** C, int p, int q, int r, int n);
unsigned int modular_inverse(unsigned int a, unsigned int m);
int modulo(int x, int m);
int modular_det(uint8_t** A, int n ,int m);
void inverse_modular_matrix(uint8_t** A, uint8_t** B, int n, int m);
void modular_ge_rre(uint8_t** A, int p, int q, int m);
int gcd(int a, int b);
void ERO1(uint8_t* Ri, uint8_t* Rj, int rowlen);
void ERO2(uint8_t* Ri, int lambda, int rowlen, int m);
void ERO3(uint8_t* Ri, uint8_t* Rj, int lambda, int rowlen, int m);

#endif