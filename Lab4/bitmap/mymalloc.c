#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "mymalloc.h"

struct PointerInfo {
	void *ptr;
	size_t size;
};

char _heap[MEMSIZE] = {0};
unsigned char map[MEMSIZE/8] = {0};
struct PointerInfo pointers[MEMSIZE];

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

void print_memlist() {
	print_map(&map[0], 8);
    // Implement this to call print_map from bitmap.c
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
	long index = search_map(&map[0], 8, size);
	if (index == -1) {
    	return NULL;
	} 
	allocate_map(&map[0], index, size);
	struct PointerInfo pointer_info = { &_heap[index], size };
	pointers[index] = pointer_info;
	return &_heap[index];
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
	long index = get_index(ptr);
	size_t size = pointers[index].size;
	free_map(&map[0], index, size);
}

