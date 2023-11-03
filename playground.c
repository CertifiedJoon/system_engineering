#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>


int main(int argc, char *argv[]) {
  int fd[2];
  if (pipe(fd) == -2) return 1;
  int pid = fork();
  if (pid == -1) return 2;

  if (pid == 0) {
    int n, i;
    srand(time(NULL));
    n = rand() % 10 + 1;
    int *arr = (int *) malloc (sizeof(int) * n);

    for (i = 0; i < n; i++) {
      arr[i] = i;
      printf("%d ", arr[i]);
    }
    printf("\n");
    
    close(fd[0]);
    if (write(fd[1], &n, sizeof(int)) == -1) return 3;
    printf("wrote size from child\n");
    if (write(fd[1], arr, sizeof(int) * n) == -1) return 4;
    printf("wrote array from child\n");
    close(fd[1]);
  } else {
    int n, i;

    close(fd[1]);
    if (read(fd[0], &n, sizeof(int)) == -1) return 5;
    printf("read size %d\n", n);
    int *arr = (int*) malloc(sizeof(int) * n);
    if (read(fd[0], arr, sizeof(int) * n) == -1) return 6;
    printf("Received Array \n");
    close(fd[0]);

    int sum = 0;
    for (i = 0; i < n; i++) {
      sum +=  arr[i];
    }
    printf("%d\n", sum);
    wait(NULL);
  }

  return 0;
}