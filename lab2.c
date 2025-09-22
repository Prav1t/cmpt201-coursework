#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

  char command[100]; // array to store the user input

  while (1) { // infinate loop
    printf(
        "Please enter programs to run: \n"); // asks the user for program to run
    scanf("%s", command);                    // reads user input
    pid_t pid = fork();                      // creates a child process

    if (pid == 0) {                  // if we are in the child process
      execl(command, command, NULL); // replace child with the requested program

      printf("Exec failed\n"); // if exec fails
      return 1;                // exists the child process
    } else {                   // if we are in the parent process
      waitpid(pid, NULL, 0);   // waits for the child process to finish
    }
  }
  return 0; // ends the program
}
