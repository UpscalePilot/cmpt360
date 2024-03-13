#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <time.h>

#include "dStruct.h"

void completedProc(struct dStruct *runningList)
{

	struct dStruct* runningTemp = runningList;
    if (runningTemp != NULL) 
    {
        runningTemp -> status = 3;
    }
    return;
}

void incrementAllT(struct dStruct *runningList, struct dStruct *readyList, int tTime)
{
    // we will increment the time for cputime also
	struct dStruct* runningTemp = runningList;
    (runningTemp->cpuTime)++;
    runningTemp->tTime = tTime;
    runningTemp->next = NULL;

	struct dStruct* readyTemp = readyList;
    
    if (readyList != NULL) 
    {
        while(readyTemp != NULL)
        {
            readyTemp->tTime = tTime;
            readyTemp = readyTemp->next;
        }
    }
    
    return;
}

// function is used to loop adding and removing processes
// from run queue, adding and removing processes from a ready queue
// then updating, not creating a log!!!
// updating log files until he queues are empty
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

    // get the current date to make log file
    time_t t = time (NULL);
    struct tm* lt = localtime (&t);
    int year = lt->tm_year;
    int month = lt->tm_mon + 1;
    int day = lt->tm_mday;

    char buf[0x100];
    snprintf(buf, sizeof(buf), "log-%02d-%02d-%d.txt", month, day, year + 1900);

    // logically, if we can't read it, file doesn't exist, so lets create one
    // only time can fail is if someone made the file, before we ran the program
    if ((in_file = fopen(buf, "r")) == NULL)
    {
        in_file = fopen(buf, "w");
        fprintf(in_file, "  time    pid   status    niceness    cputime    proctime      Group List\n");
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
            fprintf(in_file, "  %2d      %2d      %d          %d          %.2f       %.2f        Ready List\n", readyTemp->tTime, readyTemp->pid+1, readyTemp->status, readyTemp->niceness, readyTemp->cpuTime, readyTemp->procTime);
            readyTemp = readyTemp -> next;
        } while (readyTemp != NULL);
    }

    // cycle through list and write runningList info to log
    if (runningList != NULL)
    {
        do
        {
            fprintf(in_file, "  %2d      %2d      %d          %d          %.2f       %.2f        Running List\n", runningList->tTime, runningTemp->pid+1, runningTemp->status, runningTemp->niceness, runningTemp->cpuTime, runningTemp->procTime);
            runningTemp = runningTemp -> next;
        } while (runningTemp != NULL);
    }

    fclose(in_file);

    closedir(fldrptr);

    return 0;
}


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
        push(QuePtr, processID++, status, atoi(readNiceness), cpuTime, atof(readProcTime), timeT);
        
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

void dereferenceHead(struct dStruct** head)
{
    // If readyqueue only has a single node 
    if ((*head)-> next == NULL)
    {
        *head = NULL;
        return;
    }

    // else have the list point to the second one
    struct dStruct *temp = *head;
    *head = temp->next;

    return;
}

int cpySecondToFirst(struct dStruct **runningList, struct dStruct *readyList, int t)
{

    // if runninglist is empty, but readylist is occupied
    if(isEmpty(runningList) == 0 && readyList != NULL)
    {
        //create new node
        struct dStruct* ptrTo = (struct dStruct*)malloc(sizeof(struct dStruct));
        struct dStruct* tempReadyList = readyList;

        // setting the node status to 2=running
        tempReadyList->status = 2;

        // set up ptr to point towards the same head of ready queue list
        ptrTo -> pid = tempReadyList -> pid;
        ptrTo -> status = tempReadyList -> status;
        ptrTo -> niceness = tempReadyList -> niceness;
        ptrTo -> cpuTime = tempReadyList -> cpuTime;
        ptrTo -> procTime = tempReadyList -> procTime;
        ptrTo -> tTime = tempReadyList -> tTime;
        ptrTo -> next = NULL;

        // assign runningList to point at the head of the linked list in Ready list
        *runningList = ptrTo;
        return 1;
    }

    // Runninglist has a process
    else if (isEmpty(runningList) == 1)
    {
        return 2;
    }

    return 0;
}

int isTheProcessDone(struct dStruct* head)
{
    struct dStruct* temp = head;

    // (temp->tTime) >= (temp->cpuTime) seemed unnecessary
    if ((temp->cpuTime) >= (temp->procTime))
    {
        // set status as complete=3
        temp->status = 3;
        return 1;
    }

    return 0;
}


//global time (t) for time increments
int t = 0;

int main(int argc, char **argv)
{
    // Initialize strucutre for ready and running queue 
    struct dStruct* readyQueue = NULL;
    struct dStruct* runningQueue = NULL;
    int newProcessID=0, runningListFull, popRunningList, cycle=0;

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

        // printf("Ready Queue\n");
        // printList(readyQueue);
        // printf("Running Queue\n");
        // printList(runningQueue);

        // return 0, failsafe, technically if both process is empty.
        // return 1, RunningQueue points to ReadyQueue head
        // return 2, RunningQueue has data to process
        runningListFull = cpySecondToFirst(&runningQueue, readyQueue, t);


        // since running queue points to head, get ready queue point to the second
        if (runningListFull==1){
            dereferenceHead(&readyQueue);
        }

        // increment time t, iterate through lists and update time, running list cputime is also incremented by 1
        incrementAllT(runningQueue, readyQueue, t++);
        
        // returns 1 if cputime >= proctime, set status to 3
        popRunningList = isTheProcessDone(runningQueue);
        if (popRunningList==1)
        {
            completedProc(runningQueue);
        }

        // save both lists to log
        openlog(readyQueue, runningQueue);

        //pop running list if cputime >= proctime
        if (popRunningList==1)
        {
            popfront(&runningQueue);
        }
        // if both lists are empty, break
        if (runningQueue == NULL && readyQueue == NULL){
            break;
        }
        
        // print onto user screen
        printf("Ready Queue\n");
        printList(readyQueue);
        printf("Running Queue\n");
        printList(runningQueue);
    }

    return 0;
}
