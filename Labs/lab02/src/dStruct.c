/*
#####################################################################
#           CMPT 360    - Lab 1                      		        #
#           Ben Morley  - ID 3142870								#
#			Nabil Gelle - ID 3104375                                #
#                                                                   #
#####################################################################
*/


//HEADERS
#include<stdio.h>
#include<stdlib.h>
#include"dStruct.h"


//Creation of a processEntry
processEntry* entry_init(int pid, int status, int niceness, float cputime, float proctime){
	processEntry* new_proc = malloc(sizeof(processEntry));		//initializes memory for process entry

	// Stores data specified in paramters
	new_proc->pid = pid;
	new_proc->status = status;
	new_proc->niceness = niceness;
	new_proc->cputime = cputime;
	new_proc->proctime = proctime;

	return new_proc;		// returns pointer to processEntry
}


//Prints out an individual process entry
void print_entry(processEntry* process){
	printf("\tPID         : %i\n", process->pid);
	printf("\tStatus      : %i\n", process->status);
	printf("\tNiceness    : %i\n", process->niceness);
	printf("\tCPU Time    : %.2f\n", process->cputime);
	printf("\tProcess Time: %.2f\n", process->proctime);
	printf("\n\n");

	return;
}


// Initializes a node in the sorted linked list
node* node_init(processEntry* process){			//takes pointer to process entry as parameter
	node* new_node = malloc(sizeof(node));		//initializes memory to store data
	
	new_node->process = process;		//stores pointer in node
	new_node->next = NULL;				//defaults the pointer to next node as NULL

	return new_node;		//returns pointer to node
}

// Initializes a sorted linked list
sortedLL* list_init(node* head, int size){				//takes an initial node and size as parameters
	sortedLL* new_list = malloc(sizeof(sortedLL));	//initializes memory to store data

	new_list->head = head;		//stores the pointer to the front of the list
	new_list->size = size;		//stores the size of the list

	return new_list;	//returns the pointer to the list
}


// Prints all entries stored in the linked list
void print_all(sortedLL* list){
	printf("size: %i\n", list->size);		//prints out size of list
	node* inode = list->head;			//inode (iterative node) to store each node as the list is traversed. Initial value is the front of the list

	for(int x = 0; inode != NULL; x++){		//loops through each node in the list, x is a counting variable for clarity in printing
		printf("Entry %i:\n",x);		//tells the user the entry number
		print_entry(inode->process);		//prints the process entry
		inode = inode->next;			//moves on to the next node in the list
	}
	return;
}


//adds node in order to sorted list
void add_node(sortedLL* list, node* new_node){		//takes pointer to linked list and pointer to the new node as parameters
	
	//special case if list is empty
	if(list->size == 0){
		list->head = new_node;	//sets new node to the head/front of the list
		list->size = 1;			//increments size of the list
		return;					//ends function here
	}
	
	int new_pid = new_node->process->pid;		//stores the pid of the new node as nodes are sorted by pid
	
	node* inode = list->head;			//inode - iterative node stores each node as the list is traversed
	if(inode->process->pid > new_pid){		//if the pid of the new node is smaller than the existing first node
		new_node->next = list->head;		//The old front of the list is moved to the next spot after the new node
		list->head = new_node;				//The new node is moved to the front/head of the list
		list->size += 1;					//size is incremented 
		return;								//function ends here
	}
	else if (list->size == 1){		//special case if the list only contains 1 element (and the new node isn't smaller than the head node)
		inode->next = new_node;		//adds new node to the back of the list
		list->size += 1;			//increments list size
	}
	else{									//for all other cases:
		node* ninode = inode->next;			//initializes ninode (next iterative node) stores the node that comes after inode
		while(ninode != NULL){				//loops till the ninode reaches the end of the list
			if(ninode->process->pid > new_pid){		//checks if the new node pid is smaller than the ninode
				inode->next = new_node;		//inserts the new node between the inode and the ninode 
				new_node->next = ninode;
				list->size += 1;			//increments the size of the list
				return;						//ends function here
			}
			inode = inode->next;		//increments the iterative node
			ninode = ninode->next;		//increments the next iterative node
		}
		inode->next = new_node;		//if the new node has the largest pid, its added on the end to the last node
		list->size +=1;				//increments size of list
	}
	return;
}


