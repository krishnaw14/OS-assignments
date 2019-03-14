#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	double* address;
	double* address_1;
	double* address_2;
	address = (double*)mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, -1, 0);
	address_1 = (double*)(address +1);
	// address_2 = (double*)(address +8);

	// strcpy(address, "Hello, world!");
	// address = "Hello world!";

	printf("address = %p \n", address);
	printf("address_1 = %p \n", address_1);
	printf("address_2 = %p \n", address_2);

	printf("Size = %lu \n", sizeof(double*));

	return 0;
}