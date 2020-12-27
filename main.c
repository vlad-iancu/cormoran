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
        int l = strlen(command);
        for(int i = 0;i < l;i++) {
            command[i] = '\0';
        }
        clen = 0;
        printf("<#%s#cormoran>", dir);
        fflush(stdout);
        //int clearOutput = 1;
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
                    clear();
                    printf("<#%s#cormoran>%s", dir, command);
                    fflush(stdout);
                    clen = strlen(command);
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
                fflush(stdout);
                command[clen] = (char) c;
                printf("%c", c);
                clen++;
                cursor++;
            }
        } while (c != '\n');
        if (isspace(command[0])) continue;
        commandHistory[cmdCount] = (char *) malloc(sizeof(char) * CMD_LENGTH);
        if (isspace(command[strlen(command) - 1])) {
            command[strlen(command) - 1] = '\0';
        }
        strcpy(commandHistory[cmdCount], command);
        cmdCount++;
        cmdIndex = cmdCount;
        if (strcmp(command, "history") == 0) {
            for (int i = 0; i < cmdCount; i++) {
                printf("%s\n", commandHistory[i]);
            }
        } else
            launch_command(command);
        int dirlen = strlen(dir);
        for (int i = 0; i < dirlen; i++) {
            dir[i] = '\0';
        }
        getcwd(dir, PATH_MAX);
    } while (strcmp(command, "exit") != 0);

    //The command history will be freed anyway at the end of program
    printf("\n");
    return 0;
}
