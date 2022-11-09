/* Author: Christina Moore
 * Assignment: Smallsh
 * Class: OSU CS 344 - Operating Systems
 * Due: Nov. 14th 2022
 */

#define _POSIX_SOURCE

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <util.h>

//global fg flag
int isFgMode = 0;
//global bkg mode flag
int isBkg = 0;

//define maxes
#define ARGS_MAX_SIZE 	    512
#define COMMAND_MAX_SIZE 	2048

//chars to compare to for commands
char* bkg = "&";
char* comment = "#";
char* expand = "$$";
char* moveDir = "cd";
char* status = "status";
char* exitSmallsh = "exit";
//    exit flag
int exitFlag = 0;
//    foreground command run?
int fgCommandRun = 0;
//    args list
char* argsGlobal[ARGS_MAX_SIZE];
int argsCount = 0;
//    child fork list
int forkList[100] = {0};
int forks = 0;
int forkStatus;

//function to parse commands
int parseInput(char* args);
//function to execute commands
void doCmds(void);
//helper to check for background command
void isBkgCmd(void);
//function to turn foreground only on
void fgModeOn(int);
//function to turn foreground only off
void fgModeOff(int);
//function to move dirs
void cdCmd(void);
//print status function
void statusCmd(int* passStatus);
//exit smallsh
void exitCmd(void);
//exec/fork/waitpid commands handler
void execCmds(int* passStatus);
//signal handler
void handle_SIGINT(int);
void handle_SIGTSTP(int);

struct sigaction SIGTSTP_action = {0};
struct sigaction SIGINT_action = {0};

int main(int argc, char *argv[]) {
//get pid of shell
    pid_t shellPid = getpid();
//list of raw args from user
    char rawArgs[ARGS_MAX_SIZE];
/*
 * main():
    block SIGTSTP
    register fgmode_on() for SIGTSTP
    loop:
       unblock SIGTSTP
       read_next_line()
       block SIGTSTP
       process_line()
       fork_exec()
       if fg or fg_only:
           wait()
 *
 */
// taken from module 5, SIGINT block (CTRL-C)
    SIGINT_action.sa_handler = SIG_IGN;
    sigfillset(&SIGINT_action.sa_mask);
    SIGINT_action.sa_flags = 0;
    sigaction(SIGINT, &SIGINT_action, NULL);

//    SIGTSTP (CTRL-Z) handler
    SIGTSTP_action.sa_handler = handle_SIGTSTP;
//    from module 5, do not block
    SIGTSTP_action.sa_flags = SA_RESTART;
    sigfillset(&SIGTSTP_action.sa_mask);
    sigaction(SIGTSTP, &SIGTSTP_action, NULL);

    while(1) {
        argsCount = parseInput(rawArgs);
        argsGlobal[argsCount] = NULL;
        doCmds();
    };
    return 0;
}

/*
 * FUNCTION: handle_SIGTSTP(int)
 * ---------------------------------------
 * Signal handler for SIGTSTP. Calls fgModeOn.
 *
 * Args: Int - signal
 *
 * Returns: None
 */
void handle_SIGTSTP(int signo){
    fgModeOn(1);
}

/*
 * FUNCTION fgModeOn(int)
 * ---------------------------------------
 * Toggles foreground-only mode on when signal is received. Returns message to user.
 *
 * Args: int - foreground signal
 *
 * Returns: None
 */
void fgModeOn(int sig) {
//    register fgmode_off for SIGTSTP
    isFgMode = 1;
    char* message = "Entering foreground-only mode (& is now ignored)\n";
    char* prompt = ": ";
    write(STDOUT_FILENO, message, 50);
    write(STDOUT_FILENO, prompt, 2);
}

/*
 * FUNCTION fgModeOff(int)
 * ---------------------------------------
 * Toggles foreground-only mode on when signal is received. Returns message to user.
 *
 * Args: int - foreground signal
 *
 * Returns: None
 */
void fgModeOff(int sig) {
//    register fgmode_on() for SIGTSTP
    isFgMode = 0;
    char* message = ("Exiting foreground-only mode\n");
    char* prompt = ": ";
    write(STDOUT_FILENO, message, 30);
    write(STDOUT_FILENO, prompt, 2);
}
/*
 * FUNCTION: parseInput(char*)
 * ---------------------------------------
 * Takes raw args from user and saves them to the parsedArgs list for later use.
 *
 * Args: args - char* of args[2048]
 *
 * Returns: number of arguments in int form
 */
int parseInput(char* args) {
//    prompt user ':' and flush
    int localArgsCount = 0;
    printf(": ");
    fflush(stdout);
    fgets(args, COMMAND_MAX_SIZE, stdin);
//    remove \n from rawArgs
    strtok(args, "\n");

//    taken from https://beej.us/guide/bgc/html/split/stringref.html#man-strtok - common strtok usage loop
    char* argToken;
    int i;
    if ((argToken = strtok(args, " ")) != NULL) {
        do {
//            reset background mode
            if (isBkg == 1) {
                isBkg = 0;
            };

            // put argToken in global args list
            argsGlobal[localArgsCount] = argToken;

            for(i=0;i < strlen(argsGlobal[localArgsCount]); i++) {
                if (argsGlobal[localArgsCount][i] == '$' && argsGlobal[localArgsCount][i+1] == '$') {
                    argsGlobal[localArgsCount][i] = '\0';
                    argsGlobal[localArgsCount][i+1] = '\0';
                    int pid = getpid();
                    sprintf(argsGlobal[localArgsCount], "%s%d", argsGlobal[localArgsCount], pid);
                }
            };
            localArgsCount++;
        } while ((argToken = strtok(NULL, " ")) != NULL);
    };
    return localArgsCount;
}

/*
 * FUNCTION doCmds(void)
 * ---------------------------------------
 * Takes commands parsed in parseInput() and checked them against command names (cd, exit, etc.)
 *
 * Args: None
 *
 * Returns: None
 */
void doCmds(void) {
    char* command = argsGlobal[0];
    int passStatus = 0;

//    command is comment '#'
    if (command[0] == '#' || command[0] == '\n') {
//       eat comments and new lines
    }
//    command is 'cd'
    else if (strcmp(command, moveDir) == 0) {
        cdCmd();
    }
//    command is 'status
    else if (strcmp(command, status) == 0) {
        statusCmd(&passStatus);
    }
//    command is 'exit'
    else if (strcmp(command, exitSmallsh) == 0) {
        exitCmd();
    }
    else {
        execCmds(&passStatus);
    };
}

/*
 * FUNCTION isBkgCmd()
 * ---------------------------------------
 * Helper for doCmds. Checks if entered command is background and sets global flag
 *
 * Args: None
 *
 * Returns: None
 */
void isBkgCmd() {
    char* lastCommand = argsGlobal[argsCount - 1];
    if (strcmp(lastCommand, bkg) == 0) {
        printf("bkg cmd processed\n");
//        don't allow bkg if foreground only mode on
        if (isFgMode == 1) {
            printf("Error: shell is in foreground only mode, background processes not allowed");
            isBkg = 0;
        } else isBkg = 1;
    }
}

/*
 * FUNCTION cdCmd()
 * ---------------------------------------
 * Handles "cd" call from user.
 *  - only "cd", will put user in HOME
 *  - path specified, will move user
 *
 * Args: None
 *
 * Returns: None
 */
void cdCmd() {
    int errorFlag = 0;
//    no dir specified
    if (argsCount == 1) {
//        E.T. go home
        errorFlag = chdir(getenv("HOME"));
    }
    else {
        errorFlag = chdir(argsGlobal[1]);
    }

    if (errorFlag != 0) {
        printf("Error cd failed\n");
        fflush(stdout);
    }
}

/*
 * FUNCTION statusCmd(int)
 * ---------------------------------------
 * Prints out either the exit status or the terminating signal of the last foreground process.
 * The three built-in shell commands do not count as foreground processes for the purposes of this built-in command,
 * status will ignore built-in commands.
 *
 * Args: int - 1 or 0 statusCalled flag
 *
 * Returns: None
 */
void statusCmd(int* passStatus) {
    if(forkList[0] == 0) {
        printf("Exit status %d\n", forkStatus);
    } else {
        if (WIFEXITED(*passStatus)) {
            printf("Process terminated with exit code %d.\n", WEXITSTATUS(*passStatus));
        } else if (WIFSIGNALED(*passStatus)) {
            printf("Process terminated by signal %d.\n", WTERMSIG(*passStatus));
        }
        fflush(stdout);
    }
}

/*
 * FUNCTION exitCmd()
 * ---------------------------------------
 * The exit command exits the shell.
 * Shell will kill any other processes or jobs before it terminates itself.
 *
 * Args: None
 *
 * Returns: None
 */
void exitCmd() {
    int i;
    if (forks != 0) printf("Terminating processes...\n");
    for(i = 0; i < forks; i++) kill(forkList[i], SIGTERM);
    printf("Exiting smallsh, goodbye.\n");
    exitFlag = 1;
    exit(0);
}

/*
 * FUNCTION execCmds(int)
 * ---------------------------------------
 * Non build in command handler. Will fork a child.
 *
 * Source: Module 4
 *
 * Args: int - 1 or 0 statusCalled flag
 *
 * Returns: None
 */
void execCmds(int* passStatus) {
//    Taken from Module 4
//    check for isBkg
//    check for fgOnlyMode
//      flip fgCommandRun flag
    fgCommandRun = 1;
    // Fork a new process
    pid_t spawnPid = fork();
//    add to list of forks for exit call
    forkList[forks] = spawnPid;
    forks++;
//    to trim &
    char* lastCommand = argsGlobal[argsCount - 1];

    if (strcmp(lastCommand,"&") == 0) {
//        remove &
        isBkgCmd();
        argsGlobal[argsCount - 1] = NULL;
}
    int i;
    char* inputFile = NULL;
    char* outputFile = NULL;
    switch(spawnPid){
//        error
        case -1:
            perror("Error: Fork failed\n");
//            TODO: Fix exitFlag/exitCmd and set status to 1
            exit(1);
        case 0:
//            check for input / output redirection
            for(i = 0; argsGlobal[i] != NULL; i++) {
                if (strcmp(argsGlobal[i], "<") == 0) {
//                        remove < from args list
                    argsGlobal[i] = NULL;
                    inputFile = argsGlobal[i + 1];
                    i++;
                }
                else if (strcmp(argsGlobal[i], ">") == 0) {
//                        remove > from args list
                    argsGlobal[i] = NULL;
                    outputFile = argsGlobal[i + 1];
                    i++;
                }
            }

            if (inputFile != NULL) {
                int file;
                if ((file = open(inputFile, O_RDONLY)) == -1) {
                    printf("Error: Cannot open file for input\n");
                    fflush(stdout);
                    exit(1);
                } else {
                    dup2(file, 0);
                    close(file);
                }
            }

            if (outputFile != NULL) {
                int file2;
                if ((file2 = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC)) == -1) {
                    printf("Error: Cannot open file for output\n");
                    fflush(stdout);
                    exit(1);
                } else {
                    dup2(file2, 0);
                    close(file2);
                }
            }
//          if fg only ON remove previous ignore for CTRL-C
            if(isFgMode) {
                SIGINT_action.sa_handler = SIG_DFL;
                sigaction(SIGINT, &SIGINT_action, NULL);
            }

            execvp(argsGlobal[0], argsGlobal);
            // TODO: Fix exitFlag/exitCmd and set status to 1
            printf("Error: No such file or directory\n");
            exit(1);

        default:
            // In the parent process
            // Wait for child's termination
            if(isBkg == 1) {
                printf(":");
                fflush(stdout);
                spawnPid = waitpid(spawnPid, &forkStatus, WNOHANG);
                printf("background pid is %d done: ", spawnPid);
                fflush(stdout);
                statusCmd(passStatus);
            }
            else {
                waitpid(spawnPid, &forkStatus, 0);
            }
    }
}


