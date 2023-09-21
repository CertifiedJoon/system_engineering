#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv []){
  int pid = fork();

  if (pid == 0) {
    // child process
    printf("Hello from child\n");
  } else {
    // parent
    printf("Hello i am %d's parent\n", pid);
  }

  return 0;
}