#ifndef randys_h
#define randys_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void write_str_to_file(unsigned char *_str, size_t _size, unsigned char *file_name, FILE *fp);
void read_str_from_file(unsigned char *_str, size_t _size, unsigned char *file_name, FILE *fp);
void psd_permute(size_t *array, size_t n, unsigned int seed, size_t N);
void psd_func(size_t *array, size_t n, unsigned int seed, size_t q);
#endif /* randys_h */