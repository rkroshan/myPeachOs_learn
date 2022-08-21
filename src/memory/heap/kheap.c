#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "display/vga/vga.h"
#include "memory/memory.h"

/*kernel heap structures*/
struct heap kernel_heap;
struct heap_table kernel_heap_table;

/*initialize kernel heap structure*/
void kheap_init(){
    // get the total table entries size
    size_t total_table_entries = (HEAP_SIZE_BYTES / HEAP_BLOCK_SIZE);
    //set the table entries pointer where we want to store it in memory
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)(HEAP_TABLE_ADDRESS);
    //set total table entries
    kernel_heap_table.total_entries = total_table_entries;

    //Heap start and end address
    void* end = (void*)(HEAP_ADDRESS + HEAP_SIZE_BYTES);
    void* start = (void*)HEAP_ADDRESS;

    int result = heap_create(&kernel_heap,start,end,&kernel_heap_table);

    if(result < 0){
        terminal_print("Failed to create heap\n");
    }
}

/*kmalloc: heap memory allocation method*/
void* kmalloc(size_t size)
{
    return heap_malloc(&kernel_heap,size);
}

/*kzalloc: heap memory allocation method and clear the memory*/
void* kzalloc(size_t size)
{
    void* ptr = kmalloc(size);
    /*check if memory got allocated*/
    if(ptr==NULL){
        return NULL;
    }
    /*clear the memory*/
    memset(ptr,0x00,size);
    return ptr;
}

/*free : heap memory free method*/
void kfree(void* ptr)
{
    return heap_free(&kernel_heap,ptr);
}