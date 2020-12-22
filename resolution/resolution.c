//
// Created by iancu on 22.12.2020.
//
#include "resolution.h"
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
const int MAX_EXECUTABLES_WD = 1024;
const int MAX_EXECUTABLES_HOME = 4096;
resolution_result resolution_error(char *msg);
resolution_result resolution_file(char *name);

resolution_result get_bin(char *name, char *wd) {
    char *basedir = "/opt/cormoran";
    DIR *d;

    d = opendir(wd);
    struct dirent *dir;
    while( (dir = readdir(d)) != NULL) {
        if(dir->d_type == DT_REG && strcmp(dir->d_name, name) == 0) {
            char *result = (char*)malloc(sizeof(char) * (strlen(wd) + strlen(name) + 1));
            strcpy(result, wd);
            strcat(result, "/");
            strcat(result, name);
            return resolution_file(result);
        }
    }

    d = opendir(basedir);
    if(d) {
        while( (dir = readdir(d)) != NULL) {
            if(dir->d_type == DT_REG && strcmp(dir->d_name, name) == 0) {
                char *result = (char*)malloc(sizeof(char) * (strlen(basedir) + strlen(name) + 1));
                strcpy(result, basedir);
                strcat(result, "/");
                strcat(result, name);
                return resolution_file(result);
            }
        }
    }
    else {
        return resolution_error("Make sure Cormoran is installed in the /opt/cormoran directory");
    }


    return resolution_error("Could not find the requested file");
}
resolution_result resolution_file(char *name) {
    resolution_result result;
    result.msg = "Program resolution success";
    result.bin = name;
    return result;
}
resolution_result resolution_error(char *msg) {
    resolution_result result;
    result.bin = NULL;
    result.err = 1;
    result.msg = msg;
    return result;
}

