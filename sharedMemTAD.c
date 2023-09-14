// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "sharedMemTAD.h"

/*
    fstat() => obtengo stat struct q me la descri
                + size, permissions, ...
    fchmod() => para cambiar permisos
*/

static int mapMem( sharedMem * shm, int prot, int fd){
    
    shm->virtualAdress = mmap(shm->name, SIZE_SHM, prot , MAP_SHARED, fd, 0 );
    if ( shm->virtualAdress == MAP_FAILED){
        perror("shm couldnt be adressed");
        return EXIT_FAILURE;
    }

    // evito cod repe, pero no se q tan bien queda aca
    shm->offset = 0;

    // podria ya cerrar el fd sin q me afecte la shm
    // saco el fd, necesario?
    close(fd);
}


int createShm( sharedMem * shm, char * name){
    if ( name == NULL || name[0]!='/' ){
        perror("Invalid shm name");
        return EXIT_FAILURE; 
    }

    // crea o ABRE
    // O_EXCL asi si ya existe una con = nombre, no se crea
    int fd = shm_open( name,O_CREAT | O_RDWR | O_EXCL , 0666);
    if ( fd==-1) {             
        perror("shm couldnt be created");
        return EXIT_FAILURE;
    }
    shm->name = name;    //no esta copiando el nombre?

    // set size 
    int returnValue;
    returnValue = ftruncate(fd,SIZE_SHM);
    if ( returnValue==-1) {             
        perror("shm size unavailable"); //! raro
        return returnValue;
    }

    // la mapea en el virtual adress space del PROCESO Q LA LLAMO
    returnValue = mapMem(shm, PROT_READ | PROT_WRITE, fd);    
    if ( returnValue == EXIT_FAILURE)
        return returnValue;

    shm->readsAvailable = sem_open(name,O_CREAT | O_EXCL | O_RDWR, 0666,0);
    if ( shm->readsAvailable == SEM_FAILED){
        perror("Could not create semaphore");
        return EXIT_FAILURE;
    }
}

/// @brief * shm = DIRECC DONDE METO INFO D LA NUEVA SHM
/// => app.c no la invoca
int openShm( sharedMem * shm, char * name ){      
    if ( name==NULL){
        perror("Invalid shm name");
        return EXIT_FAILURE;  
    }

    shm->name = name;    
    
    int fd = shm_open(name,O_RDONLY, 0666);
    if ( fd == -1){
        perror("shm couldnt be opened");
        return EXIT_FAILURE;
    }
    
    int returnValue;
    returnValue = mapMem(shm, PROT_READ, fd);    
    if ( returnValue == EXIT_FAILURE)
        return returnValue;
    
    shm->readsAvailable = sem_open(name,O_RDWR);
    if ( shm->readsAvailable == SEM_FAILED){
        perror("Could not open semaphore, chau chau adios");
        return EXIT_FAILURE;
    }
}


// la debe elim el proceso vista
int deleteShm( sharedMem * shm){
    int returnValue;
    
    returnValue = shm_unlink( shm->name);
    if ( returnValue == EXIT_FAILURE)
        return returnValue;

    returnValue = sem_unlink(shm->name);
    if ( returnValue == EXIT_FAILURE)
        return returnValue;
}
 
int closeShm( sharedMem * shm){
    int returnValue;
    // una vez q todos cerrados, lo destruye 
    returnValue = munmap( shm->name , SIZE_SHM);
    if ( returnValue == EXIT_FAILURE)
        return returnValue;

    returnValue = sem_close( shm->readsAvailable);
    if ( returnValue == EXIT_FAILURE)
        return returnValue;

}

// message es donde voy a meter lo leido
// GUARDA mensaje SIN \n 
// copia hasta size => evito pasarme dl espacio message
int readShm(sharedMem * shm, char * message, int size){
    if ( message==NULL ){
        perror(INVALID_ARGS);
        return EXIT_FAILURE; 
    }
   
    //hay para leer
    sem_wait(shm->readsAvailable);
    
    //RESERVO LUGAR PARA 0 FINAL
   size--;

    int i;
    for ( i=0; shm->virtualAdress[shm->offset] != 0 && i<size; i++){
            message[i] = shm->virtualAdress[shm->offset++];
    }

    message[i] = 0;
    shm->offset++;
    //shm->offset++;
}

int writeShm(sharedMem * shm, char * message){
    // check si offset se pasa de SIZE_SHM?
    /* TP: "lugar se puede crear un buffer suficientemente 
    grande para guardar tods lo resultados."
    */
    int i;
    for ( i=0; message[i] != 0; i++){
        shm->virtualAdress[shm->offset++] = message[i];
    }                      

    //mete el cero 
    // ++ asi no pisa desp el 0
    shm->virtualAdress[shm->offset++] = message[i];
    

    sem_post(shm->readsAvailable);
}





