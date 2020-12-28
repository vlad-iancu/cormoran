//
// Created by iancu on 23.12.2020.
//

#ifndef CORMORAN_EXEC_H
#define CORMORAN_EXEC_H

#define WRITE -2
#define APPEND -3
#define PIPE  -4
int launch_command(char *command);
char **read_piped_commands(char *command);
char **get_args(char *command);

typedef struct piped_commands {
    char **commands;
    int *symbols;
} piped_commands;

piped_commands *get_piped_commands(char *command);
#endif //CORMORAN_EXEC_H
