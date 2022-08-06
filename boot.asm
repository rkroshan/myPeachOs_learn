; Program to print a character on screen from bootloader

ORG 0x7c00 ; changing origin address to 0x7c00
BITS 16 ; 16 bit addressing

start:
    mov ah, 0eh ; http://www.ctyme.com/intr/rb-0106.htm "Ralphs brown interrupts"
    mov al, 'R' ; going to print this character
    mov bx, 0
    
    int 0x10 ; interrupt to print output on screen

    jmp $ ; infinite jmp to current address, so it stops here

times 510-($ - $$) db 0 ; need to make bootloader of 512 bytes so filling remaining bytes to 0
dw 0xAA55 ; 0x55AA is the boot signature which bios looks for in a boot loader before loading it dw flips the bytes and needed in little endian