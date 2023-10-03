#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *mythread(void *arg){
  long long int val = (long long int) arg;
  return (void *) (val + 1);
}

int main(int argc, char *argv[]) {
  pthread_t p;

  pthread_create(&p, NULL, mythread, (void *)100);
  
  long long int val;

  pthread_join(p, (void **) &val);

  printf("%lld\n", val);

  return 0;
}