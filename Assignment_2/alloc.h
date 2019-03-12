#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#define PAGESIZE 4096 //size of memory to allocate from OS
#define MINALLOC 8 //allocations will be 8 bytes or multiples of it

// You can declare any data structures required here

//Declaring some global variables
int total_memory_occupied;
int number_of_allocations;
char* address_array[512];
size_t memory_size_array[512];

char* page_memory;


// function declarations
int init();
int cleanup();
char *alloc(int);
void dealloc(char *);
