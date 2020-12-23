//
// Created by iancu on 23.12.2020.
//
#include "exec.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
int execute(char *bin,char **args) {
    pid_t child = fork();
    if(child == 0) {
        execvp(bin, args);
    }
    else
        if(child < 0) {
            return -1;
        }
        else {
            wait(NULL);
            return 0;
        }
}
