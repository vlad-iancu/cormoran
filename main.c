#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "resolution/resolution.h"
#include "exec/exec.h"
#include <unistd.h>
#include <ctype.h>

const int MAX_ARGS = 1024;
int main() {
    /*printf("$");
    char *bin = NULL;
    size_t len = 0;
    getline(&bin, &len, stdin);
    bin[strlen(bin) - 1] = '\0';
    len--;
    resolution_result result = get_bin(bin, "/home/iancu/Desktop");
    printf("%s", result.bin);*/
    char *command = NULL;
    size_t cmdlen;
    do {
        printf("$:");
        getline(&command, &cmdlen, stdin);
        cmdlen = strlen(command);
        if(isspace(command[cmdlen - 1]))
            command[cmdlen - 1] = '\0';
        char delim[] = " ";
        if(strlen(command) > 0) {
            char *cname = strtok(command, delim);
            char **args = (char**)malloc( sizeof(char*) * MAX_ARGS);
            args[0] = cname;
            char *arg = strtok(NULL, delim);
            int argi = 1;
            while(arg != NULL) {
                args[argi] = arg;
                arg = strtok(NULL, delim);
                argi++;
            }
            args[argi] = NULL;
            execute(cname, args);
        }
        else
            continue;

    } while (strcmp(command, "exit") != 0);

    return 0;
}
