#ifndef __KHEAP_H__
#define __KHEAP_H__

#include <stddef.h>

/*initialize kernel heap structure*/
void kheap_init();
/*kmalloc: heap memory allocation method*/
void* kmalloc(size_t size);
/*free : heap memory free method*/
void kfree(void* ptr);

#endif