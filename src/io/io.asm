section .asm

global insb ; unsigned char insb(unsigned short port);
global insw ; unsigned short insw(unsigned short port);
global outb ; void outb(unsigned short port, unsigned char val);
global outw ; void outw(unsigned short port, unsigned short val);

insb:
    push ebp ; Backup the current base pointer
    mov ebp, esp ; load the current function addr on stack to ebp

    xor eax, eax ; clear the eax reg
    mov edx, [ebp+8] ; get the first argument from the function, which is the port number
    in al, dx ; in x86 instruction: get a byte from port addr present in dx to al

    pop ebp ; restore the base pointer val
    ret

insw:
    push ebp ; Backup the current base pointer
    mov ebp, esp ; load the current function addr on stack to ebp

    xor eax, eax ; clear the eax reg
    mov edx, [ebp+8] ; get the first argument from the function, which is the port number
    in ax, dx ; in x86 instruction: get a word from port addr present in dx to al

    pop ebp ; restore the base pointer val
    ret

outb:
    push ebp ; Backup the current base pointer
    mov ebp, esp ; load the current function addr on stack to ebp

    mov edx, [ebp+8] ; get the first argument from the function, which is the port number
    mov eax, [ebp+12] ; get the second argument from function, which is the byte to send
    out dx, al ;  x86 instruction: send the byte present in al to port addr in dx

    pop ebp ; restore the base pointer val
    ret

outw:
    push ebp ; Backup the current base pointer
    mov ebp, esp ; load the current function addr on stack to ebp

    mov edx, [ebp+8] ; get the first argument from the function, which is the port number
    mov eax, [ebp+12] ; get the second argument from function, which is the byte to send
    out dx, ax ;  x86 instruction: send the word present in al to port addr in dx

    pop ebp ; restore the base pointer val
    ret