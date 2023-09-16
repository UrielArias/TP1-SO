// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "slave.h"

 int main(){

    char * path = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    // Si se pasan 2 paths (esto podria pasar la primera vez que se usa el slave), 
    // deberian pasarse los paths uno abajo del otro por stdout 
    // De esta forma: "path1/npath2 "
    // Si se pasa solo un path deberia mandarse de esta forma: "path " 
    while ((linelen = getline(&path, &linecap, stdin)) > 0) {
        path[linelen-1] = '\0';
        char command[50];
        char result[160];
        sprintf(command, "md5sum %s", path);
        FILE * fp = popen(command, "r");

        if (fp == NULL) {
            fprintf(stderr, "There has been a problem making the md5 on file %s: %s\n", path, strerror(errno));
        }
        else {
            // Obtengo los primeros 32 caracteres, es decir, el hash, 
            // hay que hacer esto porque el md5 devuelve el hash + el nombre del archivo
            char hash_md5[HASH_LENGTH];
            if (fgets(hash_md5, HASH_LENGTH, fp) != NULL) {
                sprintf(result, "File name: %s - Md5: %s - Slave's pid: %d \n", path, hash_md5, getpid());
                write(1, result, strlen(result));
            }
            else {
                fprintf(stderr, "Error reading MD5 hash for file %s\n", path);
            }
        }
        pclose(fp);
    }
    free(path);
 }