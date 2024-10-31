#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>

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

void popfront(struct dStruct** head)
{
    // If the list contains only a single node
    if ((*head)-> next == NULL)
    {
        free(*head);
        *head = NULL;
        return;
    }

    return;
}

int isEmpty(struct dStruct** head)
{
    // returns 0 if empty, else return 1
    if (*head == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


