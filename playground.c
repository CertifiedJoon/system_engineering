#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *mythread(void *val){
  long long int i = (long long int) val;
  i++;
  return (void *) i;
}

int main(int argc, char *argv[]){
  pthread_t thread;

  pthread_create(&thread, NULL, mythread, (void *) 100);
  long long int val;
  pthread_join(thread, (void *) &val);
  printf("returnd %lld\n", val);
}