section .asm

global enable_interrupts
global disable_interrupts
global idt_load ; so that we can refernce it globally in kernel
global pic_init
global int21h
global no_interrupt

extern int21h_handler
extern no_interrupt_handler

enable_interrupts: ; function to enable interrupts
    sti ; sti inst : enable the interrupts
    ret

disable_interrupts: ; function to disable interrupts
    cli ; cli inst : disable the interrupts
    ret

idt_load:
    push ebp ; push the base pointer to stack
    mov ebp, esp ; mov the current stack pointer to base pointer as it points to current function call

    mov ebx, [ebp+8] ; mov first argument addr to ebx
    ; ebp+4 gives the return addr of caller function
    lidt [ebx] ; load the interrupt descriptor table addr to idtr register
    
    pop ebp ; pop the base pointer to stack
    ret ; return from subroutine

pic_init: ; programmable interrupt controller initialization
.remap_master_pic:
    mov al, 00010001b ; https://wiki.osdev.org/PIC#Programming_the_PIC_chips
    out 0x20, al ; Tell master that initialization required, 0x20 is master command port

    mov al, 0x20 ; Interrupt 0x20 is where master ISR 0 should start
    out 0x21, al ; 0x21 is master PIC data port

    mov al, 00000001b 
    out 0x21, al ; Tell master to go in x86 mode
    ret
    ;End of remap of master pic

no_interrupt: ; handler to be assigned to uninitialized interrupts
    cli ; clear the interrupts
    pushad ; pushad inst : Push EAX, ECX, EDX, EBX, original ESP, EBP, ESI, and EDI
    call no_interrupt_handler ; call no_interrupt_handler
    popad ; popad inst
    sti ; enable the interrupts
    iret ; return from subroutine

int21h: ; handler to be assigend to interrupt 0x21 handler = keyboard interrupt
    cli ; clear the interrupts
    pushad ; pushad inst : Push EAX, ECX, EDX, EBX, original ESP, EBP, ESI, and EDI
    call int21h_handler ; call int21h_handler
    popad ; popad inst
    sti ; enable the interrupts
    iret ; return from subroutine