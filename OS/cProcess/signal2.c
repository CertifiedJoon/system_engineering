#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handle_sigstp(int signum) {
  printf("Stop not allowed\n");
}

void handle_sigcont(int signum) {
  printf("Input number: ");
  fflush(stdout);
}

int main(int argc, char* argv[]) {
  signal(SIGCONT, &handle_sigcont);

  int x;
  printf("input number: ");
  scanf("%d", &x);
  printf("Result %d * 5 = %d\n", x, x*5);

  return 0;
}