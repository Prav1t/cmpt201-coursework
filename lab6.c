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
    
    // If we reached the end without inserting, add at the tail
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

// Helper function to print list for debugging
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

  print_list(); // Debug: see the list state
  TEST(index_of(3) == 2);

  insert_sorted(0);
  insert_sorted(4);

  print_list(); // Debug: see the list state
  TEST(index_of(4) == 4);

  printf("All tests passed!\n");
  return 0;
}



#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(expr)                                                           
  {                                                                            
    if (!(expr)) {                                                             
      fprintf(stderr, "Assertion failed at %s:%d: %s\n", __FILE__, __LINE__, #expr); 
      exit(1);                                                                 
    }                                                                          
  }

#define TEST(expr)                                                             
  {                                                                            
    if (!(expr)) {                                                             
      fprintf(stderr, "Test failed: %s\n", #expr);                             
      exit(1);                                                                 
    }                                                                         
  }

typedef struct node {
  uint64_t data;
  struct node *next;
} node_t;

typedef struct info {
  uint64_t sum;
} info_t;

node_t *head = NULL;
info_t info = {0};

// Function to calculate sum by iterating through the list
uint64_t calculate_sum() {
  uint64_t sum = 0;
  node_t *curr = head;
  while (curr != NULL) {
    sum += curr->data;
    curr = curr->next;
  }
  return sum;
}

void insert_sorted(uint64_t data) {
  node_t *new_node = malloc(sizeof(node_t));
  new_node->data = data;
  new_node->next = NULL;

  if (head == NULL) {
    head = new_node;
  } else if (data < head->data) {
    new_node->next = head;
    head = new_node;
  } else {
    node_t *curr = head;
    node_t *prev = NULL;

    while (curr != NULL) {
      if (data < curr->data) {
        break;
      }

      prev = curr;
      curr = curr->next;
    }

    prev->next = new_node;
    new_node->next = curr;  // Fixed: was curr->next
  }

  info.sum += data;
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
  insert_sorted(3);
  insert_sorted(5);
  insert_sorted(2);

  // Debug: print the list to see what we have
  print_list();
  printf("Info sum: %lu\n", info.sum);
  printf("Calculated sum: %lu\n", calculate_sum());

  // Assertions to verify correctness
  ASSERT(info.sum == calculate_sum());
  ASSERT(index_of(2) == 1);
  
  TEST(info.sum == 1 + 3 + 5 + 2);
  TEST(index_of(2) == 1);

  printf("All tests passed!\n");
  return 0;
}
