#include "stdio.h"
#include "randys.h"
#include <pbc/pbc.h>
#include "merkletree.h"
#define PAIRING_PARAM_FILE_PATH "./a.param"
#define FILE_PATH "the-sun-also-rises.mp3"
#define TOTAL_NUMBER 5000
#define CHAL_NUMBER 1000
#define BLK_SIZE 20
#define ID_O "9527"
#define ID_C "2333"
#define name "9853"
#define s "8341"
#define m_max 5
#define CHAL_SEED 984521

#define D_TEST_VALUES 17
int main()
{
    printf("Running...\n");

    mt_t *mt;
    mt_hash_t root;
    memset(root, 0, sizeof(mt_hash_t));
    mt = mt_create();

    clock_t start, finish;
    pairing_t pairing;
    element_t pt_s, pt_x_o, pt_sk_o1, pt_sk_o2, pt_pk_o, g, pt_mpk, pt_block_i, pt_a, pt_A, pt_sk_c, pt_pk_c, pt_ssk_c;
    element_t innerprod, outerprod, tmp;
    element_t left_p, right_p, left, right;
    element_t tmp_zq1, tmp_zq2, tmp_g1, tmp_g2;
    FILE *fp = NULL;

    if (pairing_init(pairing, PAIRING_PARAM_FILE_PATH) < 0)
        exit(0);
    element_init_Zr(tmp_zq1, pairing);
    element_init_Zr(tmp_zq2, pairing);
    element_init_G1(tmp_g1, pairing);
    element_init_G1(tmp_g2, pairing);

    element_init_Zr(pt_a, pairing);
    element_init_G1(pt_A, pairing);
    
    element_init_G1(g, pairing);
    element_init_Zr(pt_block_i, pairing);

    element_init_Zr(pt_s, pairing);
    element_init_Zr(pt_x_o, pairing);

    element_init_G1(pt_sk_o1, pairing);
    element_init_Zr(pt_sk_o2, pairing);

    element_init_G1(pt_pk_o, pairing);
    element_init_G1(pt_mpk, pairing);

    element_init_Zr(pt_sk_c, pairing);
    element_init_G1(pt_pk_c, pairing);
    element_init_G1(pt_ssk_c, pairing);

    element_random(g);
    element_random(pt_s);
    element_random(pt_x_o);
    element_random(pt_a);

    element_pow_zn(pt_A, g, pt_a);
    
    element_init_G1(innerprod, pairing);
    element_init_G1(outerprod, pairing);
    element_init_G1(tmp, pairing);
    element_set1(innerprod);
    element_set1(outerprod);
    
    element_init_GT(left_p, pairing);
    element_init_GT(right_p, pairing);
    element_init_GT(left, pairing);
    element_init_GT(right, pairing);

    int zr_size = pairing_length_in_bytes_Zr(pairing);

    // Calculate $sk_{O1}$, namely $ssk_O$
    H1(tmp_g1, ID_O); // tmp_g1 = H1(ID_O)
    element_pow_zn(pt_sk_o1, tmp_g1, pt_s); // sk_o1 = tmp_g1^pt_s

    // Calculate $sk_{O2}$
    H2(tmp_zq1, pt_sk_o1); // tmp_zq1 = H2(pt_sk_o1)
    element_add(pt_sk_o2, pt_x_o, tmp_zq1); // sk_o2 = x_o + tmp_zq1

    // Calculate $pk_o$
    element_pow_zn(pt_pk_o, g, pt_sk_o2);

    // Calculate $mpk$
    element_pow_zn(pt_mpk, g, pt_s);

    if ((fp = fopen(FILE_PATH, "r")) == NULL)
    {
        printf("Fail to open the sourced file!\n");
        exit(0);
    }

    unsigned char* file_block = malloc(BLK_SIZE);
    unsigned char *replica_str = pbc_malloc(zr_size);
    element_t sigma_I[TOTAL_NUMBER];
    element_t replicas[TOTAL_NUMBER][m_max]; // 5000 * 2
    element_t H4bid[TOTAL_NUMBER][m_max];
    unsigned char hash_aggred[1024] = {0};
    unsigned char h_i[32] = {0};
    unsigned char bid_ij[128] = {0};
    start = clock();
    for(size_t i = 0; i < TOTAL_NUMBER; i++)
    {
        fread(file_block, 1, BLK_SIZE, fp);
        element_from_hash(pt_block_i, file_block, BLK_SIZE);
        memset(hash_aggred, 0, 1024);
        // element_t sigma_i;
        element_init_G1(sigma_I[i], pairing);
        element_set1(sigma_I[i]);
        for(size_t j = 0; j < m_max; j++)
        {
            // 计算副本
            H3(tmp_zq1, pt_sk_o2, name, j); // tmp_zq1 = H3(sk_o2, name||j)
            element_init_Zr(replicas[i][j], pairing);
            element_add(replicas[i][j], pt_block_i, tmp_zq1); // b_ij = b_i + tmp_zq1
            // 计算副本的哈希
            element_to_bytes(replica_str, replicas[i][j]); // replica_str = b_ij
            sha256(replica_str, zr_size, hash_aggred + 32 * j); // hash_aggred += b_ij
        }
        // 计算 h_i
        memset(h_i, 0, 32);
        sha256(hash_aggred, zr_size * m_max, h_i); // h_i = H(b_ij || b_ij || ... || b_ij)

        mt_add(mt, h_i, HASH_LENGTH); // append to MHT

        element_t sigma_ij[m_max]; // TODO: need free
        for(size_t j = 0; j < m_max; j++)
        {
            // 计算 bid_ij
            memset(bid_ij, 0, 128);
            set_bid(bid_ij, name, ID_O, m_max, j, h_i); // bid_ij = name || ID_O || m_max || j ||h_i
            // 计算 sigma_ij
            element_init_G1(H4bid[i][j], pairing);
            element_from_hash(H4bid[i][j], bid_ij, strlen(bid_ij)); // store H4(bid_ij)

            element_mul(tmp_zq2, pt_a, replicas[i][j]); // tmp_zq2 = a * b_ij
            element_pow_zn(tmp_g2, g, tmp_zq2); // tmp_g2 = g^{a * b_ij}
            element_mul(tmp_g2, H4bid[i][j], tmp_g2); // tmp_g1 = h4bid * g^{a * b_ij}
            element_pow_zn(tmp_g2, tmp_g2, pt_sk_o2); // (h4bid * g^{a * b_ij})^{sk_o2}
            element_mul(tmp_g2, pt_sk_o1, tmp_g2); // sk_o1 * (h4bid * g^{a * b_ij})^{sk_o2}
            element_mul(sigma_I[i], sigma_I[i], tmp_g2);
        }
    }
    mt_get_root(mt, root);
    for(int i = 0; i < 32; i++)
    {
        printf("%x", root[i]);
    }
    printf("\n");
    finish = clock();
    printf("Tag generation costs %f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
    // Challenge Generation
    unsigned int *u_s = (unsigned int *)malloc(sizeof(unsigned int) * CHAL_NUMBER);
    psd_permute(u_s, CHAL_NUMBER, CHAL_SEED, TOTAL_NUMBER);
    pbc_random_set_deterministic(CHAL_SEED);
    // Proof Generation
    element_t sigma, mu;
    element_init_G1(sigma, pairing);
    element_init_Zr(mu, pairing);
    element_set1(sigma);
    element_set0(mu);
    start = clock();
    for(size_t i = 0; i < CHAL_NUMBER; i++)
    {
        element_random(tmp_zq1); // tmp_zq1 = r_i
        element_pow_zn(tmp_g1, sigma_I[u_s[i]], tmp_zq1); // tmp_g1 = sigma_i^{r_i}  # i\in Q
        element_mul(sigma, sigma, tmp_g1); // sigma *= sigma_i^{r_i}

        element_set0(tmp_zq2);
        for(size_t j = 0; j < m_max; j++)
        {
            element_add(tmp_zq2, tmp_zq2, replicas[u_s[i]][j]); //  tmp_zq2 += b_ij
        }
        element_mul_zn(tmp_zq2, tmp_zq2, tmp_zq1); // tmp_zq2 *= r_i
        element_add(mu, mu, tmp_zq2); // mu += tmp_zq2
    }
    finish = clock();
    printf("Proof generation costs %f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);

    // Proof verification
    // Calculate $sk_{C}$
    H1(tmp_g1, ID_C); // tmp_g1 = H1(ID_C)
    element_pow_zn(pt_ssk_c, tmp_g1, pt_s); // ssk_c = tmp_g1^pt_s
    H2(tmp_zq1, pt_ssk_c); // tmp_zq1 = H2(pt_ssk_c)
    element_add(pt_sk_c, pt_s, tmp_zq1); // sk_c = x_c + tmp_zq1, take x_c as s
    element_pow_zn(pt_pk_c, g, pt_sk_c);
    start = clock();
    element_from_hash(tmp_g1, root, 32); // tmp_g1 = H4(R)
    element_pow_zn(tmp_g2, tmp_g1, pt_sk_c); // temp_g2 = H4(R)^{sk_c}
    pairing_apply(left, tmp_g2, g, pairing);
    pairing_apply(right, tmp_g1, pt_pk_c, pairing);
    if (!element_cmp(left, right)) {
        printf("TPA signature verifies\n");
    } else {
        printf("TPA signature does not verify\n");
        return -1;
    }

    pbc_random_set_deterministic(CHAL_SEED);
    element_set0(tmp_zq2);
    for(size_t i = 0; i < CHAL_NUMBER; i++)
    {
        element_set1(innerprod);
        element_random(tmp_zq1); // tmp_zq1 = r_i
        element_add(tmp_zq2, tmp_zq2, tmp_zq1); // tmp_zq2 += r_i
        for(size_t j = 0; j < m_max; j++)
        {
            element_pow_zn(tmp, H4bid[u_s[i]][j], tmp_zq1); // tmp = H4bid^{r_i}
            element_mul(innerprod, innerprod, tmp); // innerprod *= H4bid^{r_i}
        }
        element_mul(outerprod, outerprod, innerprod);
    }
    element_pow_zn(pt_A, pt_A, mu); // pt_A = pt_A^\mu
    element_mul(outerprod, outerprod, pt_A); // outerprod = outerprod * A^\mu
    pairing_apply(right_p, outerprod, pt_pk_o, pairing); // right_p = e(outerprod, pk_o)

    H1(tmp_g1, ID_O); // tmp_g1 = H1(ID_O)
    element_pow_zn(tmp_g1, tmp_g1, tmp_zq2); // tmp_g1 = H1(ID_O)^{\sum_{i\in Q}r_i}
    pairing_apply(left_p, tmp_g1, pt_mpk, pairing);

    element_mul(right, left_p, right_p);
    pairing_apply(left, sigma, g, pairing);

    if (!element_cmp(left, right)) {
        printf("proof verifies\n");
    } else {
        printf("proof does not verify\n");
    }
    finish = clock();
    printf("Proof verification costs %f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
    // free
    free(file_block);
    free(u_s);

    pbc_free(replica_str);
    element_clear(pt_s);
    element_clear(pt_x_o);
    element_clear(pt_sk_o1);
    element_clear(pt_sk_o2);
    element_clear(pt_pk_o);
    element_clear(g);
    element_clear(pt_block_i);
    element_clear(pt_a);
    element_clear(pt_A);
    element_clear(tmp_zq1);
    element_clear(tmp_zq2);
    element_clear(tmp_g1);
    element_clear(tmp_g2);
    element_clear(mu);
    element_clear(innerprod);
    element_clear(outerprod);
    element_clear(tmp);
    pairing_clear(pairing);

    return 0;
}
