#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unitsd.h>

int main(int argc, char *argv[]){
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_lock(&lock);
  balance = balance + 1;
  pthread_mutex_unlock(&lock);
}