#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <string.h>
#include "communicationWithView.h"

#define MAXSLAVE 5
#define FDSPERPIPE 2
#define PIPESPERPROC 2
#define NUMDEFAULTFDS 3
#define MAXFILESPERSLAVE 2
#define MINFILESPERSLAVE 1
#define STDIN 0
#define STDOUT 1
#define READFILEPIPE 0
#define WRITEFILEPIPE 1
#define READRESULTPIPE 2
#define WRITERESULTPIPE 3
#define TRUE 1
#define FALSE 0

typedef struct process{
    int pid;
    int readFrom;
    int sendTo;
    int isOperative;
    int remainingTasks;
} process;

void createSlaves(int numSlaves, process* slaveList);
void monitorSlaves(process* slaves, int remainingFiles, int numSlaves, char * argv [], FILE * resultFile);
int sendInitialLoad(process* slaves, int numSlaves, char ** files, int workload);
