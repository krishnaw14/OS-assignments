#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#define PAGESIZE 4096 //size of memory to allocate from OS
#define MINALLOC 8 //allocations will be 8 bytes or multiples of it

// You can declare any data structures required here

//Declaring some global variables
int total_memory_available;
int total_allocations;

char* free_address_list[512];
int free_memory_list[512];

char* occupied_address_list[512];
int occupied_memory_list[512];

char* page_memory_address;

// function declarations
int init();
int cleanup();
char *alloc(int);
void dealloc(char *);
