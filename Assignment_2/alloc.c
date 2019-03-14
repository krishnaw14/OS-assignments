#include "alloc.h"

/* Code to allocate page of 4KB size with mmap() call and
* initialization of other necessary data structures.
* return 0 on success and 1 on failure (e.g if mmap() fails)
*/
int init()
{
	// Write your code below
	int i;

	page_memory_address = (char*)mmap(NULL, PAGESIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);

	for (i = 0; i< 512; i++)
	{
		free_address_list[i] = NULL;
		free_memory_list[i] = 0;

		occupied_address_list[i] = NULL;
		occupied_memory_list[i] = 0;
	}
	free_address_list[0] = page_memory_address;
	free_memory_list[0] = 4096;

	total_memory_available = 4096;
	total_allocations = 0;

	return 0;
  
}

/* optional cleanup with munmap() call
* return 0 on success and 1 on failure (if munmap() fails)
*/
int cleanup()
{

	// Write your code below
	int i;
	for (i = 0; i< 512; i++)
	{
		free_address_list[i] = NULL;
		free_memory_list[i] = 0;

		occupied_address_list[i] = NULL;
		occupied_memory_list[i] = 0;
	}
	total_memory_available = 4096;
	total_allocations = 0;

	int flag = munmap(page_memory_address, PAGESIZE);
	// if (flag<0)
	// 	printf("Error in cleanup!\n");
	return flag;


}

/* Function to allocate memory of given size
* argument: bufSize - size of the buffer
* return value: on success - returns pointer to starting address of allocated memory
*               on failure (not able to allocate) - returns NULL
*/
char *alloc(int bufSize)
{
	// write your code below
	if (bufSize % MINALLOC != 0 || total_memory_available <= 8)
		return NULL;
  	
  	int i;
  	char* return_address;

  	// Get return address, update total available memory and update free list
  	for(i = 0; i < 512; i++)
  	{
  		if (free_memory_list[i] >= bufSize)
  		{
  			return_address = free_address_list[i];
  			total_memory_available = total_memory_available - bufSize;
  			total_allocations = total_allocations + 1;

  			free_address_list[i] = free_address_list[i]+bufSize;
  			free_memory_list[i] = free_memory_list[i] - bufSize;
  			break;
  		}
  	}

  	//Update Occupied List
  	for (i = 0; i<512; i++)
  	{
  		if (occupied_address_list[i] == NULL)
  		{
  			occupied_address_list[i] = return_address;
  			occupied_memory_list[i] = bufSize;
  			break;
  		}
  	}
  	
  	return return_address;

}

/* Function to free the memory
* argument: takes the starting address of an allocated buffer
*/

void dealloc(char *memAddr)
{
	// write your code below

	// Get length from the data structure defined
	int i, length;
	for (i = 0; i<512; i++)
	{
		if (memAddr == occupied_address_list[i])
		{
			length = occupied_memory_list[i];
			break;
		}
	}

	// Update Free list
	for(i = 0; i< 512; i++)
	{
		if (free_address_list[i] == NULL)
		{
			free_address_list[i] = memAddr;
			free_memory_list[i] = length;
			break;
		}
	}

	// Update Occupied List
	for(i = 0; i< 512; i++)
	{
		if (occupied_address_list[i] == memAddr)
		{
			occupied_address_list[i] = NULL;
			occupied_memory_list[i] = 0;
			break;
		}
	}

	total_memory_available = total_memory_available + length;
	total_allocations = total_allocations - 1;

}
