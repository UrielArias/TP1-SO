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
        fdsToClose[index++] = pipeFds[WRITEFILEPIPE];
        if (returnValue == -1){
            perror("pipe creation unsuccesful");
            exit(1);
        }
        returnValue = pipe(&pipeFds[READRESULTPIPE]);
        
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

void monitorSlaves(process* slaves, int remainingFiles, int numSlaves){
    fd_set set;
    int filesDone = 0;
    int maxfd, activeFd;
    while (filesDone <= remainingFiles){
       //select needs the first argument to be the number of the greatest fds to check-> we want to check only the ones who are still are working
       //int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
        maxfd = setFdsToCheck(slaves, numSlaves, &set);
        activeFd = select(maxfd+1, &set, NULL, NULL, NULL);
        if (activeFd == -1){
            perror("Select has failed");
            exit(1);
        }
        //TODO: write to buffer
        for (int slave = 0; activeFd> 0 && slave < numSlaves; slave++){
            //if
            if (FD_ISSET(slaves[slave].readFrom, &set)){
                //tera
            }
        }
    }

}

int setFdsToCheck(process* slaves, int numSlaves, fd_set * set){
    FD_ZERO(set); //first step to clear set
    int fd;
    int maxFd = -1; //in case there are no slaves left working it returns -1
    for (int slave = 0; slave < numSlaves; slave++){
        if (slaves[slave].isExecuting){
            fd = slaves[slave].readFrom;
            FD_SET(fd, set);
            maxFd = (fd > maxFd)? fd : maxFd;
        }
    }
    return maxFd;
}

