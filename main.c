#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "resolution/resolution.h"
#include <unistd.h>


int main() {
    printf("$");
    char *bin = NULL;
    size_t len = 0;
    getline(&bin, &len, stdin);
    bin[strlen(bin) - 1] = '\0';
    len--;
    resolution_result result = get_bin(bin, "/home/iancu/Desktop");
    printf("%s", result.bin);

    return 0;
}
