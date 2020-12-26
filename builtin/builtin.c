//
// Created by iancu on 25.12.2020.
//
#include "builtin.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
int resolve_builtin_command(char *command) {
    char *temp = (char*)malloc(strlen(command) * sizeof (char));
    strcpy(temp, command);
    char delim[] = " ";
    char * cmd = strtok(temp, delim);
    if(strcmp(cmd, "cd") == 0) {
        chdir(strtok(NULL, delim));
        return 1;
    }
    return 0;
}
