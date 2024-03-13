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
void printList(struct dStruct* head);
void deleteNode(struct dStruct** head, int key);
void popfront(struct dStruct** head);
void getCpu(struct dStruct* head, int key);
void setCpu(struct dStruct* head, int key, float cpu);
void getNiceness(struct dStruct* head, int key);
void setNiceness(struct dStruct* head, int key, int nice);
void getStatus(struct dStruct* head, int key);
void setStatus(struct dStruct* head, int key, int status);
void searchByEntry(struct dStruct* head, int key);
int isEmpty(struct dStruct** head);

#endif
