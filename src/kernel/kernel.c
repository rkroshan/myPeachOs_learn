#include "kernel.h"
#include "display/vga/vga.h"

static void kernel_init(){
    terminal_initialize();
}

void kernel_main()
{
    kernel_init();
    terminal_print("This is roshanOs........\nInitializing Please wait\nchecking..\n");
}