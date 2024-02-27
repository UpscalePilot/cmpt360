#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <time.h>

#include "dStruct.h"

#ifndef TIME_JIFFY
#define TIME_JIFFY 0
#endif

#ifndef TIME_DT
#define TIME_DT 0
#endif

// this will always be true, FIFO is function is always used
// we do not use this define... shorter code this way
#ifndef ALGOR_FIFO
#define ALGOR_FIFO 1
#endif

#ifndef ALGOR_SJF
#define ALGOR_SJF 0
#endif

#ifndef ALGOR_RR
#define ALGOR_RR 0
#endif

#ifndef ALGOR_MLFQ 
#define ALGOR_MLFQ 0
#endif

//global time (t) for time increments
int t = TIME_JIFFY + TIME_DT;

// open the directory, iterate through files in said directory, remove files
int openDirectory(struct dStruct **QuePtr, int timeT, int processID) {
    // format is time, PID, status, niceness, cputime, proctime
    int status = 1;
    float cpuTime = 0.00;    

    // this is used to hold the characters from the file opened
    char line[100], fileName[100];
    char readNiceness[50], readProcTime[50];
    char *split;

    // change directory to newProc folder
    chdir("..");
    chdir("newProc");

    // open the directory for newProc folder, check if it opened properly
    DIR *fldrptr = opendir(".");
    if (fldrptr == NULL){
        printf("Error : Failed to open input directory\n");
        return processID;
    }

    // used to iterate/open files on directory
    struct dirent* filesInFolder;
    FILE *in_file;
    // loop through the directory, opening all the files
    while ((filesInFolder = readdir(fldrptr))){
        // unix/linux iterates through the current/parent directories by default, skip them
        if (!strcmp (filesInFolder->d_name, ".")){
            continue;
        }
        if (!strcmp (filesInFolder->d_name, "..")){
            continue;
        }

        // check if file is properly opened
        strcpy(fileName, filesInFolder->d_name);
        if ((in_file = fopen(fileName, "r")) == NULL){
            printf("Error: Failed to open input file\n");
        }

        // gets the first line, split by (, )
        fgets(line, 50, in_file);
        split = strtok(line, ", ");

        // check if split is valid, copy string of proc and niceness
        if (split != NULL){
            strcpy(readProcTime, split);
            split = strtok(NULL, "\n");
            strcpy(readNiceness, split);
        }

        // push to list, also increment processID
        // format(pointer to queuelist,int pid, int status, int niceness, float cputime, float proctime, int timeadded)
		if(ALGOR_SJF == 0){
	        push(QuePtr, processID++, status, atoi(readNiceness), cpuTime, atof(readProcTime), timeT);
    	}
		else{
	        pushSJF(QuePtr, processID++, status, atoi(readNiceness), cpuTime, atof(readProcTime), timeT);
		}
        fclose(in_file);

        // implement remove file after closing here
        if (remove(fileName) == 0){
            printf("File has been processed and successfully removed\n\n");
        }
        else{
            printf("Unable to process deleting file\n\n");
        }
    }

    closedir(fldrptr);

    return processID;
}


int main(void){
    // Initialize structure for ready and running queue
    struct dStruct* readyQueue = NULL;
    struct dStruct* runningQueue = NULL;
    int newProcessID=0;

    while (1){

        // open directory and open all files and load files to Ready Queue
        newProcessID = openDirectory(&readyQueue, t, newProcessID);

        // copy to running if possible
        copyReadyToRunning(&runningQueue, &readyQueue);        

        // increment time t, iterate through lists and update time, running list cputime is also incremented by 1
        incrementAllT(&runningQueue, readyQueue, t++);

        // sets status for running process can't pop yet, save first
        isTheProcessDone(&runningQueue);

        // save both lists to log
        openlog(readyQueue, runningQueue);
        
        // time to pop if process is done
        checkProcess(&runningQueue);

        if(ALGOR_RR == 1){
            roundRobin(&runningQueue, &readyQueue);
        }
        // if both lists are empty, break
        if (runningQueue == NULL && readyQueue == NULL){
            break;
        }
    }
    close(newProcessID);

    return 0;
}