#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_CMD_LEN 128

void parse(char *cmd, char **args) {
    while (*cmd != '\0') {
        while (*cmd == ' ' || *cmd == '\t' || *cmd == '\n') {
            *cmd++ = '\0';
        }
        *args++ = cmd;
        while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t' && *cmd != '\n') {
            cmd++;
        }
    }
    *args = '\0';
}

void execute(char **args) {
    pid_t pid;
    int status;

    if ((pid = fork()) < 0) {
        printf("Fork error\n");
        exit(1);
    } else if (pid == 0) {
        if (execvp(*args, args) < 0) {
            printf("Exec error\n");
            exit(1);
        }
    } else {
        while (wait(&status) != pid);
    }
}

int main() {
    char cmd[MAX_CMD_LEN];
    char *args[MAX_CMD_LEN];

    while (1) {
        printf("myshell> ");
        
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
            break;
        }

        // Remove trailing newline character
        if (cmd[strlen(cmd)-1] == '\n') {
            cmd[strlen(cmd)-1] = '\0';
        }

        // Exit the shell if the user types 'exit'
        if (strcmp(cmd, "exit") == 0) {
            break;
        }

        parse(cmd, args);
        execute(args);
    }

    return 0;
}
