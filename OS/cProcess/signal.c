#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char *argv[]){
  int pid = fork();

  if (pid == -1) {return 1;}

  if (pid == 0) {
    // child process
    while(1) {
      printf("Some text goes here\n");
      usleep(50000);
    }
  } else {
    sleep(1);
    kill(pid, SIGKILL);
    wait(NULL);
  }

  return 0;
}