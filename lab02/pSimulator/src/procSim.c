/*
#####################################################################
#           CMPT 360    - Lab 2                      		        #
#           Ben Morley  - ID 3142870								#
#			Nabil Gelle - ID 3104375                                #
#                                                                   #
#####################################################################
*/

//HEADERS
#include<stdio.h>
#include"dStruct.h"
#include<stdlib.h>
#include<time.h>

//PROTOTYPES
void create_proc(sortedLL*, int, float);
void read_file(sortedLL*, char*);
void check_proc(sortedLL*, sortedLL*, time_t*);


//MAIN
int main(int argc, char* argv[]){
	sortedLL* ready = list_init(NULL, 0); 			// initializes ready queue
	sortedLL* running = list_init(NULL, 0); 		// initializes running queue
	
	time_t current_time; 		//stores current time
	time_t elapsed_time; 		//stores elapsed time of the current proc
	time_t start_time; 			//stores the start time of the current proc
	time(&start_time); 			//gets a starting time
	time(&current_time); 		//gets a value for current time


	//TEST CODE - just for checking functions
	char* filename = "newProc/first.txt"; //just for testing
	
	for(int x = 0; x < 6; x++){ 		//loop grabs a few processes from file for testing
		read_file(ready, filename);
	}


	//END TEST CODE

	add_node(running, pop_node(ready)); 		//adds a node from the ready queue to the running queue
	int current_pid = running->head->process->pid; 		//gets the pid of the running proc

	//loops till there is no more processes to simulate
	while(running->head != NULL){
		current_pid = running->head->process->pid; 		//gets the current pid
		
		time(&current_time); 		//gets the current time
		elapsed_time = current_time - start_time; 		//calculates elapsed time
		
		set_cputime(running, current_pid, (float)elapsed_time); 		//sets cpu time of current proc
		

		check_proc(running, ready, &start_time); // checks if process is complete and gets new proc if neccisary
	}

	

}


//This function creates a new process for a provided niceness and proctime
void create_proc(sortedLL* list, int niceness, float proctime){
	int pid = 0; 			//initializes a pid
	int status = 1; 		//initializes a status
	float cputime = 0.0f;   //initializes a cputime

	//initializes a new process, stores the new process in a new node, and pushes the node to the ready queue
	processEntry* new_proc = entry_init( pid, status, niceness, cputime, proctime);
	node* new_node = node_init( new_proc);
	push_node(list, new_node);
}


//reads a new process from file
void read_file(sortedLL* list, char* filename){
	FILE* fread = fopen(filename, "r"); 		//opens file for reading
	if (fread == NULL){ 		//if error opening file, exit failure
		exit(EXIT_FAILURE);
	}
	
	//initialises variables for proctime and niceness
	float proctime;
	int niceness;
	
	fscanf(fread, "%f, %i", &proctime, &niceness); 		//reads data from file

	create_proc(list, niceness, proctime);  //creates a process for data read from file
}


//checks process status and handles process completion
void check_proc(sortedLL* running, sortedLL* ready, time_t* start){
	//check if process has been completed
	if(running->head->process->cputime >= running->head->process->proctime){
		set_status(running, running->head->process->pid, 3); 	//sets status to 3 for complete process
		delete_node(running, running->head->process->pid); //deletes completed process

		//if ready queue has another process
		if(ready->head != NULL){
			add_node(running, pop_node(ready));  	//adds new process to running queue from ready queue
			set_status(running, running->head->process->pid, 2); 	//sets status to running(2)
			time(start); 			//gets a new start time for the new process
			print_all(running); 	//prints running queue 
			print_all(ready); 		//prints ready queue
		}
	}
}
