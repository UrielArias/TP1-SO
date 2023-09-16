// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "communicationAppView.h"

//! falta poner en main de app
// incluir el define NAME_SHM
//      conviene ponerlo fuera dl TAD, + prolijo
// escribe  - una x si ya termino sleep(2)
//          - una z para indicar fin
//              hay q darle un tiempito a view para q lea
//              antes de deleteShm() en app

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
        perror("communication couldnt be established, make sure app was initiated");
        exit(1);
    }
    
    // puede acceder? 
    if ( *shm.virtualAdress=='x'){
        perror("Buffer cant be accessed, its too late");
        exit(1);
    }

    
    //leo la x
    returnValue = readShm(&shm,message,2);
        if ( returnValue == EXIT_FAIL) {             
            perror("shm couldnt be read");
            closeShm(&shm);
            exit(1);
        }

    while(1){
        returnValue = readShm(&shm,message,MSG_SIZE);
        if ( returnValue == EXIT_FAIL) {             
            perror("shm couldnt be read");
            closeShm(&shm);
            exit(1);
        }
        if ( message[0]=='x' )
            break;
        printf("%s\n",message);
    }
            
    returnValue = closeShm(&shm);
    if ( returnValue == EXIT_FAIL) {             
        perror("shm couldnt be closed");
        exit(1);
    }

  
}
