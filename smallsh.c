/* Author: Christina Moore
 * Assignment: Smallsh
 * Class: OSU CS 344 - Operating Systems
 * Due: Nov. 14th 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char str[] = "This is my string";
    char* piece = strtok(str, " ");
    while (piece != NULL) {
        printf("%s\n", piece);
        piece = strtok(NULL, " ");
    };
    return 0;
}
