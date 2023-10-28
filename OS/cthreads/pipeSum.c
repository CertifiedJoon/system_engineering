#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main (int argc, char* argv[]) {
  int fd[2];
  int arr[] = {1,4,3,6,2,3,4};
  int arrSize = sizeof(arr) / sizeof(int);
  int start, end;

  if (pipe(fd) == -1){
    return 1;
  }

  int id = fork();

  if (id == -1) {
    return 2;
  }

  if (id == 0) {
    //child process
    start = 0;
    end = arrSize / 2;
  } else {
    start = arrSize / 2;
    end = arrSize;
  }
  
  int sum = 0;

  for (int i = start; i < end; i++)
    sum += arr[i];
  
  if (id == 0) {
    // child process
    close(fd[0]);
    write(fd[1], &sum, sizeof(sum));
    close(fd[1]);
  } else {
    close(fd[1]);
    int sumFromChild;
    read(fd[0], &sumFromChild, sizeof(sumFromChild));
    close(fd[0]);
    printf("Total sum is %d\n", sum + sumFromChild);
    wait(NULL);
  }

  return 0;
}