#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *count() {
  return (void *) 100;
}

int main(int argc, char *argv[]) {
  pthread_t p;

  pthread_create(&p, NULL, count, NULL);
  int rval;
  pthread_join(p, (void **) &rval);
  printf("%d\n", rval);

  return 0;
}