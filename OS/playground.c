#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 4

sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

int buffer[10];
int count = 0;

void* producer() {
  while(1) {
    sem_wait(&semEmpty);
    pthread_mutex_lock(&mutexBuffer);
    buffer[count++] = rand() % 100;
    pthread_mutex_unlock(&mutexBuffer);
    sem_post(&semFull);
    sleep(1);
  }
}


void* consumer() {
  while(1) {
    sem_wait(&semFull);
    pthread_mutex_lock(&mutexBuffer);
    int y = buffer[--count];
    printf("consumed %d\n", y);
    pthread_mutex_unlock(&mutexBuffer);
    sem_post(&semEmpty);
    sleep(1);
  }
}


int main(int argc, char* argv[]) {
  pthread_t threads[THREAD_NUM];
  sem_init(&semEmpty, 0, 10);
  sem_init(&semFull, 0, 0);
  pthread_mutex_init(&mutexBuffer, NULL);

  for (int i = 0; i < THREAD_NUM; i++) {
    if (i % 2 == 0)
      pthread_create(&threads[i], NULL, producer, NULL);
    else
      pthread_create(&threads[i], NULL, consumer, NULL);
  }

  for (int i = 0; i < THREAD_NUM; i++)
    pthread_join(threads[i], NULL);

  sem_destroy(&semEmpty);
  sem_destroy(&semFull);
  pthread_mutex_destroy(&mutexBuffer);
}