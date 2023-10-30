#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>


int main(int argc, char *argv[]){
  int fd[2];
  if (pipe(fd) == -1) { return 1;}

  int pid = fork();
  if (pid == -1 ) {return 2;}

  if (pid == 0) {
    // child process

    int n, i;
    int arr[10];
    srand(time(NULL));
    n =  rand() % 10 + 1;
    printf("Generated: ");
    for (i = 0; i < n; i++) {
      arr[i] =  rand() % 11;
      printf("%d ", arr[i]);
    }
    printf("\n");

    close(fd[0]);
    if (write(fd[1], &n, sizeof(int)) == -1) {return 3;}
    printf("Sent n = %d\n", n);

    if (write(fd[1], &arr, sizeof(int) * n) == -1) {return 3;}
    printf("Sent Array\n");

    close(fd[1]);
  } else {
    int n, i;
    int arr[10];

    close(fd[1]);
    if (read(fd[0], &n, sizeof(int)) == -1) {return 3;}
    printf("Received n = %d\n", n);

    if (read(fd[0], &arr, sizeof(int) * n) == -1) {return 3;}
    printf("Received Array\n");

    int sum = 0;
    for (i = 0; i < n; i++){
      sum += arr[i];
    }
    printf("Sum: %d\n", sum);
    wait(NULL);

    close(fd[0]);
  }
}