//
// Created by iancu on 24.12.2020.
//

#ifndef CORMORAN_UTILS_H
#define CORMORAN_UTILS_H

#define UP_ARROW -1
#define DOWN_ARROW -2
#define LEFT_ARROW -3
#define RIGHT_ARROW -4
#define BACKSPACE  -5

#include <stdlib.h>

#define cursorforward(x) printf("\033[%dC", (x))
#define cursorbackward(x) printf("\033[%dD", (x))

void clear();
int getchr();

#endif //CORMORAN_UTILS_H
