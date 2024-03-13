#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <time.h>

#include "dStruct.h"


// pushes data to the tail of the list
void push(struct dStruct** head_ref, int pid, int status, int niceness, float cpuTime, float procTime, int tTime)
{
	//create new node
	struct dStruct* ptr1 = (struct dStruct*)malloc(sizeof(struct dStruct));
	ptr1 -> pid = pid;
    ptr1 -> status = status;
    ptr1 -> niceness = niceness;
    ptr1 -> cpuTime = cpuTime;
    ptr1 -> procTime = procTime;
    ptr1 -> tTime = tTime;
	ptr1 -> next = NULL;

	//if linked list is not null, set next node
	if (*head_ref == NULL)
    {
        *head_ref = ptr1;
    }
    else
    {
		struct dStruct* temp = *head_ref;
		while (temp->next != NULL)
        {
			temp = temp->next;
        }
		temp->next = ptr1;
	}

    return;
}

void pushSJF(struct dStruct** head_ref, int pid, int status, int niceness, float cpuTime, float procTime, int tTime){
	//create new node
	struct dStruct* ptr1 = (struct dStruct*)malloc(sizeof(struct dStruct));
	ptr1 -> pid = pid;
    ptr1 -> status = status;
    ptr1 -> niceness = niceness;
    ptr1 -> cpuTime = cpuTime;
    ptr1 -> procTime = procTime;
    ptr1 -> tTime = tTime;
	ptr1 -> next = NULL;

	//if linked list is not null, set next node
	if (*head_ref == NULL){
        *head_ref = ptr1;
    }

    else{
		struct dStruct* inode = *head_ref;
		
		if(inode->procTime > procTime){
			ptr1->next = *head_ref;
			*head_ref = ptr1;
			return;
		}
		struct dStruct* next_inode = inode->next;
		while(inode->next != NULL){
			if(next_inode->procTime > procTime){
				ptr1->next = next_inode;
				inode->next = ptr1;
				return;
			}
			inode = inode->next;
			next_inode = next_inode->next;
		}
		inode->next = ptr1;
	}
    return;
}

void printList(struct dStruct* head)
{
	struct dStruct* temp = head;
    if (head != NULL) 
    {
        while(temp != NULL)
        {
            //print out all the data on this pid node
            printf("PID: %d \n", temp -> pid);
            printf("status: %d \n", temp -> status);
            printf("niceness: %d \n", temp -> niceness);
            printf("cpuTime: %.2f \n", temp -> cpuTime);
            printf("procTime: %.2f \n\n", temp -> procTime);
            temp = temp->next;
        }
    }
    printf("\n");
    return;
}

void checkProcess(struct dStruct **runningQueue)
{
    if ((*runningQueue)-> status == 3)
    {
        *runningQueue = NULL;
    }
    return;
}

