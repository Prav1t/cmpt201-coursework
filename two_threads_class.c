#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void* my_thread(void *arg) {
  char* message = (char*) arg;
  printf("Inside a thread!\n");
  printf("message is: %s\n", message);

  return (void *)strlen(message);
  
}
int main(void) {
  
  printf("Hello world\n");

  pthread_t thread_id;

  pthread_create(&thread_id, NULL, my_thread, "Hello threads!");

  void* ans;
  //sleep(5);
  pthread_join(thread_id, &ans);
  long long len = (long long) ans;
  printf("Done: answer is %lld.\n", len);

}
