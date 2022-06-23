#include "stdio.h"
#include <pbc/pbc.h>
#include "randys.h"
int main()
{
    clock_t start, finish;
    pairing_t pairing;
    pairing_init(pairing, "./a.param");
    // element_t a, b, c, d;
    // element_init_G1(a, pairing);
    // element_init_G1(b, pairing); 
    // element_random(a);
    // element_set1(b);
    // start = clock();
    // for(size_t i = 0; i < 10000; i++)
    // {
    //     element_mul(b, b, a);
    // }
    // finish = clock();
    // printf("Mul in G1 costs %f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);

    
    // element_init_Zr(c, pairing);
    // element_init_Zr(d, pairing);
    // element_random(c);
    // element_set1(d);
    // start = clock();
    // for(size_t i = 0; i < 10000; i++)
    // {
    //     element_mul(d, d, c);
    // }
    // finish = clock();
    // printf("Mul in Zn costs %f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);

    // pbc_random_set_deterministic(12333);
    // element_t r;
    // element_init_Zr(r, pairing);
    // element_random(r);
    // element_printf("%B\n", r);
    // element_random(r);
    // element_printf("%B\n", r);
    // pbc_random_set_deterministic(12333);
    // element_random(r);
    // element_printf("%B\n", r);

    element_t g, h, a, b, c, temp1, temp2;
    element_t g_a, g_b, g_ab;
    element_init_G1(g, pairing);
    element_init_G1(h, pairing);
    element_init_G1(g_a, pairing);
    element_init_G1(g_b, pairing);
    element_init_G1(g_ab, pairing);
    element_init_Zr(a, pairing);
    element_init_Zr(b, pairing);
    element_init_Zr(c, pairing);
    element_init_GT(temp1, pairing);
    element_init_GT(temp2, pairing);
    element_random(g);
    element_random(h);
    element_random(a);
    element_random(b);
    element_mul(c, a, b);
    element_pow_zn(g_a, g, a);
    element_pow_zn(g_b, g, b);
    element_pow_zn(g_ab, g, c);
    pairing_apply(temp1, g_a, g_b, pairing);
    pairing_apply(temp2, g_ab, g, pairing);
    if (!element_cmp(temp1, temp2)) {
        printf("signature verifies\n");
    } else {
        printf("signature does not verify\n");
    }
}