//Deletes nodes from the linked list
int delete_node(sortedLL* list, int pid){		//takes the linked list and pid as parameters
	if(list->size == 0){		//if the linked list is empty returns a 1 for failure
		return 1;
	}
	else if(list->size == 1 && list->head->process->pid != pid){		//if there is only 1 node in the list and it doesn't match the pid to be deleted
			return 1;		//1 returned for failure
	}

	if (list->head->process->pid == pid){		//special case if the head/front node is to be deleted
		node* remove = list->head;		//stores the node to be removed
		if(list->size > 1){				//if there are other nodes in the list
			list->head = remove->next;		//the next node is moved to the head/front of the list
			list->size -= 1;				//size is decremented
		}
		else{				//if the only node is the node to be deleted
			list->head = NULL;		//sets the head to NULL
			list->size = 0;			//sets the size to 0
		}
		free(remove->process);		//frees memory used to store the process entry
		free(remove);				//frees memory used to store the node
		return 0;		//return 0 for successful node deletion 
	}

	
	node* inode = list->head;				//initializes the iterative node
	node* ninode = inode->next;				//initializes the next iterative node
	int nipid = ninode->process->pid;		//stores the pid of the next iterative node
	while(ninode != NULL && nipid <= pid){		//loops through till the end of the list or till the possible location of the pid is passed over since the list is sorted
		if(nipid == pid){				//if the next iterative pid is the same as the pid to be deleted
			inode->next = ninode->next;		//sets the iterative node to skip the next iterative node and go directly to the next iterative node's next node
			list->size -= 1;				//decrements the size of the node
			free(ninode->process);			//frees memory used to store the process entry
			free(ninode);					//frees memory used to store the node
			return 0;						//return 0 for success
		}
		
		inode = inode->next;		//iterates the iterative node
		ninode = ninode->next;		//iterates the next iterative node
		nipid = ninode->process->pid;		//stores the pid of the new next iterative node
	}

	return 1;		//If this point in the code is reached, the given pid was not found and 1 is returned for failure 

}


//searches list for a node specified by pid
node* node_search(sortedLL* list, int pid){		//takes the linked list and the pid as parameters
	if(list->size == 0){		//if the size is 0
		return NULL;			//return null since the node doesn't exist
	}

	node* inode = list->head;			//initializes the iterative node
	int ipid = inode->process->pid;		//stores the pid of the iterative node
	while(inode != NULL && ipid <= pid){		//loops through each node in the list or till the location of that pid is passed in the list
		if(ipid == pid){		//if the pid of the iterative node matches the pid being searched for
			return inode;		//returns the node 
		}
		inode = inode->next;		//iterates the iterative node
		ipid = inode->process->pid;	//stores the new pid for the new iterative node
	}

	return NULL;		// if no node is found, null is returned
}


//searches for a given niceness
int nice_search(sortedLL* list, int nice){			//takes the list and the given niceness as parameters
	if(list->size == 0){		//if the list is empty, 0 returned for boolean FALSE
		return 0;
	}

	node* inode = list->head;					//initializes the iterative node
	int inice = inode->process->niceness;		//stores the niceness of the iterative node
	while(inode != NULL){		//loops through till the end of the list
		if(inice == nice){		//checks if the niceness matches
			return 1;			//returns 1 if there's a match for boolean TRUE
		}
		inode = inode->next;			//iterates the iterative node
		inice = inode->process->niceness;		//stores the new niceness for the new iterative node
	}

	return 0;		//returns 0 if the niceness was not found
}


//searches for a given status
int status_search(sortedLL* list, int status_key){		//takes the list and given status as parameters
	if(list->size == 0){		//if the list list is empty, 0 returned for boolean FALSE
		return 0;
	}

	node* inode = list->head;					//initializes the iterative node 
	int istatus = inode->process->status;		//stores the status of the iterative node
	while(inode != NULL){						//loops through each node of the list
		if(istatus == status_key){		//if the status matches the given status, 1 is returned for boolean TRUE
			return 1;
		}
		inode = inode->next;		//iterates the iterative node
		istatus = inode->process->status;		//stores the new status of the new iterative node
	}

	return 0;		//returns 0 for boolean FALSE if status note found
}


// returns just the processEntry (note the whole node) for a give pid
processEntry* process_search(sortedLL* list, int pid){			//takes the linked list and the pid as paramters
	node* target = node_search(list, pid);	//searches for node in list
	
	if(target == NULL){		//if node was not found
		return NULL;		//return NULL
	}
	return target->process; //returns just the pointer to the processEntry if found
}


//gets the cputime for a give process id
float get_cputime(sortedLL* list, int pid){				//takes the linked list and pid as parameters
	processEntry* proc = process_search(list, pid);		//gets process entry that matches pid

	if(proc == NULL){		//return -1 if process entry does not exit
		return (-1);
	}
	return proc->cputime;		// returns value of cpu time for give pid
}


//sets the cpu time for a give pid
void set_cputime(sortedLL* list, int pid, float cputime_change){		//takes the linked list, the pid, and the value the cputime is changing to as parameters
	processEntry* proc = process_search(list, pid);		//finds process entry for given pid

	if(proc == NULL){		//if process entry not found, function just ends here
		return;
	}
	
	proc->cputime = cputime_change;		//sets cpu time for the given process

	return;
}


