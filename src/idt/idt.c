#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "display/vga/vga.h"

/*creating total no of descriptors*/
struct idt_desc descriptors[TOTAL_INTERRUPT_DESCRIPTORS];
struct idtr idtr_descriptor;

/*asm function to load idtr struct addr to idtr reg, defined in idt.asm*/
extern void idt_load(struct idtr* ptr);

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

    /*setting interrupt*/
    /*int 0*/
    idt_set(0,idt_zero);

    /*load interrupt descriptor table*/
    idt_load(&idtr_descriptor);
}