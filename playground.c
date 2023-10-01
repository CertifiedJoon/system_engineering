#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig10Handler(int sig_num){
  exit();
}


int main(int argc, char *argv[]){
  signal(10, sig10Handler);

  while(true){
    sleep(1);
    printf("still running");
  }

  return 0;
}
