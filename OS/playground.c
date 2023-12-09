#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
int main(int argc, char* argv[]) {
  int p1[2];
  int p2[2];

  pipe(p1);
  pipe(p2);

  int pid = fork();

  if (pid == 0) {
    // child process
    close(p1[1]);
    close(p2[0]);
    int x;
    read(p1[0], &x, sizeof(x));
    x *= 4;
    write(p2[1], &x, sizeof(x));
    close(p1[0]);
    close(p2[1]);
  } else {
    // parent process
    close(p1[0]);
    close(p2[1]);
    srand(time(NULL));
    int y = rand() % 100;
    write(p1[1], &y, sizeof(y));
    printf("Wrote %d\n", y);
    read(p2[0], &y, sizeof(y));
    printf("Read %d\n", y);
    close(p1[1]);
    close(p2[0]);
  }

  return 0;
}