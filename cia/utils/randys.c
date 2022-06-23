#include "randys.h"
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
    if ((fp = fopen(file_name, "r")) == NULL)
    {
        printf("Fail to open file *%s*\n", file_name);
        exit(0);
    }
    fread(_str, 1, _size, fp);
    fclose(fp);
}
void psd_permute(size_t *array, size_t n, unsigned int seed, size_t N)
{
    n = N > n ? n : N;
    srand(seed);
    size_t *_array = malloc(N * sizeof(size_t));
    for (size_t i = 0; i < N; i++)
        _array[i] = i;

    for (size_t i = 0; i < N - 1; i++)
    {
        size_t j = i + rand() / (RAND_MAX / (N - i) + 1);
        size_t t = _array[j];
        _array[j] = _array[i];
        _array[i] = t;
    }
    memcpy(array, _array, sizeof(size_t) * n);
    free(_array);
}

void psd_func(size_t *array, size_t n, unsigned int seed, size_t q)
{
    srand(seed);
    for (size_t i = 0; i < n; i++)
    {
        array[i] = rand() % q;
    }
}