#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  // Forks execution line -> child process is created to execute the following lines.
  // everything before fork is executed once, everything after fork is executed twice.
  // return -1 for error, 0 to the new process, pid of new process to the old process.
  int id = fork();
  if (id != 0){
    fork();
  }
  printf("Hello world from id: %d\n", id);
  
  // if (id == 0){
  //   printf("Hello from child process\n");
  // } else {
  //   printf("Hello from main process\n");
  // }

  return 0;
}