//gets the niceness for a given process id
int get_niceness(sortedLL* list, int pid){			//takes the linked list and the pid as parameters
	processEntry* proc = process_search(list, pid);		//searches for given pid

	if(proc == NULL){		//if process is not found, -1 returned
		return (-1);
	}
	return proc->niceness;		//otherwise niceness is returned
}


//sets the niceness for a give process id
void set_niceness(sortedLL* list, int pid, int nice_change){		//linked list, pid and the new niceness as paramters
	processEntry* proc = process_search(list, pid);		//searches for entry with given pid

	if(proc == NULL){		//if entry not found, function ends
		return;
	}

	proc->niceness = nice_change;		//sets niceness if entry is found
	return;	
}


//gets the status for a given pid
int get_status(sortedLL* list, int pid){			//linked list and pid as parameters
	processEntry* proc = process_search(list, pid);		//searches for entry with pid

	if(proc == NULL){		//return -1 if entry not found
		return (-1);
	}
	return proc->status;		//returns status
}


//sets the status for a given pid
void set_status(sortedLL* list, int pid, int status_change){		//linked list, pid and new status as parameters
	processEntry* proc = process_search(list, pid);		//searches for entry with pid

	if(proc == NULL){		//if entry not found, function ends
		return;
	}

	proc->status = status_change;		//changes status
	return;
}


//destroys the entire linked list and frees memory
void destroy_list(sortedLL* list){		//linked list as parameters
	
	while(list->size > 0){		//loops till list is empty
		delete_node(list, list->head->process->pid);		//deletes the head node from the list
	}
	free(list);		//frees memory used for list
	return;
}


// prints all the process entries that have a given status
void print_status(sortedLL* list, int key){				//linked list as well as search key (status) as parameters
	sortedLL* status_list = list_init(NULL, 0);		//creates a new linked list

	node* inode = list->head;		//initializes an iterative node
	while(inode != NULL){			//loops through each node of the original list
		if(inode->process->status == key){				//if the node status matches the search key status
			node* new_node = node_init(inode->process);		//creates a new node for the status list
			add_node(status_list, new_node);				//adds new node to new status list
		}
		inode = inode->next;		//iterates the iterative node
	}
	
	printf("All processes with the status %i:\n", key);		//Tells the user what is being printed out
	print_all(status_list);		//prints all entries in the new status list 
	
	node* dnode = status_list->head;			//iterative node for deleting nodes from the new status list (without deleting the process entries from the original list)
	while(dnode != NULL){			//loops through each node
		status_list->head = dnode->next;		//moves the next list to front of the list
		free(dnode);				//frees up space from deleted node
		dnode = status_list->head;		//stores next node to be deleted
	}
	free(status_list);		//after all nodes have been deleted, list memory is freed up

	return;
}	


// prints all the process entries that have a give niceness
void print_niceness(sortedLL* list, int key){		//linked list and a search key (niceness in this case) as parameters
	sortedLL* nice_list = list_init(NULL, 0);		//creates a new linked list for all elements of a give niceness

	node* inode = list->head;		//initializes an iterative node
	while(inode != NULL){			//loops through each node in the original list
		if(inode->process->niceness == key){			//if the node niceness matches the search key
			node* new_node = node_init(inode->process);		//creates a new node for the nice list
			add_node(nice_list, new_node);					//adds new node to nice list
		}
		inode = inode->next;		//iterates the iterative node
	}
	
	//prints all the entries that are on the nice list
	printf("All processes with the niceness %i:\n", key);
	print_all(nice_list);		
	

	node* dnode = nice_list->head;		//iterative node for deleting nodes from nice list
	while(dnode != NULL){		//loops through each node in nice list
		nice_list->head = dnode->next;		//moves front of list to the next node
		free(dnode);					//frees memory used for node
		dnode = nice_list->head;		//iterates node to the next front of the list
	}
	free(nice_list);	//after all nodes have been removed frees up the memory for the list

	return;
}


// pops node from list
node* pop_node(sortedLL* list){
	node* popped_node = list->head;	//gets node to pop off
	list->head = popped_node->next;	//moves list head to second node, removing it from the list
	list->size -= 1;
	popped_node->next = NULL;		//removes link to list from popped node
	
	return popped_node;	//returns popped node
}


// push node onto back of list
void push_node(sortedLL* list, node* pushed_node){
	int new_pid = 0;		//stores the pid for the next node
	node* inode = list->head;		//iterative node to loop through list
	while(inode != NULL){		//loops to end of list
		if(inode->next == NULL){		//if the node is the last in the list
			new_pid = inode->process->pid + 1;		//sets the new process id to be 1 larger than lastlargest
		}
		inode = inode->next;	//iterates the iterative node
	}

	pushed_node->process->pid = new_pid;	//updates pid of new node
	add_node(list, pushed_node);				//adds node to list

	return;
}

