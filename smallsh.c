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

/* Foreground flag */
int isForeground = 0;

//catch sigtstp
void catchSigtstp(int);
//function to parse commands
void parseCmd(char**, char*[], char[], char[], size_t*, const size_t*, const pid_t*);
//function to execute commands
void executeCmd(char*[], char[], char[], struct sigaction, const size_t*, pid_t*, int*, int);
//function to move dirs
void moveDir(char*[], const size_t*);
//print status function
void printStatus(const int*);
//exit smallsh
void exitSmallsh(char**, pid_t*);

int main(int argc, char *argv[]) {
//    get pid of shell
    pid_t shellPid = getpid();
//    max arg size
    size_t argsMaxSize = 512;
//    max command size
    size_t commandMaxSize = 2049;
//    chars to compare to for commands
    char bkg = '&';
    char comment = '#';
    char* expand = "$$";
    char* moveDir = "exit";
    char* status = "status";
    char* exitSmallsh = "exit";
//    exit flag
    int exitFlag = 0;

//    args
    char* args[argsMaxSize];
    size_t argsCount = 0;
//    commands
    char* cmd = NULL;
    size_t cmdSize = 0;


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
