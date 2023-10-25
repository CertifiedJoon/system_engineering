#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 100

// Define a CommandExecutor class
typedef struct {
    char *args[MAX_ARGS];
    int numArgs;
    int numPipes;
    int pipeFds[MAX_ARGS][2];
} CommandExecutor;

// Function to initialize a CommandExecutor instance
void initCommandExecutor(CommandExecutor *executor, char *command) {
    executor->numArgs = 0;
    executor->numPipes = 0;

    // Tokenize the command string
    char *token = strtok(command, " \t\n");
    while (token != NULL && executor->numArgs < MAX_ARGS - 1) {
        executor->args[executor->numArgs] = token;
        executor->numArgs++;
        token = strtok(NULL, " \t\n");
    }
    executor->args[executor->numArgs] = NULL; // Null-terminate the argument list

    // Check for built-in commands (e.g., "exit")
    if (executor->numArgs > 0 && strcmp(executor->args[0], "exit") == 0) {
        if (executor->numArgs > 1) {
            printf("JCshell: exit with other arguments\n");
        } else {
            exit(0);
        }
    }

    // Check for pipes (commands separated by "|")
    for (int j = 0; j < executor->numArgs; j++) {
        if (strcmp(executor->args[j], "|") == 0) {
            executor->args[j] = NULL; // Terminate the current command
            if (pipe(executor->pipeFds[executor->numPipes]) == -1) {
                perror("JCshell");
            }
            executor->numPipes++;
        }
    }
}

// Define a Shell class to encapsulate shell functionality
typedef struct {
    CommandExecutor* executor;
    pid_t pid;
} Shell;

// Define a ProcessStats class to encapsulate process statistics
typedef struct {
    pid_t pid;
    char command[MAX_COMMAND_LENGTH];
    char state;
    int parentPid;
    int exitCode;
    unsigned long userTime;
    unsigned long sysTime;
    unsigned long voluntaryCtxtSwitches;
    unsigned long nonvoluntaryCtxtSwitches;
} ProcessStats;

int isWaiting = 0;

void ignoreCtrlC(int signum){
    // Ignore Ctrl-C
    ;
}

void handleSignal(int signum)
{
    isWaiting = 1;
}

void customPause()
{
    while (!isWaiting);
    isWaiting = 0;
}

// Function to initialize a ProcessStats instance
void initProcessStats(ProcessStats *stats, pid_t pid) {
    stats->pid = pid;
    // Initialize other fields as needed
}


void getProcessStatistics(pid_t pid, siginfo_t *info) {
    char statFilename[100];
    snprintf(statFilename, sizeof(statFilename), "/proc/%d/stat", pid);

    // Check if the file exists
    if (access(statFilename, F_OK) != 0) {
        perror("JCshell: /proc/{pid}/stat file does not exist");
        return;
    }

    FILE* statFile = fopen(statFilename, "r");
    char command[256];
    char state;
    int parentPid, exitCode, _d;
    unsigned long userTime, sysTime, _lu;
    unsigned _u;
    long _ld;
    unsigned long long _llu, startTime;

    if (statFile) {
        if (fscanf(statFile, "%d %s %c %d %d", &_d, command, &state, &parentPid, &_d) == 5) {
            fscanf(statFile, "%d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld",
                   &_d, &_d, &_d, &_u, &_lu, &_lu, &_lu, &_lu, &userTime, &sysTime, &_ld, &_ld, &_ld, &_ld);
            fscanf(statFile, "%ld %ld %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %u %u %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %d",
                   &_ld ,&_ld ,&_llu ,&_lu ,&_ld ,&_lu , &_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_d ,&_d ,&_u ,&_u ,&_llu ,&_lu ,&_ld ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu ,&_lu , &exitCode);
            
            fclose(statFile);
        } else {
            perror("JCshell: Error reading stat file");
        }
    } else {
        perror("JCshell: Error opening stat file");
    }

    // Now, read context switches from /proc/{pid}/status
    snprintf(statFilename, sizeof(statFilename), "/proc/%d/status", pid);
    FILE* statusFile = fopen(statFilename, "r");

    char line[256];
    unsigned long voluntaryCtxtSwitches, nonvoluntaryCtxtSwitches;

    if (statusFile) {
        while (fgets(line, sizeof(line), statusFile)) {
            if (strstr(line, "voluntary_ctxt_switches:")) {
                sscanf(line, "voluntary_ctxt_switches: %lu", &voluntaryCtxtSwitches);
            } else if (strstr(line, "nonvoluntary_ctxt_switches:")) {
                sscanf(line, "nonvoluntary_ctxt_switches: %lu", &nonvoluntaryCtxtSwitches);
            }
        }
        fclose(statusFile);
    } else {
        perror("JCshell: Error opening status file");
    }
    
    int status;
    waitpid(pid, &status, 0);

    // Print the obtained statistics
    printf("(PID)%d ", pid);
    printf("(CMD)%s ", command);
    printf("(STATE)%c ", state);
    if (WTERMSIG(status) == 11) {
        printf("(EXSIG)Segmentation fault ");
    } else if (WTERMSIG(status) == 2) {
        printf("(EXSIG)Interrupt ");
    } else if (exitCode >= 256) {
        printf("(EXCODE)%d ", exitCode / 256);
    } else {
        printf("(EXCODE)%d ", exitCode);
    }
    printf("(PPID)%d ", parentPid);
    printf("(USER)%.2f ", userTime * 1.0 / (double) sysconf(_SC_CLK_TCK));
    printf("(SYS)%.2f ", sysTime * 1.0 / (double) sysconf(_SC_CLK_TCK));
    printf("(VCTX)%lu ", voluntaryCtxtSwitches);
    printf("(NVCTX)%lu\n", nonvoluntaryCtxtSwitches);

    fflush(stdout);
}




