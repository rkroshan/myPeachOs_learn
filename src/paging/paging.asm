[BITS 32]

section .asm

global paging_load_directory ; load pdg in cr3 register
global enable_paging ; enable paging

paging_load_directory:
    push ebp            ; push the base pointer to stack
    mov ebp, esp        ; mov the current stack pointer to base pointer as it points to current function call
    mov eax, [ebp+8]    ; get the pgd addr given as first argument
    mov cr3, eax        ; move pgd addr to cr3 reg
    pop ebp             ; pop the base pointer to stack
    ret                 ; return from subroutine

enable_paging:
    push ebp            ; push the base pointer to stack
    mov ebp, esp        ; mov the current stack pointer to base pointer as it points to current function call
    mov eax, cr0        ; get cr0 current reg value in eax
    or eax, 0x80000000  ; enable paging by setting highest bit of cr0
    mov cr0, eax        ; set the new cr0 val
    pop ebp             ; pop the base pointer to stack
    ret                 ; return from subroutine