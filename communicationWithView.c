// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "communicationWithView.h"

void connectWithView(sharedMem * shm){
    int returnValue;
   
    returnValue= createShm(shm, NAME_SHM);
    if ( returnValue == EXIT_FAIL ) {             
        perror("shm couldnt be created");
        exit(EXIT_FAIL);
    }
    write(1,NAME_SHM,7);

    sleep(2);
    printf("\nYa no se puede conectar la view\n");
    writeToShm(shm,"x");
    
}

void writeToShm( sharedMem * shm, char * message ){
    char returnValue =  writeShm(shm, message);
    if ( returnValue == EXIT_FAIL) {             
        perror("shm couldnt be written");
        closeShm(shm);
        deleteShm(shm);
        exit(EXIT_FAIL);
    }
}

void disconnectShm(sharedMem * shm){
    writeToShm(shm,"x");
    sleep(2);
    int returnValue = closeShm(shm);
    if ( returnValue == EXIT_FAIL) {             
        perror("shm couldnt be closed");
        deleteShm(shm);
        exit(EXIT_FAIL); 
    }

    returnValue = deleteShm(shm);
    if ( returnValue == EXIT_FAIL) {             
        perror("shm couldnt be deleted");
        exit(EXIT_FAIL);
    }
}
