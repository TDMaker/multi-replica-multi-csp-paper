#include "../utils/randys.h"
#include <gmp.h>
#define BASE_PATH "../swap"
// #define SOURCED_FILE_NAME "the-sun-also-rises.flac"
#define SOURCED_FILE_NAME "the-last-emperor.mkv"
#define FILE_TMP_PATH BASE_PATH "/%s.r2"
#define SECURITY_LEVEL 160
int main() {
  FILE *fp;
  unsigned char file_path_buf[128] = {0};
  mpz_t r2;
  mpz_init(r2);
  gmp_randstate_t state;
  gmp_randinit_mt(state);
  size_t seed;
  read_str_from_file((unsigned char*)&seed, sizeof(size_t), "/dev/urandom", fp);

  printf("%lu\n", seed);

  gmp_randseed_ui(state, seed);
  mpz_urandomb(r2, state, SECURITY_LEVEL);

  sprintf(file_path_buf, FILE_TMP_PATH, SOURCED_FILE_NAME);
  fp = fopen(file_path_buf, "w");
  puts(file_path_buf);
  mpz_out_str(fp, 10, r2);
  mpz_clear(r2);
  return 0;
}