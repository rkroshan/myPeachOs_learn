section .asm

global idt_load ; so that we can refernce it globally in kernel

idt_load:
    push ebp ; push the base pointer to stack
    mov ebp, esp ; mov the current stack pointer to base pointer as it points to current function call

    mov ebx, [ebp+8] ; mov first argument addr to ebx
    ; ebp+4 gives the return addr of caller function
    lidt [ebx] ; load the interrupt descriptor table addr to idtr register
    
    pop ebp ; pop the base pointer to stack
    ret ; return from subroutine
