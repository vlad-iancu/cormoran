//
// Created by iancu on 24.12.2020.
//
#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <ctype.h>

void replace_string(char *str, size_t old_len) {
    for(int i = 0; i < old_len; i++) {
        printf("\b");
        printf(" ");
        printf("\b");
    }
    printf("%s", str);
}

char *trim_space(char *str) {
    size_t len = strlen(str);
    for(int i = len - 1;i > 0 && isspace(str[i]);i--) {
        str[i] = '\0';
    }
    int k = 0;
    while(isspace(str[k])) k++;
    return str + k;
}