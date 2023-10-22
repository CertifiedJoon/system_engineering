/*

Name & UID
Joonyoung Moon 30356666657

Dev Platform
Visual Studio Code

Remark
All code other than those listed below is coded by myself.

Coded by GPT (Statistics reading from /proc/) & debugged by myself 
- https://chat.openai.com/share/cfcb1233-7990-46aa-af01-2436c58528f1

Coded by GPT/Phind (piping)
- Prompt
Would you debug this shell program in C? It won't execute the piped commands.
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

        for (int j = 0, cmd_start = 0; j <= num_pipes; j++, cmd_start++) {
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
                execvp(args[cmd_start], args + cmd_start);
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

==Answer:
The user's task is debugging a large amount of code.

Looking at the code, it seems like the issue might be with how the commands are being split and executed in the pipeline. The code currently increments cmd_start after each loop, which means it's skipping over the next command after a pipe. This could be the reason why the piped commands are not being executed.

To fix this, you need to find the next command start index after a pipe. This can be done by adding a loop that increments cmd_start until it finds a non-null argument after a pipe.

Here's the corrected part of the code:

for (int j = 0, cmd_start = 0; j <= num_pipes; j++) {
    // Find the next command start index after a pipe
    while (args[cmd_start] == NULL) {
        cmd_start++;
    }

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
        execvp(args[cmd_start], args + cmd_start);
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

    // Increment cmd_start for the next command
    cmd_start++;
}
Please replace the corresponding part of your code with this and try running your program again.

Let's confirm if this solution works for you. Also, I have a few more questions to better understand your situation:

Did the program work as expected after making the suggested changes?
Are there any other parts of the code that you're having trouble with?
Are you familiar with how piping works in a shell program?
Do you need help understanding any other parts of this code?
Would you like to know more about error handling in C?
User
It did not, 2. nope, 3. yes, 4. nope, 5. nope
PhindASK USER
question: Could you please provide the error message or describe the behavior you're experiencing when you try to run the program?

User
MyShell> ls -la | grep .txt execvp: No such file or directory

Phind
The error message "execvp: No such file or directory" typically means that the command you're trying to execute cannot be found. In your case, it seems like the execvp function is not receiving the correct arguments when a pipe is involved.

The issue lies in how you're splitting the commands and arguments. When you encounter a pipe symbol "|", you correctly set args[j] to NULL to terminate the current command. However, you're not correctly setting the start of the next command in the args array.

The cmd_start variable should be set to the index of the first argument of the next command after a pipe. However, in your current code, cmd_start is only incremented by 1 in each iteration of the loop, which means it points to NULL (the pipe symbol) instead of the start of the next command.

Here's the corrected part of the code:

for (int j = 0, cmd_start = 0; j <= num_pipes; j++) {
    // Find the next command start index after a pipe
    while (args[cmd_start] == NULL) {
        cmd_start++;
    }

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
        execvp(args[cmd_start], args + cmd_start);
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

    // Increment cmd_start for the next command
    cmd_start++;
}
Please replace the corresponding part of your code with this and try running your program again.

Let's confirm if this solution works for you. Also, I have a few more questions to better understand your situation:

Did the program work as expected after making the suggested changes?
Are there any other parts of the code that you're having trouble with?
Are you familiar with how piping works in a shell program?
Do you need help understanding any other parts of this code?
Would you like to know more about error handling in C?
User
No
PhindASK USER
question: Could you please provide the error message or describe the behavior you're experiencing after making the suggested changes?

User
MyShell> ls -l | grep .txt execvp: No such file or directory

Phind
I apologize for the confusion earlier. The issue seems to be with how the arguments are passed to the execvp function. When a pipe is involved, the arguments of the command after the pipe are not being passed correctly.

In your current code, you're passing args + cmd_start as the argument to execvp. However, this will include the arguments of the previous command as well because they are not being separated properly.

You need to create a new array for the arguments of each command in the pipeline and pass that to execvp. Here's how you can do it:

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
Please replace the corresponding part of your code with this and try running your program again.

Let's confirm if this solution works for you. Also, I have a few more questions to better understand your situation:

Did the program work as expected after making the suggested changes?
Are there any other parts of the code that you're having trouble with?
Are you familiar with how piping works in a shell program?
Do you need help understanding any other parts of this code?
Would you like to know more about error handling in C?

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 100

sem_t mutex;
int done_waiting = 0;

void sig2Handler(int signum){
    // ignore ctrl-c
    ;
}

void sig10Handler(int signum)
{
    done_waiting = 1;
}

void my_pause()
{
    while (!done_waiting);
    done_waiting = 0;
}

void get_process_statistics(pid_t pid, siginfo_t *info) {
    char filename[100];
    snprintf(filename, sizeof(filename), "/proc/%d/stat", pid);

    // Check if the file exists
    if (access(filename, F_OK) != 0) {
        perror("Error: /proc/{pid}/stat file does not exist");
        return;
    }

    FILE* stat_file = fopen(filename, "r");
    if (stat_file) {
        char cmd[256];
        char state;
        int ppid, exit_code, _d;
        unsigned long user_time, sys_time, _lu;
        unsigned _u;
        long _ld;
        unsigned long long _llu, start_time;

        if (fscanf(stat_file, "%d %s %c %d %d", &_d, cmd, &state, &ppid, &_d) == 5) {
            fscanf(stat_file, "%d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld",
                   &_d, &_d, &_d, &_u, &_lu, &_lu, &_lu, &_lu, &user_time, &sys_time, &_ld, &_ld, &_ld, &_ld);
            fscanf(stat_file, "%ld %ld %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %u %u %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %d",
                   &_ld ,&_ld ,&_llu ,&_lu ,&_ld ,&_lu , &_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_d ,&_d ,&_u ,&_u ,&_llu ,&_lu ,&_ld ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu , &exit_code);
            
            fclose(stat_file);

            // Print the obtained statistics
            printf("(PID)%d ", pid);
            printf("(CMD)%s ", cmd);
            printf("(STATE)%c ", state);
            printf("(EXCODE)%d ", exit_code);
            printf("(EXSIG)%d ", info->si_status);
            printf("(PPID)%d ", ppid);
            printf("(USER)%.2f ", user_time / (double) sysconf(_SC_CLK_TCK));
            printf("(SYS)%.2f ", sys_time / (double) sysconf(_SC_CLK_TCK));
            printf("%lu %lu", user_time, sys_time);
        } else {
            perror("Error reading stat file");
        }
    } else {
        perror("Error opening stat file");
    }

    // Now, read context switches from /proc/{pid}/status
    snprintf(filename, sizeof(filename), "/proc/%d/status", pid);
    FILE* status_file = fopen(filename, "r");
    if (status_file) {
        char line[256];
        unsigned long voluntary_ctxt_switches, nonvoluntary_ctxt_switches;
        while (fgets(line, sizeof(line), status_file)) {
            if (strstr(line, "voluntary_ctxt_switches:")) {
                sscanf(line, "voluntary_ctxt_switches: %lu", &voluntary_ctxt_switches);
            } else if (strstr(line, "nonvoluntary_ctxt_switches:")) {
                sscanf(line, "nonvoluntary_ctxt_switches: %lu", &nonvoluntary_ctxt_switches);
            }
        }
        fclose(status_file);

        // Print the context switch statistics
        printf("(VCTX)%lu ", voluntary_ctxt_switches);
        printf("(NVCTX)%lu\n", nonvoluntary_ctxt_switches);
    } else {
        perror("Error opening status file");
    }
    fflush(stdout);
}

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
                my_pause();
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
            pid_t child_pid = pids[j];
            siginfo_t info;
            int status;
            kill(child_pid, SIGUSR1);
            waitid(P_PID, child_pid, &info, WNOWAIT | WEXITED);
        }
        for (int j = 0; j <= num_pipes; j++){
            pid_t child_pid = pids[j];
            siginfo_t info;
            int status;
            get_process_statistics(child_pid, &info);
            waitpid(child_pid, &status, 0);
        }
    } else {
        // No piping, execute the command as before
        pid_t pid = fork();
        int status;

        if (pid == 0) {
            // Child process

            // Capture the PID of the child process
            pid_t child_pid = getpid();
            my_pause();
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        } else if (pid > 0) {
            // Capture and display the process statistics
            pid_t child_pid = pid;
            siginfo_t info;
            kill(pid, SIGUSR1);
            waitid(P_PID, child_pid, &info, WNOWAIT | WEXITED);
            get_process_statistics(child_pid, &info);
            waitpid(child_pid, &status, 0);
        } else {
            // Forking failed
            perror("fork");
        }
    }
}


int main() {
    char command[MAX_COMMAND_LENGTH];
    pid_t pid = getpid();
    char exit_command[10] = "exit";

    signal(SIGINT, sig2Handler);
    signal(SIGUSR1, sig10Handler);

    while (1) {
        printf("## JCShell [%d] ## ", pid);
        fflush(stdout);

        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
            break;

        if (strcmp(command, exit_command) == 1)
            exit(1);

        // Remove the newline character
        command[strcspn(command, "\n")] = '\0';

        // Execute the command
        execute_command(command);
    }

    return 0;
}
