#ifndef __idt_h__
#define __idt_h__

//includes
#include "stdint.h"
#include "stddef.h"

//structure definitions
/*idt decriptor structure*/
struct idt_desc{
    uint16_t offset_1; // offset bits 0-15
    uint16_t segment_selector; //Selector that is in our GDT, code or data selector descriptor
    uint8_t zero; //Reserved must be set to zero
    uint8_t type_attr; // Descriptor type and attributes
    uint16_t offset_2; //offset bits 16-31 
}__attribute__((packed));

/*idtr registor definition structure*/
/*idt_desc remains in contigous array and idtr points to starting desc structure address*/
struct idtr{
    uint16_t limit; // size of descriptor table -1
    uint32_t base; //Base address of the start of idt
}__attribute__((packed));

//Function Prototype
/*Initialize IDT*/
void idt_init();
/*enable interrupts*/
void enable_interrupts();
/*disable interrupts*/
void disable_interrupts();

#endif