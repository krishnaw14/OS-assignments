# commands to compile and run custom memory allocator

# for test case 1
gcc -c test_alloc1.c
gcc -c alloc.c
gcc test_alloc1.o alloc.o -o alloc
./alloc

# for test case 2
#gcc -c test_alloc2.c
#gcc -c alloc.c
#gcc test_alloc2.o alloc.o -o alloc
#./alloc

rm *.o alloc
