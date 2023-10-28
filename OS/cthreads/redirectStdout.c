#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
  int pid = fork();

  if (pid == -1) {return 1;}
  if (pid == 0) {
    // child process
    int file = open("pingResults.txt", O_WRONLY | O_CREAT, 0777);
    if (file == -1) { return 2;}
    int file2 = dup2(file, 1); // returns a file descriptor
    close(file);
    execlp("ping", "ping", "-c", "3", "google.com", NULL);
  } else {
    // parent process
    int wstatus;
    wait(&wstatus);
    if (WIFEXITED(wstatus)) {
      int statusCode = WEXITSTATUS(wstatus);
      if (statusCode == 0) {
        printf("Success!\n");
      } else {
        printf("Failure! Status Code %d\n", statusCode);
      }
    }
    printf("Some post processing goes here!\n");
  } 
  
  return 0;
}