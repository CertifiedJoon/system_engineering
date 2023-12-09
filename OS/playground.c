#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define THREAD_NUM 4

sem_t semFull;
sem_t semEmpty;

pthread_mutex_t mutexBuffer;

int buffer[10];
int count = 0;

void* producer() {
  while (1){
    int x;
    sem_wait(&semEmpty);
    pthread_mutex_lock(&mutexBuffer);
    buffer[count++] = rand() % 100;
    printf("produced %d\n", x);
    pthread_mutex_unlock(&mutexBuffer);
    sem_post(&semFull);

    sleep(1);
  }
}

void* consumer() {
  while(1) {
    int y;
    sem_wait(&semFull);
    pthread_mutex_lock(&mutexBuffer);
    y = buffer[--count];
    printf("consumed %d\n", y);
    pthread_mutex_unlock(&mutexBuffer);
    sem_post(&semEmpty);

    sleep(1);
  }
}


int main (int argc, char* argv []) {
  sem_init(&semEmpty, 0, 10);
  sem_init(&semFull, 0, 0);

  srand(time(NULL));

  pthread_mutex_init(&mutexBuffer, NULL);

  pthread_t threads[THREAD_NUM];
  
  for (int i = 0; i < THREAD_NUM; i++){
    if (i % 2 == 0) pthread_create(&threads[i], NULL, producer, NULL);
    else pthread_create(&threads[i], NULL, consumer, NULL);
  }

  for (int i = 0; i < THREAD_NUM; i++) {
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&semEmpty);
  sem_destroy(&semFull);
  pthread_mutex_destroy(&mutexBuffer);
  return 0;
}