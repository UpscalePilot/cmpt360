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

#ifndef TIME_QUANTUM
#define TIME_QUANTUM 4
#endif

#ifndef TIME_RESET
#define TIME_RESET 16
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
    while ((filesInFolder = readdir(fldrptr)) != NULL){
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
            printf("File %s has been processed(pid %d) and successfully removed\n", fileName, (processID-1));
        }
        else{
            printf("Unable to process deleting file %s\n\n", fileName);
        }
    }

    closedir(fldrptr);

    return processID;
}

void print_all(mlfq* mlfqQueue, struct dStruct* readyQueue, struct dStruct* runningQueue){
    printf("\n\n\nTQ\n");
    printf("\tP5\n");
    printList(mlfqQueue->p5);
    printf("\tP4\n");
    printList(mlfqQueue->p4);
    printf("\tP3\n");
    printList(mlfqQueue->p3);
    printf("\tP2\n");
    printList(mlfqQueue->p2);
    printf("\tP1\n");
    printList(mlfqQueue->p1);
    printf("\tReady Queue:\n");
    printList(readyQueue);
    printf("\tRunning Queue:\n");
    printList(runningQueue);

    return;
}



void mlfq_check(mlfq* queues){
    if(queues->p5 != NULL){
        if(queues->p5->status == 3){
            free(queues->p5);
            queues->p5 = NULL;
            
        }
    }
    if(queues->p4 != NULL){
        if(queues->p4->status == 3){
            free(queues->p5);
            queues->p4 = NULL;
            
        }
    }
    if(queues->p3 != NULL){
        if(queues->p3->status == 3){
            free(queues->p5);
            queues->p3 = NULL;
            
        }
    }
    if(queues->p2 != NULL){
        if(queues->p2->status == 3){
            free(queues->p5);
            queues->p2 = NULL;
            
        }
    }
    if(queues->p1 != NULL){
        if(queues->p1->status == 3){
            free(queues->p5);
            queues->p1 = NULL;
            
        }
    }


    return;
}


void mlfqToReady(mlfq* mlfqQueue, struct dStruct** readyQueue, int* priority){
    mlfq_check(mlfqQueue);
    if((*readyQueue) == NULL){    
        if(mlfqQueue->p5 != NULL){
            //(*readyQueue) = mlfqQueue->p5;
            queue_mv(readyQueue, &mlfqQueue->p5);
            *priority = 5;
            return;
        }
        if(mlfqQueue->p4 != NULL){
            //(*readyQueue) = mlfqQueue->p4;
            queue_mv(readyQueue, &mlfqQueue->p4);
            *priority = 4;
            return;
        }
        if(mlfqQueue->p3 != NULL){
            //(*readyQueue) = mlfqQueue->p3;
            queue_mv(readyQueue, &mlfqQueue->p3);
            *priority = 3;
            return;
        }
        if(mlfqQueue->p2 != NULL){
            //(*readyQueue) = mlfqQueue->p2;
            queue_mv(readyQueue, &mlfqQueue->p2);
            *priority = 2;
            return;
        }
        if(mlfqQueue->p1 != NULL){
            // (*readyQueue) = mlfqQueue->p1;
            queue_mv(readyQueue, &mlfqQueue->p1);
            *priority = 1;
            return;
        }
    }
}


int main(void){
    // Initialize structure for ready and running queue
    struct dStruct* readyQueue = NULL;
    struct dStruct* runningQueue = NULL;
    struct dStruct* readInQueue = NULL;
    int newProcessID=0;
    char* algor = (char*)malloc(sizeof(char) *5);

    mlfq* mlfqQueue = (mlfq*)malloc(sizeof(struct mlfq));
    int last_quantum = t;
    int last_reset = t;
    int current_priority = 5;

    if(ALGOR_FIFO == 1){
        strcpy(algor, "FIFO");
    }
    else if(ALGOR_SJF == 1){
        strcpy(algor, "SJF");
    }
    else if(ALGOR_MLFQ == 1){
        strcpy(algor, "MLFQ");
    }
    else{
        strcpy(algor, "RR");
    }


    while (1){

        // open directory and open all files and load files to Ready Queue
        newProcessID = openDirectory(&readInQueue, t, newProcessID);

        if(newProcessID != 0){      // pretects against errors if no processes were read in

            if(readInQueue != NULL){
                if(ALGOR_MLFQ == 1){
                    mlfqSort(mlfqQueue, readInQueue);
                    readInQueue = NULL;
                    mlfqToReady(mlfqQueue, &readyQueue, &current_priority);
                }
                else{
                    readyQueue = readInQueue;
                    readInQueue = NULL;
                }
            }

            // copy to running if possible
            //copyReadyToRunning(&runningQueue, &readyQueue);   
            new_proc_run(&runningQueue, &readyQueue);

            // increment time t, iterate through lists and update time, running list cputime is also incremented by 1
            incrementAllT(&runningQueue, readyQueue, t++);
            
            // sets status for running process can't pop yet, save first
            isTheProcessDone(&runningQueue);

            // save both lists to log
            openlog(readyQueue, runningQueue, algor);
            
            // time to pop if process is done
            checkProcess(&runningQueue);

            if(ALGOR_MLFQ == 1){
                if((t - last_reset) == TIME_RESET){
                    last_reset = t;
                    last_quantum = t;
                    queue_mv(&readInQueue, &runningQueue);
                    queue_mv(&readInQueue, &readyQueue);
                    queue_mv(&readInQueue, &mlfqQueue->p5);
                    queue_mv(&readInQueue, &mlfqQueue->p4);
                    queue_mv(&readInQueue, &mlfqQueue->p3);
                    queue_mv(&readInQueue, &mlfqQueue->p2);
                    queue_mv(&readInQueue, &mlfqQueue->p1);
                    mlfqSort(mlfqQueue, readInQueue);
                    readInQueue = NULL;
                    
                    //printf("TIME RESET at t=%d\n", t);
                    //print_all(mlfqQueue, readyQueue, runningQueue);


                }
                else if((t - last_quantum) == TIME_QUANTUM){
                    last_quantum = t;
                    //current_priority--;
                    
                    mlfq_roundRobin(mlfqQueue, &runningQueue, current_priority);

                    //printf("TIME QUANTUM at t=%d\n", t);
                    

                }

                if(readyQueue == NULL){
                    mlfqToReady(mlfqQueue, &readyQueue, &current_priority);
                }
            }
            else if (ALGOR_RR == 1){
                roundRobin(&runningQueue, &readyQueue);
            }


        }
        // if both lists are empty, break


        if (runningQueue == NULL && readyQueue == NULL){
            break;
        }
    }
    close(newProcessID);

    //print_all(mlfqQueue, readyQueue, readInQueue);

    return 0;
}