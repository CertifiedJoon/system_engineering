#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define SERVER_CAP = 12

sem_t logged;

void* serverWelcome(void* args) {
  sem_wait(&logged);
  sleep(2);
  printf("Logged in %d\n", *(int *) args);
  sem_post(&logged);
  printf("Logged out %d\n", *(int *) args);
}

int main(int argc, char *argv[]) {
  sem_init(&logged, 0, 12);
  pthread_t threads[20];

  for (int i = 0; i < 20; i++) {
    int *a = malloc(sizeof(int));
    *a = i;
    pthread_create(&threads[i], NULL, serverWelcome, a);
  }

  for (int i = 0; i < 20; i++) {
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&logged);
  return 0;
}