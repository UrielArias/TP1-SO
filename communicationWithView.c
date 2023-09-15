#include "communicationWithView.h"

void connectWithView(sharedMem * shm){
    int returnValue;
   
    returnValue= createShm(shm, NAME_SHM);
    if ( returnValue == EXIT_FAILURE ) {             
        perror("shm couldnt be created");
        exit(1);
    }
    write(1,NAME_SHM,4);

    sleep(2);
    printf("\nYa no se puede conectar la view\n");
    writeToShm(shm,"x");
    
}

void writeToShm( sharedMem * shm, char * message ){
    char returnValue =  writeShm(shm, message);
    if ( returnValue == EXIT_FAILURE) {             
        perror("shm couldnt be written");
        closeShm(shm);
        deleteShm(shm);
        exit(1);
    }
}

void desconnectShm(sharedMem * shm){

    writeToShm(shm,"Fin de analisis\n");

    int returnValue = closeShm(shm);
    if ( returnValue == EXIT_FAILURE) {             
        perror("shm couldnt be closed");
        deleteShm(shm);
        exit(1); 
    }

    returnValue = deleteShm(shm);
    if ( returnValue == EXIT_FAILURE) {             
        perror("shm couldnt be deleted");
        exit(1);
    }
}
