int main(int argc, char *argv[]){
  char *delim = " |";

  char *str = "ls | wc - l";

  char *token = strtoken(str, delim);

  while (token != NULL){
    printf("%d\n", token);
    token = strtoken(NULL, delim);
  }

}