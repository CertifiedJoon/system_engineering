#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *mythread(void *arg){
  long long int *val = (long long int*) arg;
  *val++;
  return (void *) &val;
}




int main(int argc, char *argv[]){
  pthread_t p;
  pthread_create(&p, NULL, mythread, (void *) 100);
  long long int rval;
  pthread_join(p, (void **) &rval);
  printf("%lld\n", rval);
}