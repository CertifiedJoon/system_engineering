#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv []) {
  int id = fork();
  if (id == 0) {
    // child process
    printf("Hello world");
  } else {
    // parent
    fork();
    printf("Hello World");
  }
}