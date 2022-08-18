#include "kernel.h"
#include "display/vga/vga.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"

extern void raise_int_zero(void);

static void kernel_init(){
    /*initialize the terminal*/
    terminal_initialize();

    /*Initialize the heap*/
    kheap_init();

    /*initialize the interrupt descriptor table*/
    idt_init();
}

void kernel_main()
{
    kernel_init();
    terminal_print("This is roshanOs........\nInitializing Please wait\nchecking..\n");
    // raise_int_zero();
    // outb(0x60, 0xff); //checking if outb works writing 0xff to 0x60 port
    // terminal_print("Able to write after interrupt raise\n");
    void *ptr1, *ptr2, *ptr3;
    ptr1 = kmalloc(200);
    ptr2 = kmalloc(8196);
    kfree(ptr1);
    ptr3 = kmalloc(400);
    if(ptr1 || ptr2 || ptr3){
        
    }
}