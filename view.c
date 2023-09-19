// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "communication.h"
#include "sharedMemTAD.h"
#include "errorHandler.h"

sharedMem shm;

int main(int argc, char * argv []){

    char * nameShm;
    int returnValue; 
    char message[MSG_SIZE];

    if (argc == 1){
        read(0,message,7);
        nameShm = message; 
    } else 
        nameShm = argv[1];
       
    returnValue= openShm(&shm, nameShm);
    if ( returnValue == EXIT_FAIL) {     
        abortError("Communication couldnt be established, make sure app was initiated\n");        
    }
    
    // ask if it's available to connect
    if ( *shm.virtualAdress=='x'){
        abortError("Buffer cant be accessed, its too late\n");
    }

    
    //the 'x' is read
    returnValue = readShm(&shm,message,2);
        if ( returnValue == EXIT_FAIL) {             
            closeShm(&shm);
            abortError("Shared memory couldn't be read\n");
        }

    while(1){
        returnValue = readShm(&shm,message,MSG_SIZE);
        if ( returnValue == EXIT_FAIL) {             
            closeShm(&shm);
            abortError("Shared memory couldn't be read\n");
        }
        if ( message[0]=='x' )
            break;
        printf("%s\n",message);
    }
            
    returnValue = closeShm(&shm);
    if ( returnValue == EXIT_FAIL) {             
        abortError("Shared memory couldn't be closed\n");
    }

  
}