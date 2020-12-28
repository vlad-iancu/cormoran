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
const int CMD_LENGTH = 4096;
const int MAX_CMDS = 4096;

int main() {
    /*static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int a = getchar();
    int b = getchar();
    int c = getchar();
    printf("%d %d %d", a, b, c);
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;*/
    char *command;
    char *dir = (char *) malloc(sizeof(char) * PATH_MAX);
    char *prompt = (char *)malloc(sizeof (char) * PATH_MAX);
    getcwd(dir, PATH_MAX);

    do {
        sprintf(prompt, "<#%s#cormoran>", dir);
        command = readline(prompt);
        if(strlen(command) > 0) {
            launch_command(command);
            add_history(command);
        }
        getcwd(dir, PATH_MAX);
    } while (strcmp(command, "exit") != 0);

    //The command history will be freed anyway at the end of program
    printf("\n");
    return 0;
}
