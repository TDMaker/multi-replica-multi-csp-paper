#include "randys.h"
// https://benpfaff.org/writings/clc/shuffle.html
/* Arrange the N elements of ARRAY in random order.
   Only effective if N is much smaller than RAND_MAX;
   if this may not be the case, use a better random
   number generator. */
void psd_permute(unsigned int *array, size_t n, unsigned int seed, unsigned int N)
{
    n = N > n ? n : N;
    srand(seed);
    unsigned int *_array = malloc(N * sizeof(unsigned int));
    for (size_t i = 0; i < N; i++)
        _array[i] = i;

    for (size_t i = 0; i < N - 1; i++)
    {
        size_t j = i + rand() / (RAND_MAX / (N - i) + 1);
        unsigned int t = _array[j];
        _array[j] = _array[i];
        _array[i] = t;
    }
    memcpy(array, _array, sizeof(unsigned int) * n);
    free(_array);
}

void psd_func(long *array, size_t n, unsigned int seed, unsigned int q)
{
    srand(seed);
    for (size_t i = 0; i < n; i++)
    {
        array[i] = rand() % q;
    }
}
size_t str_to_int(unsigned char *ptr)
{
    size_t intg = 0;
    for (unsigned char *p = ptr; *p != '\0'; p++)
    {
        if (*p < '0' || *p > '9')
        {
            printf("%s is not a decimal format!\n", ptr);
            exit(0);
        }
        intg *= 10;
        intg += (*p - '0');
    }
    return intg;
}

int sizefile(unsigned char *file_name)
{
    FILE *fp = NULL;
    if ((fp = fopen(file_name, "r")) == NULL)
    {
        printf("Fail to open file *%s*\n", file_name);
        return -1;
    }
    int sizef = 0;
    while (!feof(fp))
    {
        fgetc(fp);
        sizef++;
    }
    fclose(fp);
    return sizef - 1;
}
int pairing_init(pairing_t pairing, unsigned char *pairing_param_file_name)
{
    FILE *fp;
    char param[1024];
    if ((fp = fopen(pairing_param_file_name, "r")) == NULL)
    {
        printf("Pairing param file *%s* open error!\n", pairing_param_file_name);
        return -1;
    }
    size_t count = fread(param, 1, 1024, fp);
    fclose(fp);
    if (!count)
    {
        puts("pairing param file read failed!");
        return -2;
    }
    pairing_init_set_buf(pairing, param, count);
    return 0;
}

void write_str_to_file(unsigned char *_str, size_t _size, unsigned char *file_name, FILE *fp)
{
    if ((fp = fopen(file_name, "w")) == NULL)
    {
        printf("Fail to open file *%s*\n", file_name);
        exit(0);
    }
    fwrite(_str, 1, _size, fp);
    fclose(fp);
}
void read_str_from_file(unsigned char *_str, size_t _size, unsigned char *file_name, FILE *fp)
{
    if ((fp = fp = fopen(file_name, "r")) == NULL)
    {
        printf("Fail to open file *%s*\n", file_name);
        exit(0);
    }
    int _ = fread(_str, 1, _size, fp);
    fclose(fp);
}

void H1(element_t result, char* source)
{
    element_from_bytes(result, source);
}

void H2(element_t result, element_t source)
{
    unsigned char *data = malloc(65);
    element_to_bytes_compressed(data, source);
    // element_from_hash(result, buf, sizeof(buf));
}
void H3(element_t result, element_t sk_o2, char* name, size_t j)
{
    char str[48] = {0};
    char str_j[3] = {0};
    sprintf(str_j, "%ld", j);
    strcpy(str, name);
    strcat(str, str_j);
    element_from_bytes(result, str);
    element_add(result, result , sk_o2);
}
void set_bid(char* bid, char* name, char* ID_O, size_t m_max, size_t j, char* h_i)
{
    char str[3] = {0};
    strcpy(bid, name);
    strcat(bid, ID_O);
    sprintf(str, "%ld", m_max);
    strcat(bid, str);
    memset(str, 0, 3);
    sprintf(str, "%ld", j);
    strcat(bid, str);
    memcpy(bid + strlen(bid), h_i, 32);
}