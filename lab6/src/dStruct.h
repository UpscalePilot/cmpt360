#ifndef DSTRUCT_H
#define DSTRUCT_H

struct dStruct{

	int pid;
	int status;
	int niceness;
	float cpuTime;
	float procTime;
	int tTime;
    struct dStruct* next;

};

void push(struct dStruct** head_ref, int pid, int status, int niceness, float cpuTime, float procTime, int tTime);
void pushSJF(struct dStruct** head_ref, int pid, int status, int niceness, float cpuTime, float procTime, int tTime);
void printList(struct dStruct* head);
void checkProcess(struct dStruct **runningQueue);
int isEmpty(struct dStruct** head);
void incrementAllT(struct dStruct **runningList, struct dStruct *readyList, int tTime);
void copyReadyToRunning(struct dStruct **runningList, struct dStruct **readyList);
void isTheProcessDone(struct dStruct **runningQueue);
void roundRobin(struct dStruct **runningList, struct dStruct **readyList);
int openlog(struct dStruct *readyList, struct dStruct *runningList);

// void getCpu(struct dStruct* head, int key);
// void setCpu(struct dStruct* head, int key, float cpu);
// void getNiceness(struct dStruct* head, int key);
// void setNiceness(struct dStruct* head, int key, int nice);
// void getStatus(struct dStruct* head, int key);
// void setStatus(struct dStruct* head, int key, int status);
// void searchByEntry(struct dStruct* head, int key);

#endif