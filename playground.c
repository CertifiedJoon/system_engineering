#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc > 2){
    return -1;
  }

  int* arr = (int*)malloc(atoi(argv[1]) * sizeof(int));

  for (int i = 0; i < atoi(argv[1]); i++) {
    arr[i] = i * i;
    printf("%d\t", arr[i]);
  }
  printf("\n");
}