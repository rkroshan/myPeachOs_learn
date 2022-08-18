#ifndef __HEAP_H__
#define __HEAP_H__

//includes
#include "config.h"
#include <stddef.h>
#include <stdint.h>

/*
Fixed Paritition Heap Algorithm for contiguous mem allocation
 - Whole Heap memory provided will be divided into equal size of provided
 - There will be a table of entries, each entry of 1 byte consist information:
 - Block is taken or not at bit 0 (1: taken, 0: free)
 - Block is has next block allocated bit 7 : (1: has_next, 0: last block)
 - block is the first block in mem allocation at bit 6 (1: first block, 0: not first)
*/

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN    0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE     0x00
#define HEAP_BLOCK_HAS_NEXT             0b10000000
#define HEAP_BLOCK_IS_FIRST             0b01000000

/*typedef will be used inspite of unsigned char*/
typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

/*this will consist of actual heap entries corresponding to each block of heap memory*/
struct heap_table{
    HEAP_BLOCK_TABLE_ENTRY* entries; //pointer pointing to the array of entries
    size_t total_entries;
};

/*this struct will contain heap entries and starting addr of heap corresponding to heap memory*/
struct heap{
    struct heap_table* table;
    void* saddr;
};

//Function Prototype
/*
heap_create : will create heap structure filled with heap entries as FREE blocks and setup the starting addres and size of heap
*/
int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table);
/*
heap_ malloc : will allocate memory of size nearest to HEAP_BLOCK_SIZE multiple
*/
void* heap_malloc(struct heap* heap, size_t size);
/*
heap_free : will free the allocated memory
*/
void heap_free(struct heap* heap, void* ptr);

#endif