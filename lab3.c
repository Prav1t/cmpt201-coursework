//Lab3
//By Pravit Hundal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 5 //the max no of lines to store

int main() {
    char *history[MAX_HISTORY] = {0}; //array of pointers to stor 5 strings
    int index = 0; //to track where to store the next input
    char *line = NULL; //pointer that getline uses to allocate memory for input
    size_t len = 0; //variable getline will use to track the buffer size

    while(1) {
        printf("Please enter input: ");
        if (getline(&line, &len, stdin) == -1) {    
            break;
        }
        line[strcspn(line, "\n")] = '\0';
        if (history[index] != NULL) { //if the position already has a string
            free(history[index]); //frees the allocated memory at the position
        }
        history[index] = strdup(line); //duplicates the input string using strdup    
        index = (index + 1) % MAX_HISTORY; //moves to the next position in a circular buffer

        if (strcmp(line, "print") == 0) { //checks if the user entered the print command
            for (int i = 0; i < MAX_HISTORY; i++) { //loops through all history positions
                if (history[i] != NULL) { //only prints if the position has data stored
                    printf("%s\n", history[i]); //prints the input lined that was stored
                }
            }
        }
    }

    free(line); //frees the main input buffer that getline allocated

    for (int i = 0; i < MAX_HISTORY; i++) { //
        if (history[i] != NULL) {
            free(history[i]); //frees memory allocated by strdup
        }
    }

    return 0;
}
