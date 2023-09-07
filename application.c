// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "application.h"

int main (int argc, char * argv []) {
    if (argc < 2) {
        fprintf(stderr, "No arguments passed. A path to the files is needed\n");
        exit(EXIT_FAILURE);
    }
    int remainingFiles = argc - 1;    
    int numSlaves = (argc-1 < 5) ? argc-1 : MAXSLAVE;
    int filesPerSlave = ((argc - 1) < numSlaves)? 1 : MAXFILESPERSLAVE;

    FILE * result;
    if( (result = fopen("result.txt", "w")) == NULL ) {
        fprintf(stderr, "Fopen failed");
    }
    process slaves[numSlaves];


    createSlaves(argc, slaves, argv++, filesPerSlave);
    
}

void createSlaves(int numSlaves, process* slaveList, char ** files, int workload){
    char * argvFork[MAXFILESPERSLAVE+2];
    argvFork[0] = "slave";
    char * envpFork[] = {NULL};

    int pipeFds[FDSPERPIPE * PIPESPERPROC];
    int returnValue, pid;
    int maxFd = NUMDEFAULTFDS + workload * PIPESPERPROC * FDSPERPIPE;
    int fdsToClose[maxFd - NUMDEFAULTFDS];
    
    for (int slave = 0, index = 0; slave < numSlaves; slave++){
        returnValue = pipe(pipeFds);
        if (returnValue == -1){
            perror("pipe creation unsuccesful");
            exit(1);
        }
        returnValue = pipe(&pipeFds[READRESULTPIPE]);
        
        fdsToClose[index++] = pipeFds[WRITEFILEPIPE];
        fdsToClose[index++] = pipeFds[READRESULTPIPE];
        
        pid = fork();

        if (pid == 0){
            close(STDIN);
            close(STDOUT);
            for(int i = 0; i < index; i++){
                close(fdsToClose[i]);
            }
            dup(pipeFds[READFILEPIPE]);
            dup(pipeFds[WRITERESULTPIPE]);
            for (int arg = 1; arg <= workload; arg++){
                argvFork[arg] = *files;
                files++;
            }
            argvFork[workload + 1] = NULL;
            execve("./slave", argvFork, envpFork);
        }
        if (pid != 0){
            slaveList[slave].pid = pid;
            slaveList[slave].readFrom = pipeFds[READRESULTPIPE];
            slaveList[slave].sendTo = pipeFds[WRITEFILEPIPE];
            close(pipeFds[READFILEPIPE]);
            close(pipeFds[WRITERESULTPIPE]);
        }
    }
}
