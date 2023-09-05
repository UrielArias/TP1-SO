// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <openssl/md5.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>

#define SIZE_MD5 16

/**
 ** Para calcular md5
 *  unsigned char *MD5(const unsigned char *d, unsigned long n, unsigned char *md);
 * @brief 
 *  d = nombre file
 *  n = # bytes a leer
 *  md = donde se guarda el resultado
 */


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
    
    struct stat data; 

    // TODO: frena con el \n?
    // idea: separo nombre de files con \n 
    while ( scanf("%s",name_file) ){

        // vale la pena syscall x cada file?
        stat(name_file, &data);
        //if ( errno==EIO )
        //    exit(1);

        // TODO
        // man dice q retorna el hash
        // pero creo q tmb lo guarda si pasas como param
        // si ese param ==null => guarda cm var statica
        // es otra cosa? hay q sacar
        // md5 = 
        MD5(name_file,data.st_size, md5 );

        // TODO
        // ver q hay q enviarle 
        printf("%d", getpid());
        printf("%s",md5);
    }

    free(md5);

 }