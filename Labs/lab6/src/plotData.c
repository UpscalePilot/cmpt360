#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <time.h>
#include <cairo/cairo.h>

struct data{
    int filePID;
    int entryPIDTime;
    int exitPIDTime;
    float startCPUTime;
    struct data* next;
};

void push(struct data** head_ref, int filePID, int entryPIDTime, int exitPIDTime, float startCPUTime){
    struct data* ptr1 = (struct data*)malloc(sizeof(struct data));
    ptr1 -> filePID = filePID;
    ptr1 -> entryPIDTime = entryPIDTime;
    ptr1 -> exitPIDTime = exitPIDTime;
    ptr1 -> startCPUTime = startCPUTime;
    ptr1 -> next = NULL;

	//if linked list is not null, set next node
	if (*head_ref == NULL){
        *head_ref = ptr1;
    }
    else{
		struct data* temp = *head_ref;
		while (temp->next != NULL){
			temp = temp->next;
        }
		temp->next = ptr1;
	}
    return;
}

int searchAllInfo(struct data* head, int PID,int searchEntry, int searchExit, float searchStartCPU){
    struct data* temp = head;
    while(temp != NULL){
        if ((temp) -> filePID == PID){
            // compare what we are searching for
            if ((temp) -> entryPIDTime == searchEntry){
                return 1;
            }
            else if ((temp) -> exitPIDTime == searchExit){
                return 1;
            }
            else if ((temp) -> startCPUTime == searchStartCPU){
                return 1;
            }
        }
        temp = temp->next;
    }
    return 0;
}

int searchPID(struct data* head, int PID){
    struct data* temp = head;
    while(temp != NULL){
        //see if PID exists in our linked list
        if ((temp) -> filePID == PID){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void setCPU(struct data* head, int findPID, float setStartCPU){
    struct data* temp = head;
    while(temp != NULL){
        // PID is found, check if the info is un-initialized
        if ((temp) -> filePID == findPID){
            if ((temp)->startCPUTime == -1.00){
                temp -> startCPUTime = setStartCPU;
            }
            break;
        }
        temp = temp->next;
    }
    return;
}

void setExit(struct data* head, int findPID, float setExit){
    struct data* temp = head;
    while(temp != NULL){
        // PID is found, check if the info is un-initialized
        if ((temp) -> filePID == findPID){
            if ((temp)->exitPIDTime == -1){
                temp -> exitPIDTime = setExit;
            }
            break;
        }
        temp = temp->next;
    }
    return;
}

void printInfo(struct data* head){
	struct data* temp = head;
    while(temp != NULL){
        //print out all the data on this pid node
        printf("File PID:    %d \n", temp -> filePID);
        printf("Entry PID:   %d \n", temp -> entryPIDTime);
        printf("Exit PID:    %d \n", temp -> exitPIDTime);
        printf("Start CPU:   %.2f \n\n", temp -> startCPUTime);
        temp = temp->next;
    }
    return;
}
void freeList(struct data** head_ref) {
    struct data* current = *head_ref;
    struct data* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    // Reset head to NULL after freeing all nodes
    *head_ref = NULL;

    return;
}

struct drawProcess{
    int drawPID;
    struct drawProcess* next;
};

void drawPush(struct drawProcess** head_ref, int drawPID){
    struct drawProcess* ptr1 = (struct drawProcess*)malloc(sizeof(struct drawProcess));
    ptr1 -> drawPID = drawPID;
    ptr1 -> next = NULL;

	//if linked list is not null, set next node
	if (*head_ref == NULL){
        *head_ref = ptr1;
    }
    else{
		struct drawProcess* temp = *head_ref;
		while (temp->next != NULL){
			temp = temp->next;
        }
		temp->next = ptr1;
	}
    return;
}

void drawRectangles(struct drawProcess* head_ref, cairo_t* cr){

    // Set the background color
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    // create the texts for the graph
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    
    //time
    cairo_set_font_size(cr, 50);
    cairo_move_to(cr, 600, 650);
    cairo_show_text(cr, "Time");

    // Define an array of vibrant colors, color for setting rectangle
    const float vibrant_colors[][3] = 
    {
        {0.0, 1.0, 0.0},   // Green
        {1.0, 1.0, 0.0},   // Yellow
        {0.0, 1.0, 1.0},   // Cyan
        {1.0, 0.0, 0.0},   // Red
        {0.75, 0.75, 0.75},// Silver
        {1.0, 0.5, 0.0},   // Orange
        {0.0, 0.0, 1.0},   // Blue
        {1.0, 0.0, 1.0},    // Pink
        {1.0, 0.84, 0.0},   // Gold
        {0.5, 0.0, 0.5},   // Purple
    };

    // Draw the rectangles with black borders
    int drawWidth=30, xCoord=140;
    struct drawProcess* current = head_ref;
    struct drawProcess* next;
    while(current != NULL){
        next = current->next;

        // we cant use next->drawpid for the last linked list, it'll crash
        if (next == NULL)
        {
            // Set the color based on the PID using vibrant colors
            float r = vibrant_colors[current->drawPID][0];
            float g = vibrant_colors[current->drawPID][1];
            float b = vibrant_colors[current->drawPID][2];

            // draw rectangle for PID then the border
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, xCoord, 350, drawWidth, 200);
            cairo_fill(cr);
            cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);
            cairo_rectangle(cr, xCoord, 350, drawWidth, 200);
            cairo_stroke(cr);

            // Draw the number in the middle of the box
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(cr, 20);
            char pid_str[5];
            sprintf(pid_str, "%d", current->drawPID);
            cairo_text_extents_t extents;
            cairo_text_extents(cr, pid_str, &extents);
            double text_x = xCoord-5 + (drawWidth / 2);
            cairo_move_to(cr, text_x, 450);
            cairo_show_text(cr, pid_str);

            break;
        }
        // if currentPID & next PID == same, add to width to draw later

        if (current->drawPID == next->drawPID){
            drawWidth = drawWidth + 30;
        }
        // time to draw, (roundabout way is necesary for RR)
        else{
            // Draw PID now, set the color based on the PID using vibrant colors
            float r = vibrant_colors[current->drawPID][0];
            float g = vibrant_colors[current->drawPID][1];
            float b = vibrant_colors[current->drawPID][2];
            // draw rectangle for PID then the border
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, xCoord, 350, drawWidth, 200);
            cairo_fill(cr);
            cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);
            cairo_rectangle(cr, xCoord, 350, drawWidth, 200);
            cairo_stroke(cr);

            // Draw the number in the middle of the box
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(cr, 20);
            char pid_str[5];
            sprintf(pid_str, "%d", current->drawPID);
            cairo_text_extents_t extents;
            cairo_text_extents(cr, pid_str, &extents);
            double text_x = xCoord-5 + (drawWidth / 2);
            cairo_move_to(cr, text_x, 450);
            cairo_show_text(cr, pid_str);

            // set coordinates for next PID
            xCoord = xCoord + drawWidth;
            drawWidth=30;  
        }
        current = next;
    }

    // Draw the arrow
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    float arrow_x = 1200;
    cairo_move_to(cr, 140, 600);
    cairo_line_to(cr, arrow_x, 600);
    cairo_line_to(cr, arrow_x - 10, 590);
    cairo_move_to(cr, arrow_x, 600);
    cairo_line_to(cr, arrow_x - 10, 610);
    cairo_stroke(cr);

    return;
}

