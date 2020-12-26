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

int execute_piped(char **piped_commands, int **fds, int i) {
    if (piped_commands[i] == NULL) {
        //  printf("Stopping at i = %d\n", i);
        return 0;
    }
    pid_t child = fork();
    if (child == 0) {
        int rfd = -1;
        int wfd = -1;
        int read = 0;
        int write = 0;
        if (i != 0) {
            rfd = fds[i - 1][0];
            read = 1;
        }
        if (fds[i] != NULL) {
            wfd = fds[i][1];
            write = 1;
        }
        if (read) {
            //     printf("Attaching stdin to pipe\n");
            dup2(rfd, STDIN_FILENO);
            close(rfd);

        }
        if (write) {
            //     printf("Attaching stdout to pipe\n");
            dup2(wfd, STDOUT_FILENO);
            close(wfd);

        }
        // printf("We have this entire command:%s",piped_commands[i]);
        char **args = get_args(piped_commands[i]);
        // printf("I got these arguments from parent:\n");
        int j;
        /*for( j = 0;args[j] != NULL;j++) {
            printf("%d:%s ",j, args[j]);
        }
        if(args[j] == NULL) {
            printf("NULL");
        }
        printf("\n");
        printf("Executing fork for i = %d\n", i);*/
        execvp(args[0], args);
    } else if (child < 0) return -1;
    else if (child > 0) {
        execute_piped(piped_commands, fds, i + 1);
    }
}

int launch_command(char *command) {
    if (isspace(command[strlen(command) - 1])) {
        command[strlen(command) - 1] = '\0';
    }
    if (resolve_builtin_command(command)) return 0;
    //char **cmdArgs = get_args(command);
    char **piped_commands = read_piped_commands(command);
    char *cmd;
    int **fds = (int **) malloc(sizeof(int *) * MAX_COMMANDS);
    int piped_count = 0;
    while (piped_commands[piped_count + 1] != NULL) {
        fds[piped_count] = (int *) malloc(2 * sizeof(int));
        pipe(fds[piped_count]);
        piped_count++;
    }
    fds[piped_count] = NULL;
    execute_piped(piped_commands, fds, 0);

    for (int i = 0; piped_commands[i] != NULL; i++) {
        wait(NULL);
    }
    for (int i = 0; i < piped_count; i++) {
        close(fds[i][0]);
        close(fds[i][1]);
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
