#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char* argv[]) {
  int fd[2];
  pipe(fd);

  int arr[] = {1,2,3,4,5,6,7};
  int arr_size = sizeof(arr) / sizeof(int);
  int start, end;
  int pid = fork();

  if (pid == 0) {
    // child process
    start = arr_size / 2;
    end = arr_size;
  } else {
    start = 0;
    end = arr_size / 2;
  }

  int sum = 0;
  for (int i = start; i < end; i++) {
    sum += arr[i];
  }

  if (pid == 0) {
    close(fd[0]);
    write(fd[1], &sum, sizeof(sum));
    close(fd[1]);
  } else {
    close(fd[1]);
    int sumFromChild;
    read(fd[0], &sumFromChild, sizeof(sumFromChild));
    close(fd[0]);
    wait(NULL);
    printf("total sum %d\n", sumFromChild + sum);
  }

  return 0;
}