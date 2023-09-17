#include "communication.h"
#include "sharedMemTAD.h"
#include <string.h>

#define NAME_SHM "/shmMd5"

void connectWithView(sharedMem * shm);

void writeToShm( sharedMem * shm, char * message );

void disconnectShm(sharedMem * shm);