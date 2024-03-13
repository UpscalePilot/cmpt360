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

#ifndef ALGOR_FIFO
#define ALGOR_FIFO 1
#endif

#ifndef ALGOR_SJF
#define ALGOR_SJF 1
#endif

#ifndef ALGOR_RR
#define ALGOR_RR 0
#endif

//global time (t) for time increments
int t = TIME_JIFFY + TIME_DT;


// increment t for both structures
void incrementAllT(struct dStruct **runningList, struct dStruct *readyList, int tTime)
{
    (*runningList) -> cpuTime++;
    (*runningList) -> tTime = tTime;

    struct dStruct *tempReady = readyList;
    
    if (readyList != NULL) 
    {
        while(tempReady->next != NULL)
        {
            tempReady -> tTime = tTime;
            tempReady = tempReady -> next;
        }
        tempReady -> tTime = tTime;
    }
    
    return;
}

// function is used to loop adding and removing processes
// from run queue, adding and removing processes from a ready queue
// then updating, not creating a log!!!
// updating log files until the queues are empty
int openlog(struct dStruct *readyList, struct dStruct *runningList)
{
    char currentPath[100], oneLevelDown[100], log[100];

    // get current pathway
    getcwd(currentPath, 100);
    // get one level down
    chdir("..");
    getcwd(currentPath, 100);

    // these are used for opening the directory/folder
    DIR *fldrptr;
    
    // these are used for opening the files
    FILE *in_file;

    // save pathway to and log folder
    strcpy(oneLevelDown, currentPath);
    strcpy(log, currentPath);
    strcat(log, "/log");

    // change working directory to log
    chdir("log");
    printf("\nOpening filepath: %s\n\n", getcwd(currentPath, 100));

    // open the directory for newProc folder
    fldrptr = opendir(log);

    // check if directory opened properly
    if (fldrptr == NULL)
    {
        printf("Error : Failed to open input directory\n");

        return 0;
    }

    // get the current date to make log file, student server is 1 month behind....
    time_t t = time (NULL);
    struct tm* lt = localtime (&t);
    int year = lt->tm_year;
    int month = lt->tm_mon;
    int day = lt->tm_mday;
    if (month == 12)
    {
        month = 01;
    }
    else
    {
        month++;
    }

    char buf[0x100];
    snprintf(buf, sizeof(buf), "log-%02d-%02d-%d.txt", month, day, year + 1900);

    // logically, if we can't read it, file doesn't exist, so lets create one
    // only time can fail is if someone made the file, before we ran the program
    if ((in_file = fopen(buf, "r")) == NULL)
    {
        in_file = fopen(buf, "w");
        fprintf(in_file, "time    pid   status    niceness    cputime    proctime      Group List\n");
    }
    else
    {
        in_file = fopen(buf, "a");
    }
    
    struct dStruct* readyTemp = readyList;
    struct dStruct* runningTemp = runningList;

    // cycle through list and write readyList info to log
    if (readyList != NULL)
    {
        do
        {
            fprintf(in_file, "  %d      %d      %d          %d          %.2f       %.2f        Ready List\n", readyTemp->tTime, readyTemp->pid, readyTemp->status, readyTemp->niceness, readyTemp->cpuTime, readyTemp->procTime);
            readyTemp = readyTemp -> next;
        } while (readyTemp != NULL);
    }

    // cycle through list and write runningList info to log
    if (runningList != NULL)
    {
        do
        {
            fprintf(in_file, "  %d      %d      %d          %d          %.2f       %.2f        Running List\n", runningList->tTime, runningTemp->pid, runningTemp->status, runningTemp->niceness, runningTemp->cpuTime, runningTemp->procTime);
            runningTemp = runningTemp -> next;
        } while (runningTemp != NULL);
    }

    fclose(in_file);

    closedir(fldrptr);   

    return 0;
}

