#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv []){
  int pid = fork();
  
  if (pid == 0) {
    // child process
    printf("Hello World\n");
  } else {
    fork();
    printf("Hello World\n");
  }

  return 0;
}