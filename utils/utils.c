//
// Created by iancu on 24.12.2020.
//
#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <termios.h>

void replace_string(char *str, size_t old_len) {
    for(int i = 0; i < old_len; i++) {
        printf("\b");
        printf(" ");
        printf("\b");
    }
    printf("%s", str);
}