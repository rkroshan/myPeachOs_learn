#ifndef __VGA_H__
#define __VGA_H__

//include
#include <stddef.h>
#include <stdint.h>

//MACROS
#define VGA_HEIGHT  20
#define VGA_WIDTH   80
#define VGA_MEM_ADDR_START  0xB8000

//colors
typedef enum {
VGA_BLACK=0,
VGA_BLUE,
VGA_GREEN,
VGA_CYAN,
VGA_RED,
VGA_PURPLE,
VGA_BROWN,
VGA_GREY,
VGA_DARK_GREY,
VGA_LIGHT_BLUE,
VGA_LIGHT_GREEN,
VGA_LIGHT_CYAN,
VGA_LIGHT_RED,
VGA_LIGHT_PURPLE,
VGA_YELLOW,
VGA_WHITE,
VGA_TOTAL_COLORS
}VGA_COLOR;

//Function Prototypes
/*
Set Foreground color of VGA Monitor
*/
void terminal_set_fg_color(VGA_COLOR color);
/*
Set Background color of VGA Monitor
*/
void terminal_set_bg_color(VGA_COLOR color);
/*
Write one character on the terminal
*/
void terminal_putchar(char c);
/*
Write Complete message on the terminal
*/
void terminal_print(char* msg);
/*
Clear the complete terminal buffer of size VGA_HEIGHT*VGA_WIDTH
*/
void terminal_clear();
/*
Terminal Initialization function, calls during kernel init
*/
void terminal_initialize();

#endif