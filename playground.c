#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 4
#define NUM_ITER 1000000

sem_t mutex;
int counter = 0;

void *count(void *arg){
  for (int i = 0; i < NUM_ITER; i++) {
    sem_wait(&mutex);
    counter += 1;
    sem_post(&mutex);
  }
  int value = 0;
  return (void *) (value);
}

int main(int argc, char *argv[]){
  pthread_t threads[NUM_THREADS];

  sem_init(&mutex, 0, 1);
  
  for (int i = 0; i < NUM_THREADS; i++){
    pthread_create(&threads[i], NULL, count, (void *) 100);
  }

  int rvalue;

  for (int i = 0; i < NUM_THREADS; i++){
    pthread_join(threads[i], (void *) &rvalue);
  }

  sem_destroy(&mutex);

  printf("counted %d\n", counter);

  return 0;
}