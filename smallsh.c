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
#define ARGS_MAX_SIZE 	512
#define COMMAND_MAX_SIZE 	2048

//chars to compare to for commands
char bkg = '&';
char comment = '#';
char* expand = "$$";
char* moveDir = "exit";
char* status = "status";
char* exitSmallsh = "exit";
//    exit flag
int exitFlag = 0;
//    args
char rawArgs[COMMAND_MAX_SIZE];
int argsCount = 0;
//    commands
char* cmd = NULL;
int cmdSize = 0;

//signal handler. also handles fgModeOn or fgModeOff
void catchSigtstp();
//function to parse commands
int parseInput(char* args);
//function to execute commands
void makeCmds();
//function to turn foreground only on
void fgModeOn(int);
//function to turn foreground only off
void fgModeOff(int);
//function to move dirs
void cdCmd(const int*);
//print status function
void statusCmd(const int*);
//exit smallsh
void exitCmd();

int main(int argc, char *argv[]) {
//get pid of shell
    pid_t shellPid = getpid();
//list of args
    char* parsedArgs[ARGS_MAX_SIZE];


    printf("%d", shellPid);
//while no exit flag argsCount is num returned from parseInput
//argsList init to NULL
    while (!exitFlag) {
        argsCount = parseInput(rawArgs);
        parsedArgs[argsCount] = NULL;
        exitFlag = 1;
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
int parseInput(char* rawArgs) {
    return printf("hello parse input");
};

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
