#include "communicationWithView.h"
#include "communicationWithSlaves.h"
#include "errorHandler.h"

#define MAXSLAVE 5
#define MAXFILESPERSLAVE 2
#define MINFILESPERSLAVE 1
#define FALSE 0

void monitorSlaves(process* slaves, int remainingFiles, int filesToAssign, int numSlaves, char * argv [], FILE * resultFile, sharedMem* shm);