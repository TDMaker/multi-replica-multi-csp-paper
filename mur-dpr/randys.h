#ifndef psd_funcs_h
#define psd_funcs_h

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pbc/pbc.h>
#include "sha256.h"

void psd_permute(unsigned int *array, size_t n, unsigned int seed, unsigned int N);
void psd_func(long *array, size_t n, unsigned int seed, unsigned int q);
size_t str_to_int(unsigned char *ptr);
int sizefile(unsigned char *file_name);
int pairing_init(pairing_t pairing, unsigned char *pairing_param_file_name);
void write_str_to_file(unsigned char *_str, size_t _size, unsigned char *file_name, FILE *fp);
void read_str_from_file(unsigned char *_str, size_t _size, unsigned char *file_name, FILE *fp);
void H1(element_t result, char* source);
void H2(element_t result, element_t source);
void H3(element_t result, element_t sk_o2, char* name, size_t j);
void set_bid(char* bid, char* name, char* ID_O, size_t m_max, size_t j, char* h_i);
#endif /* psd_funcs_h */