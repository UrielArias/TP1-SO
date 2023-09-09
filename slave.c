// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <openssl/md5.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>

#define SIZE_MD5 16


 int main(){
    char * name_file;
    char * md5 = malloc(SIZE_MD5); 
    
    // y si aplication les pasa el path del dir
    // q contiene los files al principio?
    // => me pasa solo los nom d los files => + liviano
    // y desd cda slave los concat?
    // o termina siendo menos efi?
    // concat parece costoso, pero digo como idea
    // pr ver si se puede lograr d otra manera + efi
    // el = resultado  
    
    char * fmt;

    // TODO: frena con el \n?
    // idea: separo nombre de files con \n 
    // 

    while ( scanf("%s",name_file) ){
        // formato: ""
        // fmt = sscanf("md5 path/")
        // popen("md5sum")
        // TODO
        /*  Lo hacemos desde app
        modularizamos + evitamos syscall 
        printf("%d", getpid());
        */
        printf("%s \n ",md5);
    }

    free(md5);

 }