//
// Created by iancu on 22.12.2020.
//

#ifndef CORMORAN_RESOLUTION_H
#define CORMORAN_RESOLUTION_H

typedef struct resolution_result {
    char *bin;
    int err;
    char *msg;
} resolution_result;
resolution_result get_bin(char* name, char *wd);
#endif //CORMORAN_RESOLUTION_H
