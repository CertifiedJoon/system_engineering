#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void signalHandler(int signal_no){
  exit(0);
}

int main(int arg, char *argv[]){
  signal(10, signalHandler);


}