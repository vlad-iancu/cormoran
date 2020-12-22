#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include "resolution/resolution.h"

int main() {
  //  printf("Started!\n");
    char *bin = (char*) malloc(sizeof(char) * 1024);
    scanf("%s", bin);
  //  printf("Read the filename\n");
    resolution_result result = get_bin(bin, "/home/iancu/Desktop");
    printf("%s",result.bin);
    return 0;
}
