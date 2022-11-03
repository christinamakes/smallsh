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

//signal handler. also handles fgModeOn or fgModeOff
void catchSigtstp(void);
//function to parse commands
int parseInput(char* args);
//function to execute commands
void doCmds(void);
//function to turn foreground only on
void fgModeOn(int);
//function to turn foreground only off
void fgModeOff(int);
//function to move dirs
void cdCmd(const int*);
//print status function
void statusCmd(const int*);
//exit smallsh
void exitCmd(void);

int main(int argc, char *argv[]) {
//get pid of shell
    pid_t shellPid = getpid();
//list of raw args from user
    char rawArgs[ARGS_MAX_SIZE];


    printf("%d", shellPid);
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
 * Args: rawArgs - char of args[2048]
 *
 * Returns: number of arguments in int form
 */
int parseInput(char* args) {
    int numArgs = 0;
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
            argsGlobal[numArgs] = argToken;
            numArgs++;
        } while ((argToken = strtok(NULL, " ")) != NULL);
    };
    return numArgs;
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
//    command is comment '#'
    if (strcmp(command, comment) == 0) {
        // eat comments
    }
//    command is '$$'
    else if (strcmp(command, expand) == 0) {
        printf("variable expansion");
    }
//    command is 'cd'
    else if (strcmp(command, moveDir) == 0) {
        printf("cd command");
    }
//    command is 'status
    else if (strcmp(command, status) == 0) {
        printf("status command");
    }
//    command is 'exit'
    else if (strcmp(command, exitSmallsh) == 0) {
        printf("exit command");
        exitFlag = 1;
    }
    else {
        printf("Command not recognized");
    };
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
//    write "Foreground only mode on"
//    fg_only = true
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
//    write "Foreground only mode off"
//    fg_only = false
}
