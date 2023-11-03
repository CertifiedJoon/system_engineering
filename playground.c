#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <signal.h>


int main(int argc, char *argv[]) {
  int id = fork();  
  if (id == 0) {
    while(1) sleep(1);
  } else {
    kill(id, SIGKILL);
    wait(NULL);
  }

  return 0;
}