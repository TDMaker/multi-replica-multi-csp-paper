#include "stdio.h"
#include "randys.h"
#include "merkletree.h"
#include <pbc/pbc.h>
#define PAIRING_PARAM_FILE_PATH "a.param"
#define FILE_PATH "the-sun-also-rises.mp3"
#define BLK_SIZE 20
#define TOTAL_NUMBER 5000
#define REPLICA_NUMBER 5
#define CHAL_NUMBER 1000
#define R4RP 2333
#define CHAL_SEED 984521

int main()
{
    printf("Running...\n");
    mt_t *mt;
    mt_hash_t root;
    memset(root, 0, sizeof(mt_hash_t));
    mt = mt_create();

    clock_t start, finish;
    pairing_t pairing;
    element_t pt_alpha, pt_v, pt_sig, g, u, pt_h, pt_mi, pt_tmp_z, pt_tmp_z2, pt_tmp_g, pt_h_ij, b_ij, sigma_ij;

    FILE *fp = NULL;
    if (pairing_init(pairing, PAIRING_PARAM_FILE_PATH) < 0)
        exit(0);

    element_init_Zr(pt_alpha, pairing);
    element_init_Zr(pt_mi, pairing);
    element_init_G1(pt_v, pairing);
    element_init_G1(pt_sig, pairing);
    element_init_G1(g, pairing);
    element_init_G1(u, pairing);
    element_init_G1(pt_h, pairing);
    element_init_G1(pt_tmp_g, pairing);
    element_init_G1(pt_h_ij, pairing);
    element_init_Zr(pt_tmp_z, pairing);
    element_init_Zr(pt_tmp_z2, pairing);

    element_random(pt_alpha);
    element_random(g);
    element_random(u);
    element_pow_zn(pt_v, g, pt_alpha);

    
    element_init_Zr(b_ij, pairing);
    element_init_G1(sigma_ij, pairing);

    if ((fp = fopen(FILE_PATH, "r")) == NULL)
    {
        printf("Fail to open the sourced file!\n");
        exit(0);
    }
    start = clock();
    long *lr = (long *)malloc(sizeof(long) * REPLICA_NUMBER);
    psd_func(lr, REPLICA_NUMBER, R4RP, 88);
    element_t rand[REPLICA_NUMBER];
    for(size_t k = 0; k < REPLICA_NUMBER; k++)
    {
        element_t r;
        element_init_Zr(rand[k], pairing);
        element_set_si(rand[k], lr[k]);
    }
    int zr_size = pairing_length_in_bytes_Zr(pairing);
    unsigned char *replica_str = pbc_malloc(zr_size);
    element_t b_IJ[TOTAL_NUMBER][REPLICA_NUMBER];
    element_t sigma_IJ[TOTAL_NUMBER][REPLICA_NUMBER];
    unsigned char* file_block = malloc(BLK_SIZE);
    unsigned char h_ij[32] = {0};
    for(size_t i = 0; i < TOTAL_NUMBER; i++)
    {
        fread(file_block, 1, BLK_SIZE, fp);
        element_from_hash(pt_mi, file_block, BLK_SIZE);
        for(size_t j = 0; j < REPLICA_NUMBER; j++)
        {
            element_add(b_ij, pt_mi, rand[j]);
            memset(h_ij, 0, 32);
            element_pow_zn(pt_tmp_g, u, b_ij);
            element_to_bytes(replica_str, b_ij); // replica_str = b_ij
            sha256(replica_str, BLK_SIZE, h_ij);
            element_from_hash(pt_h_ij, h_ij, 32);
            element_mul(pt_h_ij, pt_h_ij, pt_tmp_g);
            element_pow_zn(sigma_ij, pt_h_ij, pt_alpha);

            element_init_Zr(b_IJ[i][j], pairing);
            element_init_G1(sigma_IJ[i][j], pairing);
            element_set(b_IJ[i][j], b_ij);
            element_set(sigma_IJ[i][j], sigma_ij);
        }
    }
    finish = clock();
    printf("Tag generation costs %f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);

    // Challenge
    unsigned int *u_s = (unsigned int *)malloc(sizeof(unsigned int) * CHAL_NUMBER);
    psd_permute(u_s, CHAL_NUMBER, CHAL_SEED, TOTAL_NUMBER);
    pbc_random_set_deterministic(CHAL_SEED);

    // ProofGen
    element_t sigma_J[REPLICA_NUMBER], mu_J[REPLICA_NUMBER];
    for(size_t j = 0; j < REPLICA_NUMBER; j++)
    {
        element_init_G1(sigma_J[j], pairing);
        element_init_Zr(mu_J[j], pairing);
        element_set1(sigma_J[j]);
        element_set0(mu_J[j]);
    }
    start = clock();
    for(size_t i = 0; i < CHAL_NUMBER; i++)
    {
        element_random(pt_tmp_z); // pt_tmp_z = r_ij
        for(size_t j = 0; j < REPLICA_NUMBER; j++)
        {
            element_pow_zn(pt_tmp_g, sigma_IJ[u_s[i]][j], pt_tmp_z); // pt_tmp_g = sigma_ij^{r_ij}  # i\in Q
            element_mul(sigma_J[j], sigma_J[j], pt_tmp_g); // sigma_j *= sigma_ij^{r_ij}

            element_mul_zn(pt_tmp_z2, pt_tmp_z, b_IJ[u_s[i]][j]);
            element_add(mu_J[j], mu_J[j], pt_tmp_z2); // mu_j += pt_tmp_z2
        }
    }
    finish = clock();
    printf("Proof generation costs %f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);

    // ProofVerify
    element_t left_p[REPLICA_NUMBER], right_p[REPLICA_NUMBER], innerprod[REPLICA_NUMBER];
    for(size_t j = 0; j < REPLICA_NUMBER; j++)
    {  
        element_init_G1(innerprod[j], pairing);
        element_init_GT(left_p[j], pairing);
        element_init_GT(right_p[j], pairing);
    }
    pbc_random_set_deterministic(CHAL_SEED);
    for(size_t i = 0; i < CHAL_NUMBER; i++)
    {
        element_random(pt_tmp_z); // tmp_z = r_ij
        for(size_t j = 0; j < REPLICA_NUMBER; j++)
        {
            memset(h_ij, 0, 32);
            element_to_bytes(replica_str, b_IJ[u_s[i]][j]); // replica_str = b_ij
            sha256(replica_str, BLK_SIZE, h_ij);
            element_from_hash(pt_h_ij, h_ij, 32);
            element_pow_zn(pt_tmp_g, pt_h_ij, pt_tmp_z);
            element_mul(innerprod[j], innerprod[j], pt_tmp_g);
        }
    }
    for(size_t j = 0; j < REPLICA_NUMBER; j++)
    {
        element_pow_zn(pt_tmp_g, u, mu_J[j]);
        element_mul(innerprod[j], innerprod[j], pt_tmp_g);

        pairing_apply(left_p[j], sigma_J[j], g, pairing);
        pairing_apply(right_p[j], innerprod[j], pt_v, pairing);

        if (!element_cmp(left_p[j], right_p[j])) {
            printf("proof verifies in Replica %ld\n", j);
        } else {
            printf("proof does not verify in Replca %ld\n", j);
        }
    }

    finish = clock();
    printf("Proof verification costs %f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);

    element_clear(pt_alpha);
    element_clear(pt_v);
    element_clear(pt_sig);
    element_clear(g);
    element_clear(u);
    element_clear(pt_h);
    element_clear(pt_mi);
    element_clear(pt_tmp_z);
    element_clear(pt_tmp_z2);
    element_clear(pt_tmp_g);
    element_clear(pt_h_ij);
    // element_clear(sigma_j);
    // element_clear(mu_j);
    // element_clear(left_p);
    // element_clear(right_p);
    // element_clear(innerprod);
    pairing_clear(pairing);
    return 0;
}