#ifndef LINALG_H
#define LINALG_H

// Function declarations
void matmulmodn(uint8_t** A, uint8_t** B, uint8_t** C, int p, int q, int r, int n);
unsigned int modular_inverse(unsigned int a, unsigned int m);
int modulo(int x, int m);

#endif