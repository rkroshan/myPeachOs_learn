#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "display/vga/vga.h"
#include "io/io.h"

/*creating total no of descriptors*/
struct idt_desc descriptors[TOTAL_INTERRUPT_DESCRIPTORS];
struct idtr idtr_descriptor;

/*asm function to load idtr struct addr to idtr reg, defined in idt.asm*/
extern void idt_load(struct idtr* ptr);
/*asm function to initialize PIC*/
extern void pic_init(void);
/*asm function handler for 0x21 interrupt*/
extern void int21h(void);
/*asm function handler for default interrupt handler*/
extern void no_interrupt(void);

void no_interrupt_handler(){
    outb(0x20, 0x20); //acknowledge the interrupt to PIC
}

void int21h_handler(){
    terminal_print("Keyboard Pressed\n");
    outb(0x20, 0x20); //acknowledge the interrupt to PIC
}

void idt_zero(){
    terminal_print("Error : Divide by zero\n");
}

void idt_set(int interupt_no, void* address){
    //get the descriptors address
    struct idt_desc *desc = &descriptors[interupt_no];
    desc->offset_1 = (uint32_t)address & 0x0000ffff;
    desc->segment_selector = KERNEL_CODE_SEG;
    desc->zero = 0;
    desc->type_attr = 0xEE; //pri. lvl = 3, gate_tp: 32bit interrupt, Present = 1
    desc->offset_2 = (uint32_t)address >> 16 ;
}

void idt_init(){
    /*clearing the descriptor table memory*/
    memset(descriptors,0, sizeof(descriptors));

    /*setup idtr*/
    idtr_descriptor.base = (uint32_t)descriptors;
    idtr_descriptor.limit = sizeof(descriptors) - 1;

    /*setting default interrupts*/
    for(int i=0; i<TOTAL_INTERRUPT_DESCRIPTORS; i++){
        idt_set(i, no_interrupt);
    }
    /*setting specific interrupt*/
    /*int 0*/
    idt_set(0,idt_zero);
    idt_set(0x21,int21h);

    /*load interrupt descriptor table*/
    idt_load(&idtr_descriptor);

    /*Initialize the PIC*/
    pic_init();

}