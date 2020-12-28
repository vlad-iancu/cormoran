//
// Created by iancu on 24.12.2020.
//
#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <termios.h>

void clear() {
    printf("\033[H\033[J");
}

int getchr() {
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int c = getchar();
    if (c == 27) {
        c = getchar();
        if (c == 91) {
            c = getchar();
            if (c == 65) {
                c = UP_ARROW;
            } else if (c == 66) {
                c = DOWN_ARROW;
            }
            else if(c == 67) {
                c = RIGHT_ARROW;
            }
            else if(c == 68) {
                c = LEFT_ARROW;
            }
        }
    } else if (c == 127) {
        c = BACKSPACE;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}

void replace_string(char *str, size_t old_len) {
    for(int i = 0; i < old_len; i++) {
        printf("\b");
        printf(" ");
        printf("\b");
    }
    printf("%s", str);
}