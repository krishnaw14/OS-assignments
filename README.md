# OS-assignments
Assignments done as part of Operating Systems Course (CS 347). Complete Problem Statement of each assignment is present in the respective folders.

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

The goal of this assignment was to build a custom memory manager to allocate memory dynamically in a program. Specifically, I implemented functions to allocate and free memory, that act as replacements for C
library functions like malloc and free. 

Following 4 functions were implemented:

- `init()`: Intialized the memory manager and allocated a 4KB page from OS via mmap system call. 
- `cleanup()`: cleaned up state of the manager, and returned the memory mapped page back to the OS. 
- `alloc(int)`: returns a char* pointer to a buffer within the page of size passed as argument. Only allocations of 8 bytes are considered.
- `alloc(char*)`:  takes a pointer to a previously allocated memory chunk, and frees up the entire chunk.

## Assignment 3: Worker Thread Pool

The goal of this assignment was to implement a master and worker pool using multithreaded programming. A master thread produces numbers which are consumerd by the workers while ensuring synchronisation and mutual exclusion between the threads.

Two scripts were developed: `master-worker-array.c` and `master-worker-ll.c` which use fixed size array and dynamically updating linked list respectively in order to store the numbers in a shared buffer which is accessed by the master and worker threads. 
