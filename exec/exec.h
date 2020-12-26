//
// Created by iancu on 23.12.2020.
//

#ifndef CORMORAN_EXEC_H
#define CORMORAN_EXEC_H


int execute(char *bin, char **args, int* read, int* write);
int launch_command(char *command);
char **read_piped_commands(char *command);
char **get_args(char *command);
#endif //CORMORAN_EXEC_H
