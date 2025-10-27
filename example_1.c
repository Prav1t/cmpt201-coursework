#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(expr)                                                           
  {                                                                            
    if (!(expr)) {                                                             
      fprintf(stderr, "Assertion failed: %s\n", #expr);                        
      exit(1);                                                                 
    }                                                                          
  }

#define TEST(expr)                                                             
  {                                                                            
    if (!(expr)) {                                                             
      fprintf(stderr, "Test failed: %s\n", #expr);                             
      exit(1);                                                                 
    } else {                                                                   
      printf("Test passed: %s\n", #expr);                                      
    }                                                                          
  }

typedef struct node {
  uint64_t data;
  struct node *next;
} node_t;

node_t *head = NULL;

void insert_sorted(uint64_t data) {
  node_t *new_node = malloc(sizeof(node_t));
  new_node->data = data;
  new_node->next = NULL;

  if (head == NULL) {
    head = new_node;
  } else if (data < head->data) {
    // Insert at head
    new_node->next = head;
    head = new_node;
  } else {
    node_t *curr = head;
    node_t *prev = NULL;

    bool inserted = false;
    while (curr != NULL && !inserted) {
      if (data < curr->data) {
        prev->next = new_node;
        new_node->next = curr;
        inserted = true;
      }
      prev = curr;
      curr = curr->next;
    }
    
    // If we reached the end without inserting, adds at the tail of it
    if (!inserted) {
      prev->next = new_node;
    }
  }
}

int index_of(uint64_t data) {
  node_t *curr = head;
  int index = 0;

  while (curr != NULL) {
    if (curr->data == data) {
      return index;
    }

    curr = curr->next;
    index++;
  }

  return -1;
}

// Helper function to print the list for debugging
void print_list() {
  node_t *curr = head;
  printf("List: ");
  while (curr != NULL) {
    printf("%lu ", curr->data);
    curr = curr->next;
  }
  printf("\n");
}

int main() {
  insert_sorted(1);
  insert_sorted(2);
  insert_sorted(5);
  insert_sorted(3);

  print_list(); 
  TEST(index_of(3) == 2);

  insert_sorted(0);
  insert_sorted(4);

  print_list(); 
  TEST(index_of(4) == 4);

  printf("All tests passed!\n");
  return 0;
}
