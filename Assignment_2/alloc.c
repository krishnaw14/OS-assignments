#include "alloc.h"

/* Code to allocate page of 4KB size with mmap() call and
* initialization of other necessary data structures.
* return 0 on success and 1 on failure (e.g if mmap() fails)
*/
int init()
{
	// Write your code below

  
}

/* optional cleanup with munmap() call
* return 0 on success and 1 on failure (if munmap() fails)
*/
int cleanup()
{

	// Write your code below

  
}

/* Function to allocate memory of given size
* argument: bufSize - size of the buffer
* return value: on success - returns pointer to starting address of allocated memory
*               on failure (not able to allocate) - returns NULL
*/
char *alloc(int bufSize)
{
	// write your code below

  

}


/* Function to free the memory
* argument: takes the starting address of an allocated buffer
*/
void dealloc(char *memAddr)
{
	// write your code below

}
