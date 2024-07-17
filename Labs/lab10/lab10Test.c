#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

int main() {
    int error_count = 0;

    // Get the current user's username from the USER environment variable
    const char *user = getenv("USER");
    if (user == NULL) {
        printf("Failed to get the current user's username.\n");
        return 1;
    }


    // Get the current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("Failed to get the current working directory.\n");
        return 1;
    }
    
    // Define the test commands with placeholder for the username and current working directory
    const char *commands[] = {
        // "mkdir test",
        // "mkdir test/test2",
        // "mkdir test/test2/test3",
        // "touch test/notsingle.txt",
        // "touch test/single.txt",
        // "touch test/qmark.txt",
        // "touch test/trap.c",
        // "touch test/test2/test3/glab.txt",
        // "touch test/test2/test3/glib.txt",
        // "touch test/test2/test3/glob.txt",
        // "touch test/test2/test3/not_this.txt",
        "./findme %s -type f -name \"gl*b.txt\" -user %s -maxdepth 5",
        "./findme %s -type f -name \"?mark.txt\" -user %s -maxdepth 5",
        "./findme %s -type f -name \"[s]ingle.txt\" -user %s -maxdepth 5",
        "./findme %s -type f -name \"[^p]ingle.txt\" -user %s -maxdepth 5",
        "./findme %s -type f -name \"[^f]*.c\" -user %s -maxdepth 5"
    };
    
    // Execute each command after replacing the placeholder with the username
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        // Allocate memory for the command with enough space to include the username
        char *command = (char *)malloc(strlen(commands[i]) + strlen(user) + strlen(cwd) + 10);
        if (command == NULL) {
            printf("Memory allocation failed.\n");
            return 1;
        }
        
        // Replace the placeholders with the actual username and current directory
        sprintf(command, commands[i], cwd, user);
        
        // Execute the command
        printf("Executing command: %s\n", command);
        int result = system(command);
        if (result == 0) {
            printf("Command successful.\n");
        } else {
            printf("Command failed with error code %d.\n", result);
            error_count++;
        }
        
        // Free the allocated memory
        free(command);
    }
    
    printf("Total commands that did not work: %d\n", error_count);
    return 0;
}
