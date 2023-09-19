// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "application.h"

static int setFdsToCheck(process* slaves, int numSlaves, fd_set * set);
sharedMem shm;

int main (int argc, char * argv []) {
    if (argc < 2) {
        abortError("No arguments passed. A path to the files is needed\n");
    }
    connectWithView(&shm);
    int remainingFiles = argc - 1;    
    int filesToAssign;
    int numSlaves = (remainingFiles < MAXSLAVE) ? remainingFiles : MAXSLAVE;
    int filesPerSlave = (remainingFiles < MAXFILESPERSLAVE * MAXSLAVE)? MINFILESPERSLAVE : MAXFILESPERSLAVE;
    FILE * result;
    if( (result = fopen("result.txt", "w")) == NULL ) {
        abortError("Fopen failed\n");
    }
    process slaves[numSlaves];
    
    createSlaves(numSlaves, slaves);    
    sendInitialLoad(slaves, numSlaves, ++argv, filesPerSlave);
    filesToAssign = remainingFiles -numSlaves * filesPerSlave;
    monitorSlaves(slaves, remainingFiles, filesToAssign, numSlaves, argv+ numSlaves * filesPerSlave, result, &shm);
    fclose(result);
    disconnectShm(&shm);
}


void monitorSlaves(process* slaves, int remainingFiles, int filesToAssign, int numSlaves, char * argv [], FILE * resultFile, sharedMem * shm){
    fd_set set;
    int filesDone = 0;
    char buf[MSG_SIZE];
    while (filesDone < remainingFiles){
        int maxfd, activeFd;
        maxfd = setFdsToCheck(slaves, numSlaves, &set);
        activeFd = select(maxfd+1, &set, NULL, NULL, NULL);
        if (activeFd == EXIT_FAIL){
            abortError("Select has failed\n");
        }
        for (int slave = 0; activeFd> 0 && slave < numSlaves; slave++){
            if (FD_ISSET(slaves[slave].readFrom, &set)){
                read(slaves[slave].readFrom, buf, MSG_SIZE);
                char * token = "\n";
                char * toWrite = strtok(buf, token);
                slaves[slave].remainingTasks--;
                filesDone++;
                fwrite(toWrite, strlen(toWrite), 1, resultFile);
                fwrite(token, 1, 1, resultFile);
                writeToShm(shm, toWrite);
                if (!slaves[slave].remainingTasks && filesToAssign > 0){
                    sendFilesToSlave(&slaves[slave], MINFILESPERSLAVE, argv++);
                    filesToAssign--;
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
    FD_ZERO(set);
    int fd;
    int maxFd = EXIT_FAIL; 
    for (int slave = 0; slave < numSlaves; slave++){
        if (slaves[slave].isOperative){
            fd = slaves[slave].readFrom;
            FD_SET(fd, set);
            maxFd = (fd > maxFd)? fd : maxFd;
        }
    }
    return maxFd;
}