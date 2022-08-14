#include "kernel.h"

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

//static variables for the VGA monitor
static uint16_t* const video_mem = (uint16_t*)VGA_MEM_ADDR_START;
static int terminal_col=0;
static int terminal_row=0;
static uint8_t fg_color = VGA_GREEN; /*personal favourite*/
static uint8_t bg_color = VGA_BLACK; /*personal favourite*/

//inline function
static inline uint16_t terminal_make_char(char c, uint8_t color){
    return (uint16_t)((color << 8) | c);
}

/*
Set Foreground color of VGA Monitor
*/
void terminal_set_fg_color(VGA_COLOR color){ fg_color = color; }
/*
Set Background color of VGA Monitor
*/
void terminal_set_bg_color(VGA_COLOR color){ bg_color = bg_color; }
/*
Write one character on the terminal at a coordinate
*/
static void terminal_writechar(int x, int y, char c)
{
    switch(c){
        case '\n' :
            terminal_col=0; terminal_row+=1;
            return;
        default:
            video_mem[(y*VGA_WIDTH)+x] = terminal_make_char(c,fg_color);
            terminal_col++;
            break;
    }

    //check for overflow
    if(terminal_col >= VGA_WIDTH){ 
        //move to the next line
        terminal_row+=1; terminal_col=0;
    }

    if(terminal_row >= VGA_HEIGHT){
        //move to the top line
        terminal_row=0; terminal_col=0;
    }
}

/*
Write one character on the terminal
*/
void terminal_putchar(char c){
    terminal_writechar(terminal_col, terminal_row, c);
}

/*
Write Complete message on the terminal
*/
void terminal_print(char* msg)
{
    size_t len = strlen(msg);
    for(size_t i=0;i<len;i++){
        terminal_putchar(msg[i]);
    }
}

/*
Clear the complete terminal buffer of size VGA_HEIGHT*VGA_WIDTH
*/
void terminal_clear()
{
    for(int y=0; y<VGA_HEIGHT;y++){
        for(int x=0; x<VGA_WIDTH;x++){
            video_mem[(y*VGA_WIDTH)+x] = terminal_make_char(' ',bg_color);
        }
    }
}
/*
Terminal Initialization function, calls during kernel init
*/
void terminal_initialize(){
    terminal_clear(); //clear the terminal
}


void kernel_init(){
    terminal_initialize();
}

void kernel_main()
{
    kernel_init();
    terminal_print("This is roshanOs........\nInitializing Please wait\nchecking..\n");
}