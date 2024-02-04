// Ben Morley - Lab 2

//headers
#include <cairo/cairo.h>
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include"dStruct.h"

//preprocessor definitions
#define WIDTH 700
#define HEIGHT 250
#define MAX_NAME_LEN 50

//prototypes
void generate_image(char*);
void read_data(sortedLL*, char*);
void draw_image(sortedLL*, char*);
float get_timescale(sortedLL*);
void fix_file_ext(char*, int);


//Main function, looks for all log files in log directory
int main() {

	DIR *directory; 				//directory pointer
	struct dirent* procDir; 
	char prefix[] = "log/"; 		//prefix for files in the log folder
	char file_name[MAX_NAME_LEN] = "log/"; 		//initializes a variable for the file name, starts with prefix

	directory = opendir("log"); 		//opens directory
	if(directory){ 						//if the directory open was successful
		while((procDir = readdir(directory)) != NULL){ 			//loops through each file in the dir
			if(procDir->d_type == 8 && (strcmp(".txt", strrchr(procDir->d_name, '.')) )== 0){ 		// checks if each file is a regular file and is a .txt file
				printf("Generating image for: %s\n", procDir->d_name); 		//tells user image is being generated
				strcat(file_name, procDir->d_name); 		//adds file name to file name string
				generate_image(file_name); 			//generates image for log file
			}
			strcpy(file_name, prefix); 		//copies prefix back to filename string for the next file
		}
		closedir(directory); 		//closes directory upon completion
	}

}


//This function contains the steps for generating an image
void generate_image(char* filename){
	sortedLL* proclist = list_init(NULL, 0); 		//initializes a list for all the processes in the log file

	read_data(proclist, filename); 				// reads data from log file

	draw_image(proclist, filename);	 			// draws image based on data read from log file

}



//reads data from log file and stores it in a linked list
void read_data(sortedLL* list, char* filename){
	FILE* fread = fopen(filename, "r"); 		//initializes file pointer
	if(fread == NULL){  		//if file pointer fails 
		exit(EXIT_FAILURE);
	}

	//these variables are for storing the time and date info at the start of each line
	char wday[4]; 			
	char month[4];
	int day;
	char time[9];
	int year;
	int stime;

	//these variables store the info specific to the processes
	int pid;
	int status;
	int niceness;
	float proctime;
	float cputime;


	//loops through each line of the log file till the end of the file
	while( fscanf(fread, "%s %s %d %s %d Time: %d, PID: %d, Status: %d, Niceness: %d, CPUTime: %f, ProcTime: %f",
		wday,
		month,
		&day,
		time,
		&year,
		&stime,
		&pid,
		&status,
		&niceness,
		&cputime,
		&proctime) != EOF){

			//if the status is 3, the process is complete, so then it is added to the list
			if(status == 3){ 
				processEntry* new_proc = entry_init( pid, status, niceness, cputime, proctime);
				node* new_node = node_init(new_proc);
				push_node(list, new_node);
			}
	}

}


// gets the scale for the graph, returns the number of pixels per second of process time
float get_timescale(sortedLL* list){

	//initializes a summation variable
	float sumTime = 0.0f;

	node* inode = list->head; 		//loops through each node in the list
	while(inode != NULL){
		sumTime += inode->process->cputime; 		// adds the cpu time to a running sum
		inode = inode->next;
	}

	return(600/sumTime); 		//returns the number of pixels per second of process time
}


// draws the image and saves it as a png
void draw_image(sortedLL* list, char* filename){

  	// Initialize cairo.
  	cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,
				WIDTH, HEIGHT);

  	cairo_t *cr = cairo_create (surface);

  	// White Background
  	cairo_set_source_rgb(cr, 1, 1, 1);
  	cairo_rectangle(cr, 0, 0, WIDTH, HEIGHT);
  	cairo_fill(cr);

	//draw axis
	cairo_set_line_width (cr, 10.0);
  	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
	cairo_move_to(cr, 50.0, 200.0); 
	cairo_line_to(cr, 650.0, 200.0);
	cairo_stroke(cr);


	float timesc = get_timescale(list); 		// gets the pixels/second of processing time
	int x = 50; 		// stores the x location we traverse the graph
	int box_width; 		// stores the width of each box on the graph

	//loops through each node in the list
	node* inode = list->head; 	
	while(inode != NULL){
		box_width = inode->process->cputime * timesc; 		// calculates the width for each box
		cairo_rectangle(cr, x, 50, box_width, 145); 		// creates rectangle for box

		//This section of code alternates the color of the boxes between red, blue and green
		int color = inode->process->pid % 3; 		
		if(color == 0){
			cairo_set_source_rgb(cr, 1, 0, 0);
		}
		else if(color == 1){
			cairo_set_source_rgb(cr, 0, 1, 0);
		}
		else{
			cairo_set_source_rgb(cr, 0, 0, 1);
		}

		cairo_fill(cr); 		// draws the box

  		cairo_set_font_size(cr, 24); 		//sets font size
  		cairo_move_to(cr, (x + (box_width / 2 )-12), 125); 		//sets font location
  		cairo_set_source_rgba(cr, 0, 0, 0, 1); 					//set font color
		char* pid_text = malloc(sizeof(char)*5);  		//allocates space for a string to store the pid
		snprintf(pid_text, 5, "%d", inode->process->pid); 		//creates a string to display the pid
  		cairo_show_text(cr, pid_text); 		//puts text on image


		cairo_fill(cr); 		//draws image

		inode = inode->next; 	//moves to next node in the list
		x += box_width; 		//increments the x position along the graph

	}

  
	int len = strlen(filename); 		// gets length of file name
	fix_file_ext(filename, len); 		// switches file extension from .txt to .png so file can be printed correctly
	printf("saving %s\n", filename); 	// tells user image is being saved	

  	// save the image  
  	cairo_surface_write_to_png(surface, filename);


 }


// changes the file extension to .png
 void fix_file_ext(char* filename, int len){

	char new_str[len]; 		// creates a new string
	memset(new_str, '\0', sizeof(new_str)); 		// allocates memory by setting last spot in string to the null char
	strncpy(new_str, filename, len-4); 		// copies all but last four characters to new string, leaving out old file ext
	
	strcpy(filename, new_str); 		//copies file name back to original string location

	strcat(filename, ".png"); 		//adds the .png extension on to the end of the file name
 }
