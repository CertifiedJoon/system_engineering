#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int pid = fork();
  if (pid == 0) {
    printf("Hello World, %d\n", getpid());
  } else {
    printf("Hello World , %d\n", getpid());
  }

  return 0;
}