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
char* args[ARGS_MAX_SIZE];
size_t argsCount = 0;
//    commands
char* cmd = NULL;
size_t cmdSize = 0;

//function to parse commands
void parseInput();
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
//    get pid of shell
    pid_t shellPid = getpid();



    printf("%d", shellPid);
    while (!exitFlag) {
        printf(": ");
        fflush(stdout);
        exitFlag = 1;
    };
    char str[] = "This is my string";
    char* piece = strtok(str, " ");
    while (piece != NULL) {
        printf("%s\n", piece);
        piece = strtok(NULL, " ");
    };
    return 0;
}

void catchSigtstp(int sig) {
    printf("hello catchSigtstp");
};

void parseInput(char** command, char* args[], char inPath[], char outPath[], size_t* argsCount, const size_t* argsMaxSize, const pid_t* shellPid) {
    printf("hello parse input");
};

void fgmModeOn(int sig) {
//    register fgmode_off for SIGTSTP
//    write "Foreground only mode on"
//    fg_only = true
}

void fgModeOff(int sig) {
//    register fgmode_on() for SIGTSTP
//    write "Foreground only mode off"
//    fg_only = false
}
