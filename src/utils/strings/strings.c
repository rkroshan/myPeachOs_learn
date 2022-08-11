#include "strings.h"

/*
returns length of the string
*/
size_t strlen(char* str)
{
    size_t count=0;
    while(*str != '\0'){
        count++;
    }
    return count;
}