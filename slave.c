// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "slave.h"

 int main(){

    char * path = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    char message[MSG_SIZE] = {0};
    while ((linelen = getline(&path, &linecap, stdin)) > 0) {
        path[linelen-1] = '\0';
        if (strlen(path) > PATH_SIZE){
            abortError("File name is too long\n"); 
        }
        sprintf(message, "md5sum %s", path);
        FILE * fp = popen(message, "r");

        if (fp == NULL) {
            char msg[PATH_SIZE+50];
            sprintf(msg, "There has been a problem making the md5 on file %s\n", path);
            reportError(msg);
        }
        else {
            // Obtengo los primeros 32 caracteres, es decir, el hash, 
            // hay que hacer esto porque el md5 devuelve el hash + el nombre del archivo
            char hash_md5[HASH_LENGTH];
            if (fgets(hash_md5, HASH_LENGTH, fp) != NULL) {
                sprintf(message, "File name: %s - Md5: %s - Slave's pid: %d \n", path, hash_md5, getpid());
                write(1, message, MSG_SIZE);
            }
            else {
                char msg[PATH_SIZE+50];
                sprintf(msg, "Error reading MD5 hash for file %s\n", path);
                reportError(msg);
            }
            pclose(fp);
        }
    }
    free(path);
 }