; Program to print message from disk access via bios intr chs algorithm on qemu and real hdwr

ORG 0 ; changing origin address to 0
BITS 16 ; 16 bit addressing

;considering Bios parameter block information structure needs to be here for BIOS to put some information
_start: ; https://wiki.osdev.org/FAT
    jmp short start
    nop

times 33 db 0 ; size of BIOS Param block except the jmp short instruction, this will be use by some Bios to fill some information 

func_disk_read_msg: ; chs methos to read disk
    mov ah, 0x02 ; 'Raplh Brown interrupts from Bios DISK - READ SECTOR(S) INTO MEMORY'
    mov al, 0x01 ; number of sectors to read = 1
    mov ch, 0x00 ; low eight bit of cylinder number
    mov cl, 0x02 ; sector numer to read 1-63 = 2
    mov dh, 0x00 ; head number = 0
    ; dl = drive number is already set by bios in the structure reserved
    mov bx,buffer ; buffer address where data from disk will reside

    int 0x13 ; invoke the interrupt
    ret 

;actual start of code
start:
    jmp 0x7c0:.define_segments_addr ; jump to 0x7c0+ addr(define_segments_addr) , since code segment starts at 0x7c0
.define_segments_addr: ; it is necessary to diable interrupts when changing segment addrs
    cli ; disable/clear interrupts
    mov ax,0x7c0 ; addr from where bootloader loads
    mov ds,ax ; set data segment addr to 0x7c00
    mov es,ax ; set extra segment addr to 0x7c00
    mov ax,0x00 
    mov ss,ax ; set stack segment addr to 0
    mov sp,0x7c00 ; set stack pointer addr to 0x7c00
    sti ; enable interrupts
    
.program:
    call func_disk_read_msg
    jc .print_error_message ; carry will be set if failed to read from disk

.print_buffer_message:
    mov si, buffer ; moving buffer address to source index reg
    call print_message ; call print_message subroutine
    jmp $ ; infinite jmp to current address, so it stops here

.print_error_message:
    mov si, error_message ; set si to error message addr
    call print_message ; subroutine to print string
    jmp $ ; infinite jump


print_message:
    mov bx, 0 
.loop:
    lodsb ; x86 instruction to load value present at [ds:si] reg in al reg and then increment si address
    cmp al, 0 ; check if we are at null character
    je .done ; if null then return
    call print_char ; else call subroutine print_char
    jmp .loop ; jmp to loop label again
.done:
    ret

print_char:
    mov ah, 0eh ; http://www.ctyme.com/intr/rb-0106.htm "Ralphs brown interrupts"
    int 0x10 ; interrupt to print output on screen
    ret

error_message: db 'failed to load message from disk',0 ; null termination by 0
times 510-($ - $$) db 0 ; need to make bootloader of 512 bytes so filling remaining bytes to 0
dw 0xAA55 ; 0x55AA is the boot signature which bios looks for in a boot loader before loading it dw flips the bytes and needed in little endian
buffer: ; empty label just to get a blank address where disk will be stored