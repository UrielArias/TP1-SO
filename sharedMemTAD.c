// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "sharedMemTAD.h"
#include "errorHandler.h"

static int mapMem( sharedMem * shm, int prot, int fd){
    
    shm->virtualAdress = mmap(NULL, SIZE_SHM, prot , MAP_SHARED, fd, 0 );
    if ( shm->virtualAdress == MAP_FAILED){
        perror("shm couldnt be adressed");
        return EXIT_FAIL;
    }
    shm->offset = 0;
    close(fd);

    return 0;
}


int createShm( sharedMem * shm, char * name){
    if ( name == NULL || name[0]!='/' ){
        perror("Invalid shm name");
        return EXIT_FAIL; 
    }

    int fd = shm_open( name,O_CREAT | O_RDWR | O_EXCL , 0666);
    if ( fd==EXIT_FAIL) {             
        perror("shm couldnt be created");
        return EXIT_FAIL;
    }
    shm->name = name;   

    int returnValue;
    returnValue = ftruncate(fd,SIZE_SHM);
    if ( returnValue==EXIT_FAIL) {             
        perror("shm size unavailable");
        return returnValue;
    }

    returnValue = mapMem(shm, PROT_READ | PROT_WRITE, fd);    
    if ( returnValue == EXIT_FAIL)
        return returnValue;

    shm->readsAvailable = sem_open(name,O_CREAT | O_EXCL | O_RDWR, 0666,0);
    if ( shm->readsAvailable == SEM_FAILED){
        perror("Could not create semaphore");
        return EXIT_FAIL;
    }

    return 0;
}

int openShm( sharedMem * shm, char * name ){      
    if ( name == NULL || name[0]!='/' ){
        perror("Invalid shm name");
        return EXIT_FAIL; 
    }

    shm->name = name;    
    
    int fd = shm_open(name,O_RDONLY, 0666);
    if ( fd == EXIT_FAIL){
        perror("shm couldnt be opened");
        return EXIT_FAIL;
    }
    
    int returnValue;
    returnValue = mapMem(shm, PROT_READ, fd);    
    if ( returnValue == EXIT_FAIL)
        return returnValue;
    
    shm->readsAvailable = sem_open(name,O_RDWR);
    if ( shm->readsAvailable == SEM_FAILED){
        perror("Could not open semaphore");
        munmap(name,SIZE_SHM);
        return EXIT_FAIL;
     }

    return 0;
}


int deleteShm( sharedMem * shm){
    if ( shm == NULL){
        perror(INVALID_ARGS);
        return EXIT_FAIL; 
    }
    
    int returnValue;
    returnValue = sem_unlink(shm->name);
    if ( returnValue == EXIT_FAIL)
        return returnValue;
    
    returnValue = shm_unlink( shm->name);
    if ( returnValue == EXIT_FAIL)
        return returnValue;


    return 0;
}
 
int closeShm( sharedMem * shm){
    if ( shm == NULL){
        perror(INVALID_ARGS);
        return EXIT_FAIL; 
    }
    
    int returnValue;
    
    returnValue = munmap( shm->virtualAdress , SIZE_SHM);
    if ( returnValue == EXIT_FAIL){
        perror("Couldnt unmap shm");
        return returnValue;
    }

    returnValue = sem_close( shm->readsAvailable);
    if ( returnValue == EXIT_FAIL){
        perror("Couldnt close semaphore");
        return returnValue;
    }
    
    return 0;
}

// it copies to message only one read available
// it copies till size
int readShm(sharedMem * shm, char * message, int size){
    if ( shm == NULL || message==NULL || size<=0){
        perror(INVALID_ARGS);
        return EXIT_FAIL; 
    }
   
    // waits for something to be read
    sem_wait(shm->readsAvailable);
    
    // makes place for the final 0 
    size--;

    int i;
    for ( i=0; shm->virtualAdress[shm->offset] != 0 && i<size; i++){
            message[i] = shm->virtualAdress[shm->offset++];
    }

    message[i] = 0;
    shm->offset++;
    
    return 0;
}

int writeShm(sharedMem * shm, const char * message){
    if ( shm == NULL || message==NULL ){
        perror(INVALID_ARGS);
        return EXIT_FAIL; 
    }   

    int i;
    for ( i=0; message[i] != 0; i++){
        shm->virtualAdress[shm->offset++] = message[i];
    }                      

    // includes the final 0 
    // ++ so the final 0 isnt overwritten 
    shm->virtualAdress[shm->offset++] = message[i];
    
    sem_post(shm->readsAvailable);
    
    return 0;
}





