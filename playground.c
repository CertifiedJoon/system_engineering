#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 4
#define NUM_ITERATIONS 1000000

int counter = 0;

sem_t mutex;

void *count(void *arg){
  for (int i = 0; i < NUM_ITERATIONS; i++){
    sem_wait(&mutex);
    counter++;
    sem_post(&mutex);
  }

  return NULL;
}


int main(int argc, char *argv []){
  pthread_t threads[NUM_THREADS];

  sem_init(&mutex, 0, 1);

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, count, NULL);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&mutex);

  printf("Final counter value: %d\n", counter);
  printf("Expected value: %d\n", NUM_ITERATIONS * NUM_THREADS);

  return 0;
}