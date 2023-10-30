#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define THREAD_NUM 16

sem_t semaphore;

void *routine(void *args){
  printf("(%d) waiting to login\n", *(int *)args);
  sem_wait(&semaphore);
  printf("Login %d\n", *(int *)args);
  sleep(1);
  printf("Logout %d\n", *(int *)args);
  sem_post(&semaphore);
  free(args);
}

int main(int argc, char *argv[]) {
  pthread_t threads[THREAD_NUM];
  sem_init(&semaphore, 0, 4);
  int i;
  for (i = 0; i < THREAD_NUM; i++) {
    int* a  = malloc(sizeof(int));
    *a = i;
    pthread_create(&threads[i], NULL, &routine, a);
  }

  for (i = 0; i < THREAD_NUM; i++) {
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&semaphore);

  return 0;
}