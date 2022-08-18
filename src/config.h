#ifndef __CONFIG_H__
#define __CONFIG_H__

/*code seg and data seg descriptor offset in gdt*/
#define KERNEL_CODE_SEG  0x08
#define KERNEL_DATA_SEG  0x10

/*total no of interrupts*/
#define TOTAL_INTERRUPT_DESCRIPTORS 512

/*heap size*/ /*100MB = 100*1024*1024 bytes*/
#define HEAP_SIZE_BYTES     104857600
/*Size of 1 Block of Mem Allocated*/
#define HEAP_BLOCK_SIZE     4096

/*https://wiki.osdev.org/Memory_Map_(x86)*/
/*heap starting address in x86 memory map*/
#define HEAP_ADDRESS        0x01000000
/*heap table storage address*/
#define HEAP_TABLE_ADDRESS  0x00007E00

#endif