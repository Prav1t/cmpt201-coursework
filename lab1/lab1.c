#define _POSIX_C_SOURCE 200809L // to enable features like getline

// Standard libraries
#include <stdio.h>  //for input/output
#include <stdlib.h> //memory allocation functions
#include <string.h> //for string manipulation

int main() {

  char *buffer = NULL; // pointer to store the input buffer initialized to null
                       // so getline can allocate memory
  size_t bufsize = 0; // to store the size of the allocated buffer
  ssize_t chars_read; // to store the no of char read by the getline

  printf("Enter text: "); // asks for text

  chars_read = getline(&buffer, &bufsize,
                       stdin); // reads the input line from standard input

  if (chars_read == -1) { // if getline fails

    perror("getline has failed"); // error message

    free(buffer); // free any allocated memory

    return 1; // exits
  }

  if (chars_read > 0 &&
      buffer[chars_read - 1] == '\n') { // checks if at least 1 char is read and
                                        // if last char is newline

    buffer[chars_read - 1] =
        '\0'; // replaces the newline with null terminator to end the string
  }

  printf("Tokens:\n"); // prints the headinf for the tokens output

  char *saveptr =
      NULL; // the pointer for strtok_r to initalized to null for the first call

  char *token = strtok_r(buffer, " ", &saveptr); // gets the first token

  while (token != NULL) { // lops through all tokens until no more are left

    printf("  %s\n", token); // gets the next token from the same string
  }

  free(buffer); // frees the memory allocated by getline to prevent memory leaks

  return 0;
}
