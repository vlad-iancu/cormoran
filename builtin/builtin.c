//
// Created by iancu on 25.12.2020.
//
#include "builtin.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
int resolve_builtin_command(char *command) {
    char *temp = (char*)malloc(strlen(command) * sizeof (char));
    strcpy(temp, command);
    char delim[] = " ";
    char * cmd = strtok(temp, delim);
    if(strcmp(cmd, "cd") == 0) {
        chdir(strtok(NULL, delim));
        return 1;
    }
    if(strcmp(cmd, "pwd") == 0) {
        char *dir = malloc(PATH_MAX * sizeof (char));
        getcwd(dir,PATH_MAX);
        printf("%s\n", dir);
        free(dir);
        return 1;
    }
    free(temp);
    return 0;
}