int isEmpty(struct dStruct** head)
{
    // returns 0 if empty
    if (*head == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// increment t for both structures
void incrementAllT(struct dStruct **runningList, struct dStruct *readyList, int tTime){
    (*runningList) -> cpuTime++;
    (*runningList) -> tTime = tTime;

    struct dStruct *tempReady = readyList;
    
    if (readyList != NULL) {
        while(tempReady->next != NULL){
            tempReady -> tTime = tTime;
            tempReady = tempReady -> next;
        }
        tempReady -> tTime = tTime;
    }
    
    return;
}

void copyReadyToRunning(struct dStruct **runningList, struct dStruct **readyList){
    // if runninglist is empty and readylist has data
    if(isEmpty(runningList) == 0 && isEmpty(readyList) == 1){
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

        if ((*readyList)->next != NULL){
            (*readyList) = (*readyList)->next;
        }
        else{
            *readyList = NULL;
        }
    }
    return;
}

// check if process is done in this struct
void isTheProcessDone(struct dStruct **runningQueue){
    // check if process is done
    if ((*runningQueue)->cpuTime >= (*runningQueue)->procTime){
        // set status as complete=3
        (*runningQueue)->status = 3;
    }

    return;
}

// implement the roundrobin
void roundRobin(struct dStruct **runningList, struct dStruct **readyList){
    if (isEmpty(runningList) == 0 || isEmpty(readyList) == 0){
        return;
    }
    // set status back to 1
    (*runningList)->status = 1;
    push(readyList, (*runningList) -> pid, (*runningList) -> status, (*runningList) -> niceness, (*runningList) -> cpuTime, (*runningList) -> procTime, (*runningList) -> tTime);

    free(*runningList);
    *runningList = NULL;

    return;
}


// function is used to loop adding and removing processes
// from run queue, adding and removing processes from a ready queue
// then updating, not creating a log!!!
// updating log files until the queues are empty
int openlog(struct dStruct *readyList, struct dStruct *runningList){
    // change directory to log
    chdir("..");
    chdir("log");
    
    // these are used for opening the files
    FILE *in_file;

    // open the directory
    DIR *fldrptr = opendir(".");
    // check if directory opened properly
    if (fldrptr == NULL){
        printf("Error : Failed to open input directory\n");

        return 0;
    }

    // get the current date to make log file, student server is 1 month behind....
    time_t t = time (NULL);
    struct tm* lt = localtime (&t);
    int year = lt->tm_year;
    int month = lt->tm_mon;
    int day = lt->tm_mday;
    if (month == 12){
        month = 01;
    }
    else{
        month++;
    }

    char buf[100];
    snprintf(buf, sizeof(buf), "log-%02d-%02d-%d.txt", month, day, year + 1900);

    // logically, if we can't read it, file doesn't exist, so lets create one
    // only time can fail is if someone made the file, before we ran the program
    if ((in_file = fopen(buf, "r")) == NULL){
        in_file = fopen(buf, "w");
        fprintf(in_file, "time    pid   status    niceness    cputime    proctime      Group List\n");
    }
    else{
        in_file = fopen(buf, "a");
    }
    
    struct dStruct* readyTemp = readyList;
    struct dStruct* runningTemp = runningList;

    // cycle through list and write readyList info to log
    if (readyList != NULL){
        do{
            fprintf(in_file, "  %d      %d      %d          %d          %.2f       %.2f        Ready List\n", readyTemp->tTime, readyTemp->pid, readyTemp->status, readyTemp->niceness, readyTemp->cpuTime, readyTemp->procTime);
            readyTemp = readyTemp -> next;
        } while (readyTemp != NULL);
    }

    // cycle through list and write runningList info to log
    if (runningList != NULL){
        do{
            fprintf(in_file, "  %d      %d      %d          %d          %.2f       %.2f        Running List\n", runningList->tTime, runningTemp->pid, runningTemp->status, runningTemp->niceness, runningTemp->cpuTime, runningTemp->procTime);
            runningTemp = runningTemp -> next;
        } while (runningTemp != NULL);
    }

    fclose(in_file);

    closedir(fldrptr);   

    return 0;
}






// void getCpu(struct dStruct* head, int key){
//     struct dStruct* temp = head;

//     if (head != NULL) {
//         do {
//             // if pid is found, print cpuTime for that PID
//             if ((temp) -> pid == key){
//                 printf("Found PID ID, getting cpuTime: %f \n", temp -> cpuTime);
//                 break;
//             }
//             // iterate through the linked list
//             temp = temp -> next;
//         } while (temp != head);
//     }
//     printf("\n");
//     return;
// }

// void setCpu(struct dStruct* head, int key, float cpu){
//     struct dStruct* temp = head;

//     if (head != NULL) {
//         do {
//             // if pid is found, set cpuTime
//             if ((temp) -> pid == key){
//                 printf("Found PID ID, setting cpuTime: %f \n", cpu);
//                 temp -> cpuTime = cpu;
//                 break;
//             }
//             // iterate through the linked list
//             temp = temp -> next;
//         } while (temp != head);
//     }
//     printf("\n");
    
//     return;
// }

// void getNiceness(struct dStruct* head, int key){
//     struct dStruct* temp = head;

//     if (head != NULL) {
//         do {
//             // if pid is found, print niceness for that PID
//             if ((temp) -> pid == key){
//                 printf("Found PID ID %d, getting niceness: %d \n",temp -> pid, temp -> niceness);
//                 break;
//             }
//             // iterate through the linked list
//             temp = temp -> next;
//         } while (temp != head);
//     }
//     printf("\n");
//     return;
// }

// // can probably combine set/get niceness and other functions. time constraint
// void setNiceness(struct dStruct* head, int key, int nice){
//     struct dStruct* temp = head;

//     if (head != NULL) {
//         do {
//             // if pid is found, set niceness
//             if ((temp) -> pid == key){
//                 printf("Found PID ID, setting niceness: %d \n", nice);
//                 temp -> niceness = nice;
//                 break;
//             }
//             // iterate through the linked list
//             temp = temp -> next;
//         } while (temp != head);
//     }
//     printf("\n");
    
//     return;
// }

// void getStatus(struct dStruct* head, int key){
//     struct dStruct* temp = head;

//     if (head != NULL) {
//         do {
//             // if pid is found, print status for that PID
//             if ((temp) -> pid == key){
//                 printf("Found PID ID, getting status: %d \n", temp -> status);
//                 break;
//             }
//             // iterate through the linked list
//             temp = temp -> next;
//         } while (temp != head);
//     }
//     printf("\n");
//     return;
// }

// void setStatus(struct dStruct* head, int key, int status){
//     struct dStruct* temp = head;

//     if (head != NULL) {
//         do {
//             // if pid is found, set status
//             if ((temp) -> pid == key){
//                 printf("Found PID ID, setting status: %d \n", status);
//                 temp -> status = status;
//                 break;
//             }
//             // iterate through the linked list
//             temp = temp -> next;
//         } while (temp != head);
//     }
//     printf("\n");
    
//     return;
// }
// void searchByEntry(struct dStruct* head, int key){
//     struct dStruct* temp = head;

//     if (head != NULL) {
//         do {
//             // if pid is found, print status for that PID
//             if ((temp) -> pid == key){
//             printf("PID: %d \n", temp -> pid);
//             printf("status: %d \n", temp -> status);
//             printf("niceness: %d \n", temp -> niceness);
//             printf("cpuTime: %f \n", temp -> cpuTime);
//             printf("procTime: %f \n\n\n", temp -> procTime);
//             break;
//             }
//             // iterate through the linked list
//             temp = temp -> next;
//         } while (temp != head);
//     }
//     printf("\n");
//     return;
// }