void printDrawList(struct drawProcess* head){
    struct drawProcess* temp = head;
    while(temp != NULL){
        //print out the data on this pid node
        printf("drawPID:        %d \n", temp -> drawPID);
        temp = temp->next;
    }
    return;
}

void drawFreeList(struct drawProcess** head_ref) {
    struct drawProcess* current = *head_ref;
    struct drawProcess* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    // Reset head to NULL after freeing all nodes
    *head_ref = NULL;

    return;
}

int main(int argc, char **argv)
{
	// used to iterate/open files on directory
	char line[100], filePath[100];
	int pid, status, numOfProcess=0;
    // int rr;
    // int sjf=1;
    int cycleT;
    float cpuTime, procTime, lastProcTime=-1.00, firstProcTime=-1.00, totalResponseTime=0, totalTurnTime=0;
    struct data *PIDInfo = NULL;
    struct drawProcess *drawThis = NULL;

    // change directory to log, open file
    chdir("..");
    chdir("log");
    getcwd(filePath, 100);
    strcat(filePath, "/");
    strcat(filePath, argv[1]);
    FILE *in_file;
    if ((in_file = fopen(filePath, "r")) == NULL){
        printf("Error: Failed to open input file\n");
        return 0;
    }

    // skip first line
    fgets(line, sizeof(line), in_file);
    char firstline[8][10];
    char algor[5];
    sscanf(line, "%s %s %s %s %s %s %s %s\t\tAlgorithm=%s", 
        firstline[0],
        firstline[1],
        firstline[2],
        firstline[3],
        firstline[4],
        firstline[5],
        firstline[6],
        firstline[7],
         algor);

    // printf("%s\n", algor);
    // for(int i =0;i<8;i++){
    //     printf("%s\n", firstline[i]);
    // }
    

    // Read the file line by line
    while (fgets(line, sizeof(line), in_file)){
        sscanf(line, "%d %d %d %*d %f %f", &cycleT, &pid, &status, &cpuTime, &procTime);

        // //if we have a process that is in readylist but has been processed before, its rr
        // if (status == 1 && cpuTime != 0.00){
        //     rr = 1;
        // }

        // check for sjf
        // set proctime for first PID on readylist, this is only used for the first cycle
        if (lastProcTime == -1){
            firstProcTime = procTime;
            lastProcTime = procTime;
        }
        //check first cycle, sjf is true until proven false
        if (cycleT == 0 ){          //&& sjf == 1
            if (status == 1 && lastProcTime <= procTime){
                lastProcTime = procTime;
            }
            else if (status == 1 && lastProcTime > procTime){
                //sjf = 0;
            }
            // status is 2 or 3, we have runninglist at the bottom of our cycle
            else if (status != 1 && procTime < firstProcTime){
                //sjf = 0;
            }
        }

        // if PID does not exist in the linked list, push this to linked list
        if (searchPID(PIDInfo, pid) == 0){
            push(&PIDInfo, pid, cycleT, -1, -1.00);
            numOfProcess++;
        }

        // status=2 is Runninglist, update CPUTime if it isn't initialized
        // -1=unitialized, -2=don't search, returns 1 if it isn't initialized
        if (status == 2){
            if ((searchAllInfo(PIDInfo, pid, -2, -2, -1.00)) == 1){
                setCPU(PIDInfo, pid, cycleT);
                totalResponseTime = totalResponseTime + cycleT;
            }
        }
        // line is Runninglist and process is completed, set exit time for PID
        else if (status == 3){
            setExit(PIDInfo, pid, cycleT);
            totalTurnTime = totalTurnTime + cycleT;
        }

        // if status is not ready, (running=2 or completed=3)
        if (status != 1){
            drawPush(&drawThis, pid);
        }
    }

    // Close the file
    fclose(in_file);

    // Adjust the surface width based on the total width required
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 8000, 700);
    cairo_t *cr = cairo_create(surface);

    // Draw the rectangle boxes and PID inside
    drawRectangles(drawThis, cr);

    //get average response time and draw it
    char averageResponseTime[40] = "Average Response Time: ", responseValue[10];
    sprintf(responseValue, "%.2f", (totalResponseTime/numOfProcess));
    strcat(averageResponseTime, responseValue);
    cairo_set_font_size(cr, 25);
    cairo_move_to(cr, 250, 250);
    cairo_show_text(cr, averageResponseTime);

    // get average turnaround time and draw it
    char averageTurnTime[40] = "Average Turnaround Time: ", turnAroundValue[10];
    sprintf(turnAroundValue, "%.2f", (totalTurnTime/numOfProcess));
    strcat(averageTurnTime, turnAroundValue);
    cairo_set_font_size(cr, 25);
    cairo_move_to(cr, 725, 250);
    cairo_show_text(cr, averageTurnTime);

    // Get the timestamp from the command line argument & extract timestamp
    char *timestamp = argv[1];
    char date[11];
    strncpy(date, timestamp + 4, 10);
    date[10] = '\0';

    char algorithm[25] = "Algorithm: ", new_file_name[100];
    //title and save file name based on the algorithm
    // Note: technically if FIFO files are organized as SJF by chance or even RR
    // This program will return SJF as true and print that out. False positive
    // if (rr == 1){
    //     strcat(algorithm, "RR");
    //     sprintf(new_file_name, "../log/img-RR-%s.png", date);
    // }
    // else if (sjf == 1){
    //     strcat(algorithm, "SJF");
    //     sprintf(new_file_name, "../log/img-SJF-%s.png", date);
    // }
    // else{
    //     strcat(algorithm, "FIFO");
    //     sprintf(new_file_name, "../log/img-FIFO-%s.png", date);
    // }

    strcat(algorithm, algor);
    sprintf(new_file_name, "../log/img-%s-%s.png", algor, date);

    cairo_set_font_size(cr, 50);
    cairo_move_to(cr, 510, 100);
    cairo_show_text(cr, algorithm);

    // Save the plot to the new file name
    cairo_surface_write_to_png(surface, new_file_name);
    printf("Image created and saved to %s\n", new_file_name);

    cairo_fill (cr);

    // Clean up
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    drawFreeList(&drawThis);
    freeList(&PIDInfo);

    return 0;
}