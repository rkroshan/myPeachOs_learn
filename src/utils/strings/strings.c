#include "strings.h"

/*
returns length of the string
*/
size_t strlen(char* str)
{
    int count=0;
    while(*str != '\0'){
        count++;
        str++;
    }
    return count;
}