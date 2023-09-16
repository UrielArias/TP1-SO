// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "application.h"

static int setFdsToCheck(process* slaves, int numSlaves, fd_set * set);
sharedMem shm;

int main (int argc, char * argv []) {
    if (argc < 2) {
        fprintf(stderr, "No arguments passed. A path to the files is needed\n");
        exit(1);
    }
    connectWithView(&shm);
    int remainingFiles = argc - 1;    
    int numSlaves = (remainingFiles < MAXSLAVE) ? remainingFiles : MAXSLAVE;
    int filesPerSlave = (remainingFiles <= numSlaves || remainingFiles < MAXFILESPERSLAVE * MAXSLAVE)? MINFILESPERSLAVE : MAXFILESPERSLAVE;
    FILE * result;
    if( (result = fopen("result.txt", "w")) == NULL ) {
        fprintf(stderr, "Fopen failed");
    }
    process slaves[numSlaves];

    createSlaves(numSlaves, slaves);    
    sendInitialLoad(slaves, numSlaves, ++argv, filesPerSlave);
    remainingFiles -= numSlaves * filesPerSlave;
    monitorSlaves(slaves, remainingFiles, numSlaves, argv+ numSlaves * filesPerSlave, result, &shm);
    fclose(result);
    disconnectShm(&shm);
}


void monitorSlaves(process* slaves, int remainingFiles, int numSlaves, char * argv [], FILE * resultFile, sharedMem * shm){
    fd_set set;
    int filesDone = 0;
    int maxfd, activeFd;
    int filesToAssigned = remainingFiles;
    char buf[MSG_SIZE];
    while (filesDone <= remainingFiles){
        maxfd = setFdsToCheck(slaves, numSlaves, &set);
        activeFd = select(maxfd+1, &set, NULL, NULL, NULL);
        if (activeFd == EXIT_FAIL){
            perror("Select has failed");
            exit(1);
        }
        for (int slave = 0; activeFd> 0 && slave < numSlaves; slave++){
            if (FD_ISSET(slaves[slave].readFrom, &set)){
                read(slaves[slave].readFrom, buf, MSG_SIZE);
                char * token = "\n";
                char * toWrite = strtok(buf, token);
                slaves[slave].remainingTasks--;
                filesDone++;
                fwrite(toWrite, strlen(toWrite), 1, resultFile);
                fwrite(token, strlen(token), 1, resultFile);
                writeToShm(shm, toWrite);
                if (!slaves[slave].remainingTasks && filesToAssigned > 0){
                    sendFilesToSlave(&slaves[slave], MINFILESPERSLAVE, argv);
                    filesToAssigned--;
                }
                else if (!slaves[slave].remainingTasks){
                    slaves[slave].isOperative = FALSE;
                    close(slaves[slave].sendTo);
                    close(slaves[slave].readFrom);
                }
            }
        }
    }
}

static int setFdsToCheck(process* slaves, int numSlaves, fd_set * set){
    FD_ZERO(set); //first step to clear set
    int fd;
    int maxFd = EXIT_FAIL; //in case there are no slaves left working it returns -1
    for (int slave = 0; slave < numSlaves; slave++){
        if (slaves[slave].isOperative){
            fd = slaves[slave].readFrom;
            FD_SET(fd, set);
            maxFd = (fd > maxFd)? fd : maxFd;
        }
    }
    return maxFd;
}
