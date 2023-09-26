#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sigusrHandler(int sig_num){
  exit(0);
}

int main(){
  signal(10, sigusrHandler);

  FILE *fp;
  fp = fopen("pid.txt", "w");
  fprintf(fp, "%d", getpid());
  fclose(fp);

  while(1) {
    printf("Still running...\n");
    sleep(1);
  }

  return 0;
}