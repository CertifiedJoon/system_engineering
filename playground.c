#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

  Pthread_mutex_lock(&lock); // wrapper
  balance = balance + 1;
  Pthread_mutex_unlock(&lock);

  return 0;
}