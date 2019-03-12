#include "alloc.h"

/* Code to allocate page of 4KB size with mmap() call and
* initialization of other necessary data structures.
* return 0 on success and 1 on failure (e.g if mmap() fails)
*/
int init()
{
	// Write your code below
	int i;

	page_memory = (char*)mmap(NULL, PAGESIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);

	// Intialize memory manager

	// total_memory_occupied = 0;
	// number_of_allocations = 0;
	// for(i = 0; i<512; i++)
	// {
	// 	address_array[i] = NULL;
	// 	memory_size_array[i] = 0;
	// }


  
}

/* optional cleanup with munmap() call
* return 0 on success and 1 on failure (if munmap() fails)
*/
int cleanup()
{

	// Write your code below

	int flag = munmap(page_memory, PAGESIZE);
	if (flag<0)
		printf("Error in cleanup!\n");

	// Reinitialize memory manager

}

/* Function to allocate memory of given size
* argument: bufSize - size of the buffer
* return value: on success - returns pointer to starting address of allocated memory
*               on failure (not able to allocate) - returns NULL
*/
char *alloc(int bufSize)
{
	// write your code below
	if (bufSize % MINALLOC != 0)
		return NULL;
  
  	

}


/* Function to free the memory
* argument: takes the starting address of an allocated buffer
*/
void dealloc(char *memAddr)
{
	// write your code below
	size_t length;

	// Get length from the data structure defined
	int flag = munmap(memAddr, length);

	if (flag < 0)
	{
		printf("Error in Deallocation!\n");
	}

}
