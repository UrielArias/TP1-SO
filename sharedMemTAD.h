#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>   
#include <semaphore.h>
#include <unistd.h>

#define SIZE_SHM 1048576       // 1MB
#define EXIT_FAIL -1
#define INVALID_ARGS "Invalid arguments"

typedef struct shm {
    char * name;    
    int offset;     
    char * virtualAdress; 
    sem_t * readsAvailable;
} sharedMem;


int deleteShm( sharedMem * shm);
int openShm( sharedMem * shm, char * name );
int createShm( sharedMem * shm, char * name);
int closeShm( sharedMem * shm);
int readShm(sharedMem * shm, char * message, int size);
int writeShm(sharedMem * shm, const char * message);

/* contrato:
    + solo puede escribir el proceso q creó la shm
    + el resto de procesos q quieran acceder solo 
    pueden leer.
*/