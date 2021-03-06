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
#include "../utils/utils.h"

const int MAX_ARGS = 1024;
const int MAX_COMMANDS = 256;
const int CMD_LENGTH = 4096;

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
    if (resolve_builtin_command(command)) return 0;
    char *copy_cmd = strdup(command);
    piped_commands *p_commands = get_piped_commands(copy_cmd);
    free(copy_cmd);
    int fds[2];
    int rfd = 0;
    int i = 0;
    int result = 0;
    if (p_commands->commands[1] != NULL)
        while (p_commands->commands[i] != NULL) {
            pipe(fds);
            FILE *file = NULL;
            int out = 1;
            if (p_commands->commands[i + 1] == NULL) {
                out = 1;
            } else if (p_commands->symbols[i] == PIPE) {
                out = fds[1];
            } else if (p_commands->symbols[i] == WRITE) {
                file = fopen(trim_space(p_commands->commands[i + 1]), "w");
                out = fileno(file);
            } else if (p_commands->symbols[i] == APPEND) {
                file = fopen(trim_space(p_commands->commands[i + 1]), "a");
                out = fileno(file);
            }
            copy_cmd = strdup(p_commands->commands[i]);
            char **args = get_args_spaced(trim_space(copy_cmd));
            int res = execute_piped(p_commands->commands[i], args, rfd, out);
            if (res) result = 1;
            close(fds[1]);
            if (file != NULL) {
                fclose(file);
            }
            rfd = fds[0];
            if (p_commands->symbols[i] == WRITE || p_commands->symbols[i] == APPEND) {
                i++;
            }
            i++;
            free(copy_cmd);
            for (int i = 0; args[i] != NULL; i++) free(args[i]);
            free(args);
        }
    else {
        copy_cmd = strdup(command);
        char **args = get_args_spaced(trim_space(copy_cmd));
        execute_piped(command, args, 0, 1);
        for (int i = 0; args[i] != NULL; i++) free(args[i]);
        free(copy_cmd);
        free(args);
    }
    i = 0;
    while (p_commands->commands[i] != NULL) {
        if (i == 0 || p_commands->symbols[i - 1] != WRITE)
            wait(NULL);
        i++;
    }
    for (int k = 0; p_commands->commands[k] != NULL; k++) {
        free(p_commands->commands[k]);
    }
    free(p_commands->symbols);
    free(p_commands->commands);
    free(p_commands);
    return result;
}

piped_commands *get_piped_commands(char *command) {
    piped_commands *commands = (piped_commands *) malloc(sizeof(piped_commands));
    commands->symbols = (int *) malloc(MAX_COMMANDS * sizeof(int));
    commands->commands = (char **) malloc(MAX_COMMANDS * sizeof(char *));
    char delim[] = "|>";
    char *buf = (char *) malloc(sizeof(char) * CMD_LENGTH);
    int k_buf = 0;
    int k_sym = 0;
    int k_command = 0;
    for (int i = 0; command[i] != '\0'; i++) {
        if (strchr(delim, command[i]) == NULL) {
            buf[k_buf] = command[i];
            k_buf++;
        } else {
            switch (command[i]) {
                case '|': {
                    commands->symbols[k_sym] = PIPE;
                }
                    break;
                case '>': {
                    if (command[i + 1] == '>') {
                        commands->symbols[k_sym] = APPEND;
                        i++;
                    } else
                        commands->symbols[k_sym] = WRITE;
                }
                    break;
            }
            k_sym++;
            buf[k_buf] = '\0';
            trim_space(buf);
            commands->commands[k_command] = strdup(buf);
            k_command++;
            memset(buf, '\0', sizeof buf);
            k_buf = 0;
        }
    }
    k_sym++;
    buf[k_buf] = '\0';
    trim_space(buf);
    commands->commands[k_command] = strdup(buf);
    commands->commands[k_command + 1] = NULL;
    free(buf);
    return commands;
}

char **get_chained_commands(char *command) {
    size_t len = strlen(command);
    char **chained_commands = (char **) malloc(sizeof(char *) * MAX_COMMANDS);
    char *buf = (char *) malloc(sizeof(char) * CMD_LENGTH);
    int k_buf = 0;
    int k_command = 0;
    for (int i = 0; i < len; i++) {
        if (command[i] == '&' && command[i + 1] == '&') {
            chained_commands[k_command] = (char *) malloc(sizeof(char) * CMD_LENGTH);
            trim_space(buf);
            strcpy(chained_commands[k_command], buf);
            memset(buf, '\0', CMD_LENGTH);
            k_command++;
            i++;
            k_buf = 0;
        } else {
            buf[k_buf] = command[i];
            k_buf++;
        }
    }
    chained_commands[k_command] = (char *) malloc(sizeof(char) * CMD_LENGTH);
    buf[k_buf] = '\0';
    strcpy(chained_commands[k_command], buf);
    free(buf);
    chained_commands[k_command + 1] = NULL;
    return chained_commands;

}

char **get_args(char *command) {
    char delim[] = " ";
    char **args = (char **) malloc(MAX_ARGS * sizeof(char *));
    char *arg = strtok(command, delim);
    int argc = 1;
    args[0] = arg;
    while ((arg = strtok(NULL, delim)) != NULL) {
        args[argc] = arg;
        argc++;
    }
    args[argc] = NULL;
    return args;
}

char **get_args_spaced(char *command) {
    char **args = (char **) malloc(MAX_ARGS * sizeof(char *));
    int argc = 0;
    int argl = 0;
    args[0] = (char *) malloc(CMD_LENGTH * sizeof(char));
    size_t len = strlen(command);

    int quotes = 0;
    for (int i = 0; i < len; i++) {
        char ch = command[i];
        if (isspace(ch) && !quotes) {
            args[argc][argl] = '\0';
            argc++;
            argl = 0;
            args[argc] = (char *) malloc(CMD_LENGTH * sizeof(char));
        } else if (ch == '\"') {
            quotes = (quotes + 1) % 2;
        } else {
            args[argc][argl] = ch;
            argl++;
        }
    }
  /*  int i;
    for(i = 0;args[i] != NULL && strlen(args[i]) != 0;i++);
    args[i] = NULL;*/
    args[argc][argl] = '\0';
    args[argc + 1] = NULL;
    return args;
}
