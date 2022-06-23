#include "stdio.h"
#include <pbc/pbc.h>
#include "randys.h"
#define n 1000
int main()
{
    clock_t start, finish;
    pairing_t pairing;
    pairing_init(pairing, "./a.param");
    element_t a, b, aa;
    element_init_G1(a, pairing);
    element_init_G1(aa, pairing);
    element_init_Zr(b, pairing);
    element_random(a);
    element_random(b);
    // TB: 505nExp+504nMul
    start = clock();
    for(size_t i = 0; i < 505 * n; i++)
    {
        element_pow_zn(aa, a, b);
    }

    for(size_t i = 0; i < 504 * n; i++)
    {
        
    }
    finish = clock();
    printf("Exp in G1 costs %f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
    // MuR: 1000nExp + 500nMul


    // ID: (1000n+1)Exp + 500nMul

    // MDSS: 10nExp + (10n+4)Mul
}