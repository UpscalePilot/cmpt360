#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cairo.h>

#define MAX_LINE_LENGTH 100

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <log_file>\n", argv[0]);
        return 1;
    }

    FILE *file;
    char line[MAX_LINE_LENGTH];
    int num_pids = 0;
    float *pid_cputimes = NULL;

    // Construct the file path
    char file_path[100];
    strcpy(file_path, "../log/");
    strcat(file_path, argv[1]);

    // Open the log file
    file = fopen(file_path, "r");

    // Check for error opening file
    if (file == NULL) {
        printf("Failed to open the log file.\n");
        return 1;
    }

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {

        // Check if the line contains information about the running list
        if (strstr(line, "Running List")) {

            // Extract the PID and CPU time from the line
            int pid;
            float cputime;
            sscanf(line, "%*d %d %*d %*d %f", &pid, &cputime);

            // Update the corresponding cputime variable based on the PID
            if (pid > num_pids) {
                // Resize the array to accommodate the new PID
                pid_cputimes = realloc(pid_cputimes, pid * sizeof(float));
                num_pids = pid;
            }
            // Save pid's CPU time
            pid_cputimes[pid - 1] = cputime;
        }
    }


    // Close the file
    fclose(file);

    // Calculate the total width required
    float total_width = 50; // Initial padding
    for (int i = 0; i < num_pids; i++) {
        total_width += pid_cputimes[i] * 50;
    }

    // Adjust the surface width based on the total width required
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, total_width+75, 160);
    cairo_t *cr = cairo_create(surface);

    // Set the background color
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    // Draw the boxes with black borders
    for (int i = 0; i < num_pids; i++) {
        float pid_cputime = pid_cputimes[i];

        // Define an array of vibrant colors
    const float vibrant_colors[][3] = {
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

    // Set the color based on the PID using vibrant colors
    float r = vibrant_colors[i % 10][0];
    float g = vibrant_colors[i % 10][1];
    float b = vibrant_colors[i % 10][2];
    cairo_set_source_rgb(cr, r, g, b);

        // Draw the box
	float x = 50;
        for (int j = 0; j < i; j++) {
            x += pid_cputimes[j] * 50;
        }
        cairo_rectangle(cr, x, 50, pid_cputime * 50, 50);
        cairo_fill(cr);

        // Draw the border
        cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);
        cairo_rectangle(cr, x, 50, pid_cputime * 50, 50);
        cairo_stroke(cr);

        // Draw the number in the middle of the box
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_select_font_face(cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 20);
        char pid_str[3];
        sprintf(pid_str, "%d", i + 1);
        cairo_text_extents_t extents;
        cairo_text_extents(cr, pid_str, &extents);
        double text_x = x + pid_cputime * 50 / 2 - extents.width / 2;
        double text_y = 50 + 50 / 2 + extents.height / 2;
        cairo_move_to(cr, text_x, text_y);
        cairo_show_text(cr, pid_str);
    }

    // Draw the arrow
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    float arrow_x = total_width +25;
    cairo_move_to(cr, 50, 100);
    cairo_line_to(cr, arrow_x, 100);
    cairo_line_to(cr, arrow_x - 10, 97);
    cairo_move_to(cr, arrow_x, 100);
    cairo_line_to(cr, arrow_x - 10, 103);
    cairo_stroke(cr);

    // Add "t" under the arrow
    cairo_move_to(cr, arrow_x - 15, 125);
    cairo_show_text(cr, "t");

    // Get the timestamp from the command line argument
    char *timestamp = argv[1];

    // Extract the date from the timestamp
    char date[11];
    strncpy(date, timestamp + 4, 10);
    date[10] = '\0';

    // Construct the new file name
    char new_file_name[100];
    sprintf(new_file_name, "../log/img-%s.png", date);

    // Save the plot to the new file name
    cairo_surface_write_to_png(surface, new_file_name);


    // Clean up
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    free(pid_cputimes);

    return 0;
}