// open the directory, iterate through files in said directory, remove files
int openDirectory(struct dStruct **QuePtr, int timeT, int processID) 
{
    // format is time, PID, status, niceness, cputime, proctime
    int status = 1;
    float cpuTime = 0.00;

    // these are used for opening the directory/folder
    DIR *fldrptr;
    struct dirent* filesInFolder;
    
    // these are used for opening the files
    FILE *in_file;

    // this is used to hold the characters from the file opened
    char line[100], fileName[100], currentPath[100], oneLevelDown[100], newProc[100];
    char readNiceness[50], readProcTime[50];
    char *split;

    // get the pathway one level down from current
    getcwd(currentPath, 100);
    chdir("..");
    getcwd(currentPath, 100);

    // save pathway to newProc, change working directory to newproc
    strcpy(oneLevelDown, currentPath);
    strcpy(newProc, currentPath);
    strcat(newProc, "/newProc");
    chdir("newProc");
    //printf("\nOpening filepath: %s\n\n", getcwd(currentPath, 100));

    // open the directory for newProc folder, check if it opened properly
    fldrptr = opendir(newProc);
    if (fldrptr == NULL)
    {
        printf("Error : Failed to open input directory\n");
        return processID;
    }

    // loop through the directory, opening all the files
    while ((filesInFolder = readdir(fldrptr)))
    {
        // unix/linux iterates through the current/parent directories by default, skip them
        if (!strcmp (filesInFolder->d_name, "."))
        {
            continue;
        }
        if (!strcmp (filesInFolder->d_name, ".."))
        {
            continue;
        }

        // check if file is properly opened
        strcpy(fileName, filesInFolder->d_name);
        if ((in_file = fopen(fileName, "r")) == NULL)
        {
            printf("Error: Failed to open input file\n");
        }

        // gets the first line, split by (, )
        fgets(line, 50, in_file);
        split = strtok(line, ", ");

        // check if split is valid, copy string of proc and niceness
        if (split != NULL)
        {
            strcpy(readProcTime, split);
            split = strtok(NULL, "\n");
            strcpy(readNiceness, split);
        }

        // push to list, also increment processID
        // format(int pid, int status, int niceness, float cputime, float proctime, int timeadded)
		if(ALGOR_SJF == 0){
	        push(QuePtr, processID++, status, atoi(readNiceness), cpuTime, atof(readProcTime), timeT);
    	}
		else{
	        pushSJF(QuePtr, processID++, status, atoi(readNiceness), cpuTime, atof(readProcTime), timeT);
		}
        fclose(in_file);

        // implement remove file after closing here
        if (remove(fileName) == 0)
        {
            printf("File has been processed and successfully removed\n\n");
        }
        else
        {
            printf("Unable to process deleting file\n\n");
        }
    }

    closedir(fldrptr);

    return processID;
}


void copyReadyToRunning(struct dStruct **runningList, struct dStruct **readyList)
{
    // if runninglist is empty and readylist has data
    if(isEmpty(runningList) == 0 && isEmpty(readyList) == 1)
    {
        // copy 1 node from readyList to runninglist********************************************************
        //create new node
        struct dStruct* ptrRunning = (struct dStruct*)malloc(sizeof(struct dStruct));

        // set up ptr to point towards the same head of ready queue list
        ptrRunning -> pid = (*readyList) -> pid;
        ptrRunning -> status = (*readyList) -> status=2;
        ptrRunning -> niceness = (*readyList) -> niceness;
        ptrRunning -> cpuTime = (*readyList) -> cpuTime;
        ptrRunning -> procTime = (*readyList) -> procTime;
        ptrRunning -> tTime = (*readyList) -> tTime;
        ptrRunning -> next = NULL;

        // assign runningList to point at the head of the linked list in Ready list
        *runningList = ptrRunning;
        // copy 1 node from readyList to runninglist********************************************************

        if ((*readyList)->next != NULL)
        {
            (*readyList) = (*readyList)->next;
        }
        else
        {
            *readyList = NULL;
        }

        return;
    }
    return;
}

// check if process is done in this struct
void isTheProcessDone(struct dStruct **runningQueue)
{
    // check if process is done
    if ((*runningQueue)->cpuTime >= (*runningQueue)->procTime)
    {
        // set status as complete=3
        (*runningQueue)->status = 3;
    }

    return;
}

// implement the roundrobin
void roundRobin(struct dStruct **runningList, struct dStruct **readyList)
{
    if (isEmpty(runningList) == 0 || isEmpty(readyList) == 0)
    {
        return;
    }

    push(readyList, (*runningList) -> pid, (*runningList) -> status, (*runningList) -> niceness, (*runningList) -> cpuTime, (*runningList) -> procTime, (*runningList) -> tTime);

    free(*runningList);
    *runningList = NULL;

    return;
}



int main(int argc, char **argv)
{
    // Initialize strucutre for ready and running queue 
    struct dStruct* readyQueue = NULL;
    struct dStruct* runningQueue = NULL;
    int newProcessID=0, cycle=0;


    while (1)
    {
        cycle++;
        printf("*******************************************\n");
        printf("Simulating %d cycle\n", cycle);
        printf("*******************************************\n");
        // open directory and open all files and load files to Ready Queue
        newProcessID = openDirectory(&readyQueue, t, newProcessID);
        printf("Ready Queue have been initialized\n");
        printf("*******************************************\n");

        // check if list need to be organized, organize here
        if (ALGOR_FIFO == ALGOR_SJF)
        {
            // put function here -- Actually, no -- see pushSJF function in openDirectory function
			// rather then resort the list every cycle, the processes are sorted as they're pushed on the running queue
            printf("List organized by proctime\n");
        }

        // copy to running if possible
        copyReadyToRunning(&runningQueue, &readyQueue);        

        // increment time t, iterate through lists and update time, running list cputime is also incremented by 1
        incrementAllT(&runningQueue, readyQueue, t++);

        // sets status for running process can't pop yet, save first
        isTheProcessDone(&runningQueue);

        // save both lists to log
        openlog(readyQueue, runningQueue);
        // print onto user screen
        printf("Ready Queue\n");
        printList(readyQueue);
        printf("Running Queue\n");
        printList(runningQueue);

        // time to pop if process is done
        checkProcess(&runningQueue);

        if(ALGOR_FIFO == ALGOR_RR)
        {
            roundRobin(&runningQueue, &readyQueue);
        }

        // if both lists are empty, break
        if (runningQueue == NULL && readyQueue == NULL)
        {
            break;
        }
    }

    close(newProcessID);


    return 0;
}
