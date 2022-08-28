#ifndef __STRINGS_H__
#define __STRINGS_H__
//include
#include <stddef.h>
#include <stdint.h>

//Function Prototype
/*
Return String length
*/
size_t strlen(char* str);
/*
Return String length
*/
size_t strnlen(const char* str, int len);
/*
Return whether given ascii val is a digit ?
*/
int isdigit(char c);
/*
Return ascii to digit val of the given char
*/
int char_to_digit(char c);
/*
Return whether both strings given are equal
*/
int memcmp(void* str1, void* str2, int len);
#endif

