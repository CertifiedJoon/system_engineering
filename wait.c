#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[]){
  int id = fork();
  int n;

  if (id == 0) {
    n = 1;
  } else {
    n = 6;
    waitpid(id);
  }

  int i;

  for (i = n; i < n + 5; i++) {
    printf("%d ", i);
    // allows printf to actually print as it is called. not buffered.
    fflush(stdout);
  }
  
  if (id != 0){
    printf("\n");
  }

  return 0;
}