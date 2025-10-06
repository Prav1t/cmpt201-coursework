//declaring al of the librarys I will be using
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
//defining th buffer size for printing

#define BUF_SIZE 1024 

struct header { //
    uint64_t size; //size of the memory block
    struct header* next; //size of the next block
};

void handle_error(const char* msg) { //a function to handle errors
    write(STDERR_FILENO, msg, strlen(msg)); //prints the error mesage
    _exit(1); //exits with error code 1
}

void print_out(char *format, void *data, size_t data_size) { //the custom print function
    char buf[BUF_SIZE];
    ssize_t len = snprintf(buf, BUF_SIZE, format,
                          data_size == sizeof(uint64_t) ? *(uint64_t *)data
                                                        : *(void **)data);
    if (len < 0) {
        handle_error("snprintf");
    }
    write(STDOUT_FILENO, buf, len);
}

int main() { //the main function
    void* heap_start = sbrk(256); //allocatinf 256 bytes of heap memory suing sbrk
    if (heap_start == (void*)-1) { //checks if sbrk might have failed
        handle_error("sbrk failed"); //handles the errro
    }
    
    struct header* first_block = (struct header*)heap_start; //creates the first block at the start of the memory
    struct header* second_block = (struct header*)((char*)heap_start + 128); //creates the second block 128 bytes afyer the first
  
    //initializing the firstand second block headers
  
    first_block->size = 128; 
    first_block->next = NULL;
    second_block->size = 128;
    second_block->next = first_block;
    
    size_t data_size = 128 - sizeof(struct header); //calculating the data area size

  //pointers the the first and second blocks data
  
    char* first_data = (char*)(first_block + 1);
    char* second_data = (char*)(second_block + 1);
    
    memset(first_data, 0, data_size); //initializing the first block to all 0s
    memset(second_data, 1, data_size); //initializing the second block to all 1s

  //printing now
    
    print_out("first block:       %p\n", &first_block, sizeof(first_block));
    print_out("second block:      %p\n", &second_block, sizeof(second_block));
    print_out("first block size:  %lu\n", &first_block->size, sizeof(first_block->size));
    print_out("first block next:  %p\n", &first_block->next, sizeof(first_block->next));
    print_out("second block size: %lu\n", &second_block->size, sizeof(second_block->size));
    print_out("second block next: %p\n", &second_block->next, sizeof(second_block->next));

  //looping through each byte in the data areas
  
    for (size_t i = 0; i < data_size; i++) {
        char buf1[16], buf2[16]; //creating buffers for a formatted output
      //formatting first and second block data bytes as strings
        snprintf(buf1, sizeof(buf1), "%d\n", first_data[i]);
        snprintf(buf2, sizeof(buf2), "%d\n", second_data[i]);
      //writing the first and second data block bytes to stdout
        write(STDOUT_FILENO, buf1, strlen(buf1));
        write(STDOUT_FILENO, buf2, strlen(buf2));
    }
    
    return 0; //exits the program
}
