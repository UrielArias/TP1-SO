// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main (int argc, char * argv []) {
    if (argc != 2) {
        if (argc > 2) 
            fprintf(stderr, "Too many arguments\n");
        else
            fprintf(stderr, "There is one missing argument\n");
        exit(EXIT_FAILURE);
    }

    DIR * dir = opendir(argv[1]);

    if(dir == NULL) {
        fprintf(stderr, "Directory %s is not valid\n", argv[1]);
        exit(EXIT_FAILURE);
    }
}

