#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv []){
  int id = fork();
  
  if (id == 0){
    // child process
    n = 1;
  } else {
    n = 6;
    waitpid(id);
  }

  for (int i = n; i < n + 5; i++) {
    printf("%d ", i);
    fflush();
  }

  if (id != 0) {
    printf("\n");
  }

  return 0;
}