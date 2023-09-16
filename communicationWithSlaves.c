#include "communicationWithSlaves.h"

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
            close(STDOUT);
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

int sendInitialLoad(process* slaves, int numSlaves, char ** files, int workload){
    int ret;
    for (int slave = 0; slave < numSlaves; slave++){
        ret = sendFilesToSlave(&slaves[slave], workload, &files[slave * workload]);
    } 
    return ret;
}

int sendFilesToSlave(process* slave, int numFiles, char** files){
    if (numFiles <= 0 || files == NULL){
        perror("An array of strings should be passed and it should be specified the amount to be passed");
        exit(1);
    }
    int result;
    char file[160];
    for (int indexFile = 0; indexFile < numFiles; indexFile++, files++){
        strcpy(file, *files);
        strcat(file, " \n");
        result = write(slave->sendTo, file, strlen(file));
    }
    slave->remainingTasks = numFiles;
    return result;
}
