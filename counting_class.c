#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int cnt = 0;

void *counter_thread(void *arg) {
  for (int i = 0; i < MAX; i++) {
    cnt+= 1;

  return NULL;
  
}
int main(void) {
  


  pthread_t thread_id;

  pthread_create(&thread_id, NULL, counter_thread, NULL);

  
  pthread_join(thread_id, NULL);

  pthread_t thread_id2;

  pthread_create(&thread_id2, NULL, counter_thread, NULL);

  
  pthread_join(thread_id2, NULL);
  printf("the answer is...\n");
  sleep (1);
  printf(" = d.\n", cnt);

}
