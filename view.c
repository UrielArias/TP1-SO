// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "sharedMemTAD.h"
#include <stdlib.h>

int main(){
    char * name = "/shm";
    sharedMem shm;
    int returnValue;

    printf("la voy a crear\n");

shm_unlink(name);
    returnValue= createShm(&shm, name);
    if ( returnValue == EXIT_FAILURE) {             
        perror("shm couldnt be created");
        exit(1);
    }
    
sleep(10);
    returnValue = writeShm(&shm,"Hola soy juan \n");
    if ( returnValue == EXIT_FAILURE) {             
        perror("shm couldnt be written");
        exit(1);
    }

    printf("Se mando el mnsj 1\n");
 
    returnValue = writeShm(&shm,"POR LAS NOCHES SOY JUANA \n");
    if ( returnValue == EXIT_FAILURE) {             
        perror("shm couldnt be written");
        exit(1);
    }

    printf("Se mando el mnsj 2\n");

sleep(10);
    returnValue = writeShm(&shm,"tambien me llaman como juanita \n");
  
    printf("Se mando el mnsj 3\n");
    // buena practica q x sep 
    printf("la voy a cerrar a JUAN\n");
   // returnValue = closeShm(&shm);
    if ( returnValue == EXIT_FAILURE) {             
        perror("shm couldnt be closed");
        exit(1);
    }

    printf("la voy a cerrar\n");
    returnValue = deleteShm(&shm);
    if ( returnValue == EXIT_FAILURE) {             
        perror("shm couldnt be deleted");
        exit(1);
    }
       
}