// Function to execute the command
void execute(CommandExecutor *executor) {
    if (executor->numPipes > 0) {
        pid_t pids[MAX_ARGS];

        for (int j = 0, cmdStart = 0; j <= executor->numPipes; j++) {
            while (executor->args[cmdStart] == NULL) {
                cmdStart++;
            }

            char *cmdArgs[MAX_ARGS];
            int cmdArgc = 0;
            while (executor->args[cmdStart] != NULL) {
                cmdArgs[cmdArgc] = executor->args[cmdStart];
                cmdArgc++;
                cmdStart++;
            }
            cmdArgs[cmdArgc] = NULL;

            if ((pids[j] = fork()) == 0) {
                if (j < executor->numPipes) {
                    dup2(executor->pipeFds[j][1], STDOUT_FILENO);
                    close(executor->pipeFds[j][0]);
                }
                if (j > 0) {
                    dup2(executor->pipeFds[j - 1][0], STDIN_FILENO);
                    close(executor->pipeFds[j - 1][1]);
                }

                customPause();
                execvp(cmdArgs[0], cmdArgs);
                perror("JCshell");
                exit(1);
            } else if (pids[j] < 0) {
                perror("JCshell");
            }

            if (j < executor->numPipes) {
                close(executor->pipeFds[j][1]);
            }
        }

        for (int j = 0; j <= executor->numPipes; j++) {
            pid_t childPid = pids[j];
            siginfo_t info;
            int status;
            kill(childPid, SIGUSR1);
            waitid(P_PID, childPid, &info, WNOWAIT | WEXITED);
        }
        for (int j = 0; j <= executor->numPipes; j++) {
            pid_t childPid = pids[j];
            siginfo_t info;
            getProcessStatistics(childPid, &info);
        }
    } else {
        pid_t pid = fork();
        int status;

        if (pid == 0) {
            pid_t childPid = getpid();
            customPause();
            execvp(executor->args[0], executor->args);
            perror("JCshell");
            exit(1);
        } else if (pid > 0) {
            pid_t childPid = pid;
            siginfo_t info;
            kill(pid, SIGUSR1);
            waitid(P_PID, childPid, &info, WNOWAIT | WEXITED);
            getProcessStatistics(childPid, &info);
        } else {
            perror("JCshell");
        }
    }
}


// Function to read and execute user commands
void runShell(Shell *shell) {
    char command[MAX_COMMAND_LENGTH];
    while (1) {
        printf("## JCshell [%d] ## ", shell->pid);
        fflush(stdout);

        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
            break;

        initCommandExecutor(shell->executor, command);
        // Execute the command
        execute(shell->executor);
    }
}

int main() {
    Shell shell;
    shell.pid = getpid();

    // Register signal handlers
    signal(SIGINT, ignoreCtrlC);
    signal(SIGUSR1, handleSignal);

    runShell(&shell);

    return 0;
}