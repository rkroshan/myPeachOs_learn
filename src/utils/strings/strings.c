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

/*
Return String length
*/
size_t strnlen(const char* str, int len)
{
    int count=0;
    while((*str != '\0') && (count <= len)){
        count++;
        str++;
    }
    return count;
}

/*
Return whether given ascii val is a digit ?
*/
int isdigit(char c)
{
    return ((c >= 48) && (c <= 57));
}

/*
Return ascii to digit val of the given char
*/
int char_to_digit(char c)
{
    return (c - 48);
}

/*
copy string from dest to source
Note: it is not full proof no checking similar to memmove
*/
char* strcpy(char* dst, const char* src)
{
    char* res = dst;
    while(*src != '\0')
    {
        *dst = *src;
        src +=1;
        dst +=1;
    }
    *dst = 0;
    return res;
}

/*
Return whether both strings given are equal
*/
int memcmp(void* str1, void* str2, int len)
{
    char* s1 = (char*)str1;
    char* s2 = (char*)str2;

    while(len--){
        if(*s1 != *s2){
            return (*s1-*s2);
        }
        s1++;s2++;
    }
    return 0;
}