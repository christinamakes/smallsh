/* Author: Christina Moore
 * Assignment: Smallsh
 * Class: OSU CS 344 - Operating Systems
 * Due: Nov. 14th 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

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
//    args list
char* argsGlobal[ARGS_MAX_SIZE];
int argsCount = 0;
//    commands
char* cmd = NULL;
int cmdSize = 0;
//    dir string
char dirString[512];

//function to parse commands
int parseInput(char* args);
//function to execute commands
void doCmds(void);
//function to turn foreground only on
void fgModeOn(int);
//function to turn foreground only off
void fgModeOff(int);
//function to move dirs
void cdCmd(void);
//print status function
void statusCmd(int);
//exit smallsh
void exitCmd(void);
//exec/fork/waitpid commands handler
void execCmds(int);

int main(int argc, char *argv[]) {
//get pid of shell
    pid_t shellPid = getpid();
//list of raw args from user
    char rawArgs[ARGS_MAX_SIZE];
//while no exit flag argsCount is num returned from parseInput
//argsList init to NULL
    while (!exitFlag) {
        argsCount = parseInput(rawArgs);
        argsGlobal[argsCount] = NULL;
        doCmds();
    };
    return 0;
}

/*
 * FUNCTION: catchSigtstp(int)
 * ---------------------------------------
 * Signal handler for SIGTSTP. Calls fgModeOn or fgModeOff.
 *
 * Args: None
 *
 * Returns: None
 */
void catchSigtstp() {
//switch case (isFgMode)
    printf("hello catchSigtstp");
};

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
    printf(": ");
    fflush(stdout);
    fgets(args, COMMAND_MAX_SIZE, stdin);
//    remove \n from rawArgs
    strtok(args, "\n");

//    taken from https://beej.us/guide/bgc/html/split/stringref.html#man-strtok - common strtok usage loop
    char* argToken;
    if ((argToken = strtok(args, " ")) != NULL) {
        do {
//            put argToken in global args list
            argsGlobal[argsCount] = argToken;
            argsCount++;
        } while ((argToken = strtok(NULL, " ")) != NULL);
    };
    return argsCount;
};

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
    char* lastCommand = argsGlobal[argsCount - 1];
    int statusCalled = 0;

//    command is comment '#'
    if (strcmp(command, comment) == 0) {
        // eat comments
    }
//   command contains '&' as last character
    else if (strcmp(lastCommand, bkg) == 0) {
        printf("bkg mode on\n");
//        don't allow bkg if foreground only mode on
        if (isFgMode == 1) {
            isBkg = 0;
        } else isBkg = 1;
    }
//    command is '$$'
    else if (strcmp(command, expand) == 0) {
        printf("variable expansion\n");
    }
//    command is 'cd'
    else if (strcmp(command, moveDir) == 0) {
        cdCmd();
    }
//    command is 'status
    else if (strcmp(command, status) == 0) {
        statusCmd(statusCalled);
    }
//    command is 'exit'
    else if (strcmp(command, exitSmallsh) == 0) {
        exitCmd();
    }
    else {
        printf("Exec commands\n");
        execCmds(statusCalled);
    };
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
    char* dir = argsGlobal[1];
//    no dir specified
    if (argsCount == 1) {
//        E.T. go home
        errorFlag = chdir(getenv("HOME"));
    }
    else {
        errorFlag = chdir((dir));
    }
//TODO: DONT FORGET PWD IN execCmds()
    if (errorFlag != 0) {
        printf("Error cd command failed\n");
        fflush(stdout);
    }
    printf("cd command\n");
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
void statusCmd(int called) {
    printf("status command\n");
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
    printf("exit command\n");
    exitFlag = 1;
}

/*
 * FUNCTION execCmds(int)
 * ---------------------------------------
 * Non build in command handler. Will fork a child.
 *
 * Args: int - 1 or 0 statusCalled flag
 *
 * Returns: None
 */
void execCmds(int called) {
//    check for isBkg
//    check for fgOnlyMode
    printf("fork command\n");
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
void fgmModeOn(int sig) {
//    register fgmode_off for SIGTSTP
    printf("Entering foreground-only mode (& is now ignored)\n");
    fflush(stdout);
    isFgMode = 1;
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
    printf("Exiting foreground-only mode\n");
    fflush(stdout);
    isFgMode = 0;
}
