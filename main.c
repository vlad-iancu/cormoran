#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "exec/exec.h"
#include "utils/utils.h"
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <termios.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>

//const int MAX_ARGS = 1024;
const int MAX_CMDS = 4096;

int main() {
    /*char **args = get_args_spaced("git commit -m \"Spaced args\"");
    return 0;*/
    char *command;
    char *dir = (char *) malloc(sizeof(char) * PATH_MAX);
    char *prompt = (char *) malloc(sizeof(char) * PATH_MAX);
    getcwd(dir, PATH_MAX);

    do {
        sprintf(prompt, "<#%s#cormoran>", dir);
        command = readline(prompt);
        fflush(stdout);
        if (strlen(command) > 0) {
            //launch_command(command);
            char **commands = get_chained_commands(command);
            int fail = 0;
            for (int i = 0; commands[i] != NULL; i++){
                if(!fail && launch_command(trim_space(commands[i]))) {
                    fail = 1;
                }
                free(commands[i]);
            }
            free(commands);
            add_history(command);
        }
        getcwd(dir, PATH_MAX);
    } while (strcmp(command, "exit") != 0);

    //The command history will be freed anyway at the end of program
    printf("\n");
    return 0;
}
