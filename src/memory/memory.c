#include "memory.h"

/*
Memset utility function
*/
void* memset(void * ptr, int c, size_t size)
{
    char* cptr = (char*)ptr;
    for(size_t i=0; i<size; i++){
        cptr[i] = (char)c; 
    }
    return ptr;
}