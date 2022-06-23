#include "../utils/randys.h"
#include "../utils/sha256.h"
#include <time.h>
#define BASE_PATH "../swap"
// #define SOURCED_FILE_NAME "the-sun-also-rises.flac"
#define SOURCED_FILE_NAME "the-last-emperor.mkv"
#define DEFAULT_BLK_SIZE (1024 * 1024)
#define BLOCK_AMOUNT 5000
#define BLK_PATH BASE_PATH "/csp1/blocks/%s_blk_%ld"
#define FILE_INFO_PATH BASE_PATH "/%s.info"

int main() {
  clock_t start, finish;
  size_t block_size = DEFAULT_BLK_SIZE;
  FILE *fp_read, *fp_write;
  unsigned char masked_name[32] = {0};
  unsigned char sourced_file[128] = {0};
  unsigned char write_name[128] = {0};
  sprintf(sourced_file, "%s/%s", BASE_PATH, SOURCED_FILE_NAME);
  sprintf(masked_name, "%d", rand());
  fp_read = fopen(sourced_file, "r");
  size_t block_real_size = 0;
  unsigned char *file_block = malloc(block_size);
  size_t index = 0;
  start = clock();
  while ((block_real_size = fread(file_block, 1, block_size, fp_read)) > 0 && index < BLOCK_AMOUNT) {
    memset(write_name, 0, 128);
    sprintf(write_name, BLK_PATH, masked_name, index);
    write_str_to_file(file_block, block_real_size, write_name, fp_write);
    memset(file_block, 0, block_size);
    index++;
  }
  finish = clock();
  fclose(fp_read);

  memset(write_name, 0, 128);
  sprintf(write_name, FILE_INFO_PATH, SOURCED_FILE_NAME);
  fp_write = fopen(write_name, "w");
  fprintf(fp_write, "%ld\n", index);
  fprintf(fp_write, "%s", masked_name);
  fclose(fp_write);

  free(file_block);
  printf("%f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
  return 0;
}