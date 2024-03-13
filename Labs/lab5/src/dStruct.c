#include <stdio.h>
#include <stdlib.h>
#include "dStruct.h"


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


//deletes by pid
void deleteNode(struct dStruct** head, int key) { 

    // If linked list is empty 
    if (*head == NULL) 
        return; 
  
    // If the list contains only a single node 
    if ((*head) -> pid == key && (*head)->next == *head) {
        free(*head);
        *head = NULL;
        return;
    }

    struct dStruct *last = *head, *d;

    // If head is to be deleted
    if ((*head) -> pid == key) {

        // Find the last node of the list
        while (last -> next != *head)
            last = last -> next;

        // get last node in list, then point to the second one, then pop the first
        last -> next = (*head) -> next;
        free(*head);
        *head = last -> next;
        return;
    }

    // PID is not found, iterating through list
    while (last -> next != *head && last -> next -> pid != key) {
        last = last -> next;
    }
  
    // If node to be deleted was found
    if (last -> next -> pid == key) {
        d = last -> next;
        last -> next = d -> next;
        free(d);
    }
    else{
        printf("Given PID is not found in the list!\n");
	}
    return;
}

//deletes at the last data structure pushed
void popfront(struct dStruct** head){
    
    // If linked list is empty 
    if (*head == NULL) 
        return; 

    // If the list contains only a single node 
    if ((*head)->next == *head) {
        free(*head);
        *head = NULL;
        return;
    }

    struct dStruct *last = *head;

    //Find the last node of the list
    while (last -> next != *head){
        last = last -> next;
    }
    // Now point the last node to the second node of our list
    last -> next = (*head) -> next;
    free(*head);
    *head = last -> next;
    return;
}


void getCpu(struct dStruct* head, int key){
    struct dStruct* temp = head;

    if (head != NULL) {
        do {
            // if pid is found, print cpuTime for that PID
            if ((temp) -> pid == key){
                printf("Found PID ID, getting cpuTime: %f \n", temp -> cpuTime);
                break;
            }
            // iterate through the linked list
            temp = temp -> next;
        } while (temp != head);
    }
    printf("\n");
    return;
}

void setCpu(struct dStruct* head, int key, float cpu){
    struct dStruct* temp = head;

    if (head != NULL) {
        do {
            // if pid is found, set cpuTime
            if ((temp) -> pid == key){
                printf("Found PID ID, setting cpuTime: %f \n", cpu);
                temp -> cpuTime = cpu;
                break;
            }
            // iterate through the linked list
            temp = temp -> next;
        } while (temp != head);
    }
    printf("\n");
    
    return;
}

void getNiceness(struct dStruct* head, int key){
    struct dStruct* temp = head;

    if (head != NULL) {
        do {
            // if pid is found, print niceness for that PID
            if ((temp) -> pid == key){
                printf("Found PID ID %d, getting niceness: %d \n",temp -> pid, temp -> niceness);
                break;
            }
            // iterate through the linked list
            temp = temp -> next;
        } while (temp != head);
    }
    printf("\n");
    return;
}

// can probably combine set/get niceness and other functions. time constraint
void setNiceness(struct dStruct* head, int key, int nice){
    struct dStruct* temp = head;

    if (head != NULL) {
        do {
            // if pid is found, set niceness
            if ((temp) -> pid == key){
                printf("Found PID ID, setting niceness: %d \n", nice);
                temp -> niceness = nice;
                break;
            }
            // iterate through the linked list
            temp = temp -> next;
        } while (temp != head);
    }
    printf("\n");
    
    return;
}

void getStatus(struct dStruct* head, int key){
    struct dStruct* temp = head;

    if (head != NULL) {
        do {
            // if pid is found, print status for that PID
            if ((temp) -> pid == key){
                printf("Found PID ID, getting status: %d \n", temp -> status);
                break;
            }
            // iterate through the linked list
            temp = temp -> next;
        } while (temp != head);
    }
    printf("\n");
    return;
}

void setStatus(struct dStruct* head, int key, int status){
    struct dStruct* temp = head;

    if (head != NULL) {
        do {
            // if pid is found, set status
            if ((temp) -> pid == key){
                printf("Found PID ID, setting status: %d \n", status);
                temp -> status = status;
                break;
            }
            // iterate through the linked list
            temp = temp -> next;
        } while (temp != head);
    }
    printf("\n");
    
    return;
}

void searchByEntry(struct dStruct* head, int key){
    struct dStruct* temp = head;

    if (head != NULL) {
        do {
            // if pid is found, print status for that PID
            if ((temp) -> pid == key){
            printf("PID: %d \n", temp -> pid);
            printf("status: %d \n", temp -> status);
            printf("niceness: %d \n", temp -> niceness);
            printf("cpuTime: %f \n", temp -> cpuTime);
            printf("procTime: %f \n\n\n", temp -> procTime);
            break;
            }
            // iterate through the linked list
            temp = temp -> next;
        } while (temp != head);
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
