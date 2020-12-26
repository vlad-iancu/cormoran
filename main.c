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
    char **commandHistory = (char **) malloc(sizeof(char *) * MAX_CMDS);
    char *command = (char *) malloc(sizeof(char) * CMD_LENGTH);
    char *dir = (char *) malloc(sizeof(char) * PATH_MAX);
    int cmdIndex = 0;
    int cmdCount = 0;
    size_t clen = 0;
    FILE *log = fopen("log.txt", "w");
    fprintf(log, "Begin logging\n");
    fflush(log);
    getcwd(dir, PATH_MAX);
    int cursor = 0;
    do {
        memset(command, '\0', clen);
        clen = 0;
        printf("<#%s#cormoran>", dir);
        fflush(stdout);
        command = (char *) malloc(sizeof(char) * CMD_LENGTH);
        int clearOutput = 1;
        int c;
        do {
            c = getchr();

            if (c == UP_ARROW) {
                if (cmdIndex > 0) {
                    cmdIndex--;
                    for (int i = 0; i < clen; i++) {
                        command[i] = '\0';
                    }
                    strcpy(command, commandHistory[cmdIndex]);
                    fprintf(log, "Command is now '%s' after up was pressed\n", command);
                    clear();
                    printf("<#%s#cormoran>%s", dir, command);
                    fflush(stdout);
                    clen = strlen(command);
                    fflush(log);
                    cursor = clen;
                    continue;
                }
            } else if (c == DOWN_ARROW) {
                if (cmdIndex < cmdCount - 1) {
                    cmdIndex++;
                    for (int i = 0; i < clen; i++) {
                        command[i] = '\0';
                    }
                    strcpy(command, commandHistory[cmdIndex]);
                    clear();
                    printf("<#%s#cormoran>%s", dir, command);
                    fflush(stdout);
                    clen = strlen(command);
                    cursor = clen;
                    continue;
                }
            } else if (c == BACKSPACE) {
                if (clen > 0) {
                    command[clen] = '\0';
                    clen--;
                    printf("\b");
                    printf(" ");
                    printf("\b");
                    fprintf(log, "Deleted a character");
                    fflush(log);
                    fflush(stdout);
                    fflush(stdin);
                }

            } else if (c == LEFT_ARROW) {
                if (cursor > 0) {
                    cursorbackward(1);
                    cursor--;
                }
            } else if (c == RIGHT_ARROW) {
                if (cursor < clen) {
                    cursorforward(1);
                    cursor++;
                }

            } else {
                if (clearOutput) {
                    clear();
                    printf("<#%s#cormoran>", dir);
                    fflush(stdout);
                }
                command[clen] = (char) c;
                printf("%c", c);
                clen++;
                cursor++;
            }
            clearOutput = 0;

        } while (c != '\n');
        commandHistory[cmdCount] = (char *) malloc(sizeof(char) * CMD_LENGTH);
        int count = isspace(command[strlen(command) - 1]) ? strlen(command) - 1 : strlen(command);
        if (isspace(command[strlen(command) - 1])) {
            command[strlen(command) - 1] = '\0';
            fprintf(log, "We have removed the whitespace at the end of the command\n");
        }
        strcpy(commandHistory[cmdCount], command);
        cmdCount++;
        cmdIndex = cmdCount;
        fprintf(log, "We are executing command '%s'\n", command);
        fflush(log);
        if(strcmp(command, "history") == 0) {
            for(int i = 0;i < cmdCount;i++) {
                printf("%s\n",commandHistory[i]);
            }
        } else
        launch_command(command);
        int dirlen = strlen(dir);
        for(int i = 0;i < dirlen; i++) {
            dir[i] = '\0';
        }
        getcwd(dir, PATH_MAX);
    } while (strcmp(command, "exit") != 0);

    //The command history will be freed anyway at the end of program
    printf("\n");
    return 0;
}
