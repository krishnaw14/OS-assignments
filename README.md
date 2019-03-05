# OS-assignments
Assignments done as part of Operating Systems Course (CS 347)

## Assignment 1: Shell Implementation

The goal of this assignment was to develop a simple shell to execute user commands, much like the bash shell in
Linux in order to deepen the understanding of various concepts of process management in Linux.

The shell had the following features:

- Execution of simple commands like ls, pwd, echo, cat, ps and other built in commands
- Execution of cd
- Execution of commands in background if a command are followed by &
- Executions of commands in serial in foreground if commands are separated by &&
- Executions of commands in parallel in foreground if commands are separated by &&&
- Signal handling for sigint signal to kill the executing foreground processes but not the background processes
- exit command implemented to kill the background processes and exit the shell

Cases involving a combination of &, && and &&& are not considered.

## Assignment 2: Dynamic memory management