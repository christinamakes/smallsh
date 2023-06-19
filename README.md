# What is this?

A simple Linux shell that runs command line instructions, developed in CLion.
- Support for the redirection of standard input and standard output.
- Support for three built in commands: exit, cd, and status. (Status prints the exit or termination status of the most recent background task.)
- Support for comments beginning with the # character.
- Support for expansion of the variable "$$" to the current process ID.
- Support for running tasks in foreground or background using signal handlers to prevent SIGINT from terminating the shell or any background processes, instead only terminating the foreground task. 
- Support for SIGSTP signal toggle to foreground only mode, which forces all commands to run in foreground. 

# Compile Instructions

- gcc -std=c99 smallsh.c -o smallsh
