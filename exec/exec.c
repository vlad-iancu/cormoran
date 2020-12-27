//
// Created by iancu on 23.12.2020.
//
#include "exec.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../builtin/builtin.h"

const int MAX_ARGS = 1024;
const int MAX_COMMANDS = 256;

int execute_piped(char *command, char **args, int in, int out) {

    pid_t child;
    child = fork();
    if (child == 0) {
        if (in != 0) {
            dup2(in, 0);
            close(in);
        }
        if (out != 1) {
            dup2(out, 1);
            close(out);
        }
        if (resolve_builtin_command(command)) return 0;
        else
            return execvp(args[0], args);
    }
    return child;
}

int launch_command(char *command) {
    if (isspace(command[strlen(command) - 1])) {
        command[strlen(command) - 1] = '\0';
    }
    // cd is even more special than the other special commands since it
    // cannot be executed inside fork (which means it cannot be piped)
    if(resolve_builtin_command(command)) return 0;

    char **piped_commands = read_piped_commands(command);
    int fds[2];
    int rfd = 0;
    int i = 0;
    if (piped_commands[1] != NULL)
        while (piped_commands[i] != NULL) {
            pipe(fds);
            int out = piped_commands[i+1] == NULL ? 1 : fds[1];
            char **args = get_args(piped_commands[i]);
            execute_piped(piped_commands[i], args, rfd, out);
            close(fds[1]);
            rfd = fds[0];
            i++;
            free(args);
        }
    else {
        char **args = get_args(command);
        execute_piped(command, get_args(command), 0, 1);
        free(args);
    }
    i = 0;
    while(piped_commands[i] != NULL) {
        wait(NULL);
        i++;
    }
    return 0;
}

char **read_piped_commands(char *command) {
    char delim[] = "|";
    char **piped_commands = (char **) malloc(sizeof(char *) * MAX_COMMANDS);
    int piped_count = 0;
    char *cmd = (char *) malloc(sizeof(char) * strlen(command));
    strcpy(cmd, command);
    char *program = NULL;
    program = strtok(cmd, delim);
    do {
        piped_commands[piped_count] = (char *) malloc(strlen(program) * sizeof(char));
        strcpy(piped_commands[piped_count], program);
        piped_count++;
    } while ((program = strtok(NULL, delim)) != NULL);
    piped_commands[piped_count] = NULL;
    free(cmd);
    return piped_commands;
}

char **get_args(char *command) {
    char *cmd = (char *) malloc(strlen(command) * sizeof(char));
    strcpy(cmd, command);

    char **args = (char **) malloc(MAX_ARGS * sizeof(char *));
    int argc = 1;
    char delim[] = " ";
    char *arg = strtok(cmd, delim);
    args[0] = arg;
    while ((arg = strtok(NULL, delim)) != NULL) {
        args[argc] = arg;
        argc++;
    }
    args[argc] = NULL;
    return args;
}
