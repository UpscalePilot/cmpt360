/*
#####################################################################
#           CMPT 360    - Lab 1                      		        #
#           Ben Morley  - ID 3142870								#
#			Nabil Gelle - ID 3104375                                #
#                                                                   #
#####################################################################
*/

//STRUCTURES

//Stores an individual process entry
typedef struct processEntry{
	int pid;
	int status;
	int niceness;
	float cputime;
	float proctime;

}processEntry;

//stores a node in a sorted linked list
typedef struct node{
	processEntry* process;
	struct node* next;
}node;

//stores the front and size of a sorted linked list
typedef struct sortedLL{
	node* head;	
	int size;

}sortedLL;


//PROTOTYPES
processEntry* entry_init(int, int, int, float, float);

void print_entry(processEntry*);

node* node_init(processEntry*);

sortedLL* list_init(node*, int);

void print_all(sortedLL*);

void add_node(sortedLL*, node*);

int delete_node(sortedLL*, int);

node* node_search(sortedLL*, int);

int nice_search(sortedLL*, int);

int status_search(sortedLL*, int);

processEntry* process_search(sortedLL*, int);

float get_cputime(sortedLL*, int);

void set_cputime(sortedLL*, int, float);

int get_niceness(sortedLL*, int);

void set_niceness(sortedLL*, int, int);

int get_status(sortedLL*, int);

void set_status(sortedLL*, int, int);

void destroy_list(sortedLL*);

void print_status(sortedLL*, int);

void print_niceness(sortedLL*, int);

