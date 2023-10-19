#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 100

void execute_command(char *command) {
    char *args[MAX_ARGS];
    int i = 0;

    // Tokenize the command string
    char *token = strtok(command, " \t\n");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i] = token;
        i++;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL; // Null-terminate the argument list

    if (i == 0) {
        // Empty command
        return;
    }

    // Check for built-in commands (e.g., "exit")
    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    // Initialize an array of pipes
    int num_pipes = 0;
    int pipe_fds[MAX_ARGS][2]; // Array to hold pipe file descriptors

    // Check for pipes (commands separated by "|")
    for (int j = 0; j < i; j++) {
        if (strcmp(args[j], "|") == 0) {
            args[j] = NULL; // Terminate the current command
            if (pipe(pipe_fds[num_pipes]) == -1) {
                perror("pipe");
                return;
            }
            num_pipes++;
        }
    }

    if (num_pipes > 0) {
        pid_t pids[MAX_ARGS];
        int status;

        for (int j = 0, cmd_start = 0; j <= num_pipes; j++) {
            // Find the next command start index after a pipe
            while (args[cmd_start] == NULL) {
                cmd_start++;
            }

            // Create a new array for the arguments of the current command
            char *cmd_args[MAX_ARGS];
            int cmd_argc = 0;
            while (args[cmd_start] != NULL) {
                cmd_args[cmd_argc] = args[cmd_start];
                cmd_argc++;
                cmd_start++;
            }
            cmd_args[cmd_argc] = NULL; // Null-terminate the argument list

            // Fork a child process for each command in the pipeline
            if ((pids[j] = fork()) == 0) {
                // Child process
                if (j < num_pipes) {
                    // Redirect output to the next pipe (except for the last command)
                    dup2(pipe_fds[j][1], STDOUT_FILENO);
                    close(pipe_fds[j][0]); // Close the read end of the current pipe
                }
                if (j > 0) {
                    // Redirect input from the previous pipe (except for the first command)
                    dup2(pipe_fds[j - 1][0], STDIN_FILENO);
                    close(pipe_fds[j - 1][1]); // Close the write end of the previous pipe
                }

                // Execute the current command
                execvp(cmd_args[0], cmd_args);
                perror("execvp");
                exit(1);
            } else if (pids[j] < 0) {
                perror("fork");
                return;
            }

            if (j < num_pipes) {
                // Close pipe file descriptors in the parent process (except for the last command)
                close(pipe_fds[j][1]); // Close the write end of the current pipe
            }
        }

        // Wait for all child processes to finish
        for (int j = 0; j <= num_pipes; j++) {
            waitpid(pids[j], &status, 0);
        }
    } else {
        // No piping, execute the command as before
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        } else if (pid > 0) {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        } else {
            // Forking failed
            perror("fork");
        }
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        printf("MyShell> ");
        fflush(stdout);

        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
            break;
        }

        // Remove the newline character
        command[strcspn(command, "\n")] = '\0';

        // Execute the command
        execute_command(command);
    }

    return 0;
}
