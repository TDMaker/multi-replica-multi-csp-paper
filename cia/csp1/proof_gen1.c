#include "../utils/randys.h"
#include "../utils/sha256.h"
#include <gmp.h>
#include <time.h>
#define BASE_PATH "../swap"
#define DEFAULT_BLK_SIZE (1024 * 1024)
// #define SOURCED_FILE_NAME "the-sun-also-rises.flac"
#define SOURCED_FILE_NAME "the-last-emperor.mkv"
#define BLK_PATH BASE_PATH "/csp1/blocks/%s_blk_%ld"
#define FILE_R1_PATH BASE_PATH "/%s.r1"
#define FILE_R2_PATH BASE_PATH "/%s.r2"
#define FILE_P1_PATH BASE_PATH "/%s.p1"
#define FILE_H1_PATH BASE_PATH "/%s.h1"
#define SECURITY_LEVEL 160
#define CHAL_AMOUNT 500
int main() {
  clock_t start, finish;
  FILE *fp;
  unsigned char buf[1024] = {0};
  unsigned char masked_name[32] = {0};
  unsigned char file_path_buf[128] = {0};
  size_t block_size = DEFAULT_BLK_SIZE;
  size_t blk_amount = 0;
  mpz_t r1, r2, im_r, r_sum, im_sum, blk_sum;
  gmp_randstate_t state;
  mpz_init(r1);
  mpz_init(r2);
  mpz_init(im_r);
  mpz_init(r_sum);
  mpz_init(im_sum);
  mpz_init(blk_sum);
  gmp_randinit_mt(state);
  size_t seed;
  read_str_from_file((unsigned char *)&seed, sizeof(size_t), "/dev/urandom", fp);

  gmp_randseed_ui(state, seed);
  mpz_urandomb(r1, state, SECURITY_LEVEL);

  sprintf(file_path_buf, FILE_R1_PATH, SOURCED_FILE_NAME);
  fp = fopen(file_path_buf, "w");
  mpz_out_str(fp, 10, r1);
  fclose(fp);

  memset(file_path_buf, 0, sizeof(file_path_buf) / sizeof(file_path_buf)[0]);
  sprintf(file_path_buf, FILE_R2_PATH, SOURCED_FILE_NAME);
  puts(file_path_buf);
  fp = fopen(file_path_buf, "r");
  mpz_inp_str(r2, fp, 10);
  fclose(fp);

  mpz_add(r_sum, r1, r2);
  size_t r_long = mpz_get_ui(r_sum);
  printf("Random number is %lu\n", r_long);

  memset(file_path_buf, 0, sizeof(file_path_buf) / sizeof(file_path_buf)[0]);
  sprintf(file_path_buf, "%s/%s.info", BASE_PATH, SOURCED_FILE_NAME);
  fp = fopen(file_path_buf, "r");
  fgets(buf, sizeof(buf) / sizeof(buf[0]), fp);
  blk_amount = atoi(buf);
  fgets(masked_name, sizeof(masked_name), fp);
  fclose(fp);

  gmp_randseed(state, r_sum);
  size_t *u_s = (size_t *)malloc(sizeof(size_t) * CHAL_AMOUNT);
  psd_permute(u_s, CHAL_AMOUNT, (unsigned int)r_long, blk_amount);

  unsigned char *blk_buf =
      (unsigned char *)malloc(sizeof(unsigned char) * block_size);
  start = clock();
  for (size_t i = 0; i < CHAL_AMOUNT; i++) {
    memset(file_path_buf, 0, sizeof(file_path_buf) / sizeof(file_path_buf)[0]);
    sprintf(file_path_buf, BLK_PATH, masked_name, u_s[i]);
    memset(blk_buf, 0, sizeof(unsigned char) * block_size);
    read_str_from_file(blk_buf, block_size, file_path_buf, fp);
    mpz_import(im_sum, block_size, 1, sizeof(unsigned char), 0, 0, blk_buf);
    mpz_urandomb(im_r, state, SECURITY_LEVEL);
    mpz_mul(im_sum, im_sum, im_r);
    mpz_mod(im_sum, im_sum, r_sum);
    mpz_add(blk_sum, blk_sum, im_sum);
  }
  finish = clock();
  mpz_mod(blk_sum, blk_sum, r_sum);
  sprintf(file_path_buf, FILE_P1_PATH, SOURCED_FILE_NAME);
  fp = fopen(file_path_buf, "w");
  mpz_out_str(fp, 10, blk_sum);
  fclose(fp);

  memset(buf, 0, sizeof(buf) / sizeof(buf)[0]);
  mpz_add(blk_sum, blk_sum, r1);
  size_t countp;
  mpz_export(buf, &countp, 1, 1, 1, 0, blk_sum);

  unsigned char hash_val[32];
  sha256(buf, countp, hash_val);
  memset(file_path_buf, 0, sizeof(file_path_buf) / sizeof(file_path_buf[0]));
  sprintf(file_path_buf, FILE_H1_PATH, SOURCED_FILE_NAME);
  write_str_to_file(hash_val, sizeof(hash_val) / sizeof(hash_val[0]),
                    file_path_buf, fp);

  free(u_s);
  free(blk_buf);
  mpz_clear(r1);
  mpz_clear(r2);
  mpz_clear(im_r);
  mpz_clear(r_sum);
  mpz_clear(im_sum);
  mpz_clear(blk_sum);
  printf("%f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
  return 0;
}