[BITS 32]

; section .kernel.asm ; seperating kernel asm section, commented as were not able to get debugging symbol _start

global _start
global raise_int_zero
CODE_SEG equ 0x08
DATA_SEG equ 0x10

;kernel main function defined in C
extern kernel_main

_start: ; enter into 32 bit protected mode code segment
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp
    
.enable_A20_line: ;https://wiki.osdev.org/A20_Line , necessary because without it, we will only be able to access 20 bits (1MB) address space
    in al, 0x92
    or al, 2
    out 0x92, al
    
.call_kernel_main:
    call kernel_main
    jmp $

raise_int_zero: ; int 0 is divide be zero error, using it just for checking
    int 0 ; calling interrupt zero, checking if properly setup
    ret


times 512-($ - $$) db 0 ; it will help to make kernel size upto 512 bytes and prevemt any aligment issues in final binary for other executables since 512%16=0