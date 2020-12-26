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

int execute(char *bin, char **args, int *read, int *write) {
    pid_t child = fork();
    if (child == 0) {
        if (read[0]) {
            close(write[1]);
            dup2(read[1], STDIN_FILENO);
        }
        if (write[0]) {
            close(read[1]);
            dup2(write[1], STDOUT_FILENO);
        }
        if (!resolve_builtin_command(bin)) {
            execvp(bin, args);
        }
    } else if (child < 0) {
        return -1;
    } else {
        wait(NULL);
        return 0;
    }
}

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
    if (resolve_builtin_command(command)) return 0;
    //char **cmdArgs = get_args(command);
    char **piped_commands = read_piped_commands(command);
    int fds[2];
    int rfd = 0;
    int i = 0;
    if (piped_commands[1] != NULL)
        while (piped_commands[i] != NULL) {
            pipe(fds);
            int out = piped_commands[i+1] == NULL ? 1 : fds[1];
            execute_piped(piped_commands[i], get_args(piped_commands[i]), rfd, out);
            close(fds[1]);
            rfd = fds[0];
            i++;
        }
    else execute_piped(command, get_args(command), 0, 1);
    i = 0;
    while(piped_commands[i] != NULL) {
        wait(NULL);
        i++;
    }
    return 0;
}

/*if (isspace(command[cmdlen - 1]))
        command[cmdlen - 1] = '\0';
    char delim[] = " ";
    if (strlen(command) > 0) {
        char *cname = strtok(command, delim);
        char **args = (char **) malloc(sizeof(char *) * MAX_ARGS);
        args[0] = cname;
        char *arg = strtok(NULL, delim);
        int argi = 1;
        while (arg != NULL) {
            args[argi] = arg;
            arg = strtok(NULL, delim);
            argi++;
        }
        args[argi] = NULL;
        int result = execute(cname, args, NULL, 0, 0);
        free(args);
        return result;
    } else
        return 0;*/

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
    char *arg = strtok(command, delim);
    args[0] = arg;
    while ((arg = strtok(NULL, delim)) != NULL) {
        args[argc] = arg;
        argc++;
    }
    args[argc] = NULL;
    free(cmd);
    return args;
}
