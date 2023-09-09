#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/select.h>

#define MAXSLAVE 5
#define FDSPERPIPE 2
#define PIPESPERPROC 2
#define NUMDEFAULTFDS 3
#define MAXFILESPERSLAVE 2
#define STDIN 0
#define STDOUT 1
#define READFILEPIPE 0
#define WRITEFILEPIPE 1
#define READRESULTPIPE 2
#define WRITERESULTPIPE 3

typedef struct process{
    int pid;
    int readFrom;
    int sendTo;
    int isExecuting;
} process;

void createSlaves(int numSlaves, process* slaveList, char ** files, int workload);
void monitorSlaves(process* slaves, int remainingFiles, int numSlaves);