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
#include"dStruct.h"


//Main function
int main(int argc, char* argv[]){

	sortedLL* list = list_init(NULL, 0);			//initializes a sorted linked list with no elements 

	processEntry* first_entry = entry_init(1302, 3, 2, 34.5f, 65.6f);			//creates the first process entry [pid, status, niceness, cputime, proctime]
	node* first = node_init(first_entry);										//creates the first node for the linked list
	add_node(list, first);														//adds node to list

 	node* second = node_init(entry_init(12, 2, 4, 0.03f, 12.2f));				//node creation
	add_node(list, second);														//adds node to linked list

	node* third = node_init(entry_init(50, 2, 4, 0.03f, 12.2f));				//node creation
	add_node(list, third);														//adds node to linked list

	node* fourth = node_init(entry_init(3899, 1, 2, 3.00f, 4.00f));				//node creation
	add_node(list, fourth);														//adds node to linked list

	set_cputime(list, 3899, 69.0f);				//changes cputime of pid 3899 to 69.0
	set_niceness(list, 3899, 45);				//changes niceness of pid 3899 to 45
	set_status(list, 3899, 97);					//changes status of pid 3899 to 97

	print_all(list);		//prints entire linked list

	int key_pid = 55;		//search key
	printf("The search for %i:\n", key_pid);	
	processEntry* found = process_search(list, key_pid);		//searches for processEntry with given search key
	if(found == NULL){				// should return NULL if processEntry does not exist
		printf("Not found\n");
	}
	else{
		print_entry(found);			//if process entry does exist, prints the individual process entry
	}

	printf("nice search for 4 returns: %i\n", nice_search(list, 4));			//demonstrates the search for processes with a niceness of 4
	printf("status search for 3 returns: %i\n", status_search(list, 3));		//demonstrates the search for processes with a status of 3
	printf("cpu time for 55: %.2f\n", get_cputime(list, 55));					//gets the cpu time for a process with the pid 55
	printf("niceness for 3899: %i\n", get_niceness(list, 3899));				//gets the niceness for a process with the pid 3899
	printf("status for 50: %i\n", get_status(list, 50));						//gets the status for a process with a pid of 50

	print_status(list, 2);					//prints all entries with a status of 2
	print_niceness(list, 4);				//prints all entries with a niceness of 4

	node* popped_node = pop_node(list);			//deletes a node with pid 50
	printf("after pop:\n");
	print_all(list);

	push_node(list, popped_node);
	printf("\n\nafter node pushed back on:\n");		
	print_all(list);			//prints all the entries again to double check changes and deletions

	printf("Success\n");
	
}//end


