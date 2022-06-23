#include "../utils/sha256_file.h"
#include <stdio.h>
#define SOURCED_FILE_PATH "../swap/the-sun-also-rises.flac"
int main()
{
    unsigned char result[1024] = {0};
    get_sha256(SOURCED_FILE_PATH, result);
    for (size_t ii = 0; ii < 1024 && result[ii] != 0; ii++)
    {
        fprintf(stdout, "%02x", result[ii]);
    }
    printf("\n");
}