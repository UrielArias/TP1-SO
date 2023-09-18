// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "communicationWithView.h"
#include "errorHandler.h"

void connectWithView(sharedMem * shm){
    int returnValue;
   
    returnValue= createShm(shm, NAME_SHM);
    if ( returnValue == EXIT_FAIL ) {    
        abortError("Shared memory couldn't be created\n");         
    }
    write(1,NAME_SHM,strlen(NAME_SHM));

    sleep(2);
    writeToShm(shm,"x");
    
}

void writeToShm( sharedMem * shm, char * message ){
    char returnValue =  writeShm(shm, message);
    if ( returnValue == EXIT_FAIL) {             
        closeShm(shm);
        deleteShm(shm);
        abortError("Shared memory couldn't be written\n"); 
    }
}

void disconnectShm(sharedMem * shm){
    writeToShm(shm,"x");
    sleep(2);
    int returnValue = closeShm(shm);
    if ( returnValue == EXIT_FAIL) {             
        deleteShm(shm);
        abortError("Shared memory couldn't be closed\n"); 
    }

    returnValue = deleteShm(shm);
    if ( returnValue == EXIT_FAIL) {             
        abortError("Shared memory couldn't be deleted\n"); 
    }
}