#include <gmp.h>
#include "../utils/randys.h"
#include "../utils/sha256.h"
#define BASE_PATH "../swap"
// #define SOURCED_FILE_NAME "the-sun-also-rises.flac"
#define SOURCED_FILE_NAME "the-last-emperor.mkv"
#define FILE_P1_PATH BASE_PATH "/%s.p1"
#define FILE_P2_PATH BASE_PATH "/%s.p2"
#define FILE_R1_PATH BASE_PATH "/%s.r1"
#define FILE_H1_PATH BASE_PATH "/%s.h1"
int main()
{
    FILE *fp;
    unsigned char buf[1024] = {0};
    unsigned char file_path_buf[128] = {0};
    unsigned char h1[32] = {0};
    mpz_t p1, p2, r1;
    mpz_init(p1);
    mpz_init(p2);
    mpz_init(r1);

    sprintf(file_path_buf, FILE_P1_PATH, SOURCED_FILE_NAME);
    fp = fopen(file_path_buf, "r");
    mpz_inp_str(p1, fp, 10);
    fclose(fp);

    memset(file_path_buf, 0, sizeof(file_path_buf) / sizeof(file_path_buf[0]));
    sprintf(file_path_buf, FILE_P2_PATH, SOURCED_FILE_NAME);
    fp = fopen(file_path_buf, "r");
    mpz_inp_str(p2, fp, 10);
    fclose(fp);

    memset(file_path_buf, 0, sizeof(file_path_buf) / sizeof(file_path_buf[0]));
    sprintf(file_path_buf, FILE_R1_PATH, SOURCED_FILE_NAME);
    fp = fopen(file_path_buf, "r");
    mpz_inp_str(r1, fp, 10);
    fclose(fp);

    memset(file_path_buf, 0, sizeof(file_path_buf) / sizeof(file_path_buf[0]));
    sprintf(file_path_buf, FILE_H1_PATH, SOURCED_FILE_NAME);
    read_str_from_file(h1, sizeof(h1) / sizeof(h1[0]), file_path_buf, fp);

    mpz_add(r1, p1, r1);
    size_t countp;
    mpz_export(buf, &countp, 1, 1, 1, 0, r1);

    unsigned char hash_val[32];
    sha256(buf, countp, hash_val);

    if (memcmp(hash_val, h1, 32) == 0)
    {
        if (mpz_cmp(p1, p2) == 0)
            puts("Verifies.");
        else
            puts("Not verifies.");
    }
    else
        puts("Hash not match.");

    return 0;
}