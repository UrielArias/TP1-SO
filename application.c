// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "application.h"

static int setFdsToCheck(process* slaves, int numSlaves, fd_set * set);
static int sendFilesToSlave(process* slave, int numFiles, char** files);
static int setFdsToCheck(process* slaves, int numSlaves, fd_set * set);


int main (int argc, char * argv []) {
    if (argc < 2) {
        fprintf(stderr, "No arguments passed. A path to the files is needed\n");
        exit(EXIT_FAILURE);
    }
    int remainingFiles = argc - 1;    
    int numSlaves = (remainingFiles < 5) ? remainingFiles : MAXSLAVE;
    int filesPerSlave = (remainingFiles <= numSlaves || remainingFiles <= MAXFILESPERSLAVE * MAXSLAVE)? 1 : MAXFILESPERSLAVE;
    FILE * result;
    if( (result = fopen("result.txt", "w")) == NULL ) {
        fprintf(stderr, "Fopen failed");
    }
    //code;
    process slaves[numSlaves];


    createSlaves(numSlaves, slaves);    
    write(slaves[0].sendTo, "./result.txt", 5);
    sendInitialLoad(slaves, numSlaves, ++argv, filesPerSlave);
    remainingFiles -= numSlaves * filesPerSlave;
    monitorSlaves(slaves, remainingFiles, numSlaves, argv+ numSlaves * filesPerSlave, result);
    fclose(result);
}

void createSlaves(int numSlaves, process* slaveList){
    char * argvFork[MAXFILESPERSLAVE];
    argvFork[0] = "slave";
    argvFork[1] = NULL;
    char * envpFork[] = {NULL};

    int pipeFds[FDSPERPIPE * PIPESPERPROC];
    int returnValue, pid;
    int maxFd = NUMDEFAULTFDS + numSlaves * PIPESPERPROC * FDSPERPIPE;
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
            //close(STDOUT);
            for(int i = 0; i < index; i++){
                close(fdsToClose[i]);
            }
            dup(pipeFds[READFILEPIPE]);
            dup(pipeFds[WRITERESULTPIPE]);
            close(pipeFds[READFILEPIPE]);
            close(pipeFds[WRITERESULTPIPE]);
            execve("./slave", argvFork, envpFork);
        }
        if (pid != 0){
            slaveList[slave].pid = pid;
            slaveList[slave].readFrom = pipeFds[READRESULTPIPE];
            slaveList[slave].sendTo = pipeFds[WRITEFILEPIPE];
            slaveList[slave].isOperative= TRUE;
            close(pipeFds[READFILEPIPE]);
            close(pipeFds[WRITERESULTPIPE]);
        }
    }
}

//to check
int sendInitialLoad(process* slaves, int numSlaves, char ** files, int workload){
    int ret;
    for (int slave = 0; slave < numSlaves; slave++){
        ret = sendFilesToSlave(&slaves[slave], workload, files);
    } 
    return ret;
}

void monitorSlaves(process* slaves, int remainingFiles, int numSlaves, char * argv [], FILE * resultFile){
    fd_set set;
    int filesDone = 0;
    int maxfd, activeFd;
    int filesToAssigned = remainingFiles;
    char buf[160];
    while (filesDone <= remainingFiles){
       //select needs the first argument to be the number of the greatest fds to check-> we want to check only the ones who are still are working
       //int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
        maxfd = setFdsToCheck(slaves, numSlaves, &set);
        activeFd = select(maxfd+1, &set, NULL, NULL, NULL);
        printf("%d\n", activeFd);
        if (activeFd == -1){
            perror("Select has failed");
            exit(1);
        }
        //TODO: write to buffer
        for (int slave = 0; activeFd> 0 && slave < numSlaves; slave++){
            //if
            if (FD_ISSET(slaves[slave].readFrom, &set)){
                read(slaves[slave].readFrom, buf, 160);
                slaves[slave].remainingTasks--;
                filesDone++;
                if (!slaves[slave].remainingTasks && filesToAssigned > 0){
                    sendFilesToSlave(&slaves[slave], 1, argv);
                    filesToAssigned--;
                }
                else if (!slaves[slave].remainingTasks){
                    slaves[slave].isOperative = FALSE;
                    //close fd? pipe no longer needed
                }
                //write to Shm se tiene que crear desde la app para tener el tad
                fwrite(buf, sizeof(buf), 1, resultFile);
            }
        }
    }
}
//to check
static int sendFilesToSlave(process* slave, int numFiles, char** files){
    if (numFiles <= 0 || files == NULL){
        perror("An array of strings should be passed and it should be specified the amount to be passed");
        exit(1);
    }
    int result;
    for (int indexFile = 0, i = 0; indexFile < numFiles; indexFile++){
        result = write(slave->sendTo, *files, strlen(*files));
        printf("%s\n", files[i++]);
    }
    slave->remainingTasks = numFiles;
    return result;
}

static int setFdsToCheck(process* slaves, int numSlaves, fd_set * set){
    FD_ZERO(set); //first step to clear set
    int fd;
    int maxFd = -1; //in case there are no slaves left working it returns -1
    for (int slave = 0; slave < numSlaves; slave++){
        if (slaves[slave].isOperative){
            fd = slaves[slave].readFrom;
            FD_SET(fd, set);
            maxFd = (fd > maxFd)? fd : maxFd;
        }
    }
    return maxFd;
}

