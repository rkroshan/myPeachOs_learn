; Program to load 32bit kernel in protected mode in qemu

ORG 0x7c00 ; changing origin offset address to 0x7c00
BITS 16 ; 16 bit addressing

CODE_SEG equ gdt_code - gdt_start ; Code SD start addr
DATA_SEG equ gdt_data - gdt_start ; Data SD start addr

;considering Bios parameter block information structure needs to be here for BIOS to put some information
; http://www.ntfs.com/fat-partition-sector.htm
; https://wiki.osdev.org/FAT
;BIOS Parameter Block (BPB)
jmp short start
nop
; The reason for this is to jump over the disk format information (the BPB and EBPB). 
;Since the first sector of the disk is loaded into ram at location 0x0000:0x7c00 and executed, 
;without this jump, the processor would attempt to execute data that isn't code

; FAT16 Header
OEMIdentifier           db 'PEACHOS '   ;OEM identifier,  If the string is less than 8 bytes, it is padded with spaces.
BytesPerSector          dw 0x200        ;The number of Bytes per sector
SectorsPerCluster       db 0x80         ;Number of sectors per cluster.
ReservedSectors         dw 200          ;Number of reserved sectors. The boot record sectors are included in this value.
FATCopies               db 0x02         ;Number of File Allocation Tables (FAT's) on the storage media.
RootDirEntries          dw 0x40         ;The total number of file name entries that can be stored in the root folder of the volume.
NumSectors              dw 0x00         ;The total sectors in the logical volume. If this value is 0, it means there are more than 65535 sectors in the volume, and the actual count is stored in the Large Sector Count entry at 0x20
MediaType               db 0xF8         ;represent media type as hard disk
SectorsPerFat           dw 0x100        ;Number of sectors per FAT
SectorsPerTrack         dw 0x20         ;Number of sectors per track
NumberOfHeads           dw 0x40         ;Number of heads or sides on the storage media
HiddenSectors           dd 0x00         ;Number of hidden sectors.
SectorsBig              dd 0x773594     ;Large sector count. This field is set if there are more than 65535 sectors in the volume, resulting in a value which does not fit in the Number of Sectors entry at 0x13.

; Extended BPB (Dos 4.0)
DriveNumber             db 0x80         ;0x80 for hard disks
WinNTBit                db 0x00         ;Flags in Windows NT. Reserved otherwise.
Signature               db 0x29         ;Fat16 Signature
VolumeID                dd 0xD105       ;VolumeID 'Serial' number. Used for tracking volumes between computers. You can ignore this if you want
VolumeIDString          db 'PEACHOS BOO';Volume label string. This field is padded with spaces
SystemIDString          db 'FAT16   '   ;System identifier string. This field is a string representation of the FAT file system type.

; times 33 db 0 ; size of BIOS Param block except the jmp short instruction, this will be use by some Bios to fill some information 

;actual start of code
start:
    jmp 0:.define_segments_addr ; jump to 0x7c0+ addr(define_segments_addr) , since code segment starts at 0x7c0
.define_segments_addr: ; it is necessary to diable interrupts when changing segment addrs
    cli ; disable/clear interrupts
    mov ax,0x00 
    mov ds,ax ; set data segment addr to 0
    mov es,ax ; set extra segment addr to 0
    mov ss,ax ; set stack segment addr to 0
    mov sp,0x7c00 ; set stack pointer addr to 0x7c00
    sti ; enable interrupts
    
.load_protected_mode:   ;https://wiki.osdev.org/Protected_Mode
    cli ; disable the interrupts
    lgdt[gdt_descriptor] ; load the gdt_descriptor
    mov eax, cr0 ; Control Register 0
    or eax, 0x01 ; set PE (Protection Enable) bit in CR0 (Control Register 0)
    mov cr0, eax ; set the cr0 reg
    jmp CODE_SEG:load32 ; CODE_SEG = code segment Descriptor and load32 = code segment starting
    
;GDT (Global Descriptor Table)
;where we define different Segment Descriptors, which gives information about that segments, there base addr, limit, priviliged ring level 
;and then call the segment descriptor attached to specific segment
;https://wiki.osdev.org/Global_Descriptor_Table

gdt_start:
gdt_null:
    dd 0x0 ; since it is 16 bit addressing so dd = 32bits
    dd 0x0 ; total 64 bits of 0's , since first descriptor is always null

;offset 0x8
gdt_code:   ; actually it is simply a Segment Descriptor but we will use it for code segment
    dw 0xffff ; segment limit first 0-15 bits ,means it can access 2^16 = 64K adresses starting from base
    dw 0 ; Base first 0-15 bits
    db 0 ; Base 16-23 bits
    db 0x9a ; Access byte
    db 11001111b ; high 4 bits and low 4 bits of flags
    db 0 ; Base 24-31 bits
;offset 0x10
gdt_data:   ; actually it is simply a Segment Descriptor but we will use it for data segment
    dw 0xffff ; segment limit first 0-15 bits ,means it can access 2^16 = 64K adresses starting from base
    dw 0 ; Base first 0-15 bits
    db 0 ; Base 16-23 bits
    db 0x92 ; Access byte
    db 11001111b ; high 4 bits and low 4 bits of flags
    db 0 ; Base 24-31 bits

gdt_end: ;label to mark the end of gdt descriptor

gdt_descriptor: ; actual gdt descriptor that will be loaded
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
load32: ; so, Bios will only load first 512 bytes i.e. sector 0 into the memory, 
        ;after switching to protected mode, we have to write our own driver to load remaining sectors into the memory
    mov eax,1 ; starting logical blk addr i.e. sector number to load
    mov ecx, 100 ; no of sectors to load
    mov edi, 0x0100000 ; starting addr where to load the sectors
    call ata_lba_read ; call the subroutine to load the sectors
    jmp CODE_SEG:0x0100000 ; jump to the address 0x0100000 in memory

ata_lba_read:
    mov ebx, eax ; backup the LBA
    
    mov edx, 0x01F6 ; Port to send drive and bit 24 - 27 of LBA
    shr eax, 24 ; shift right eax by 24 bits
    or al, 0xE0 ; select the master drive and to select LBA mode
    out dx, al ; sending al to port written in dx reg 

    mov edx, 0x01F2 ; port to send no of sectors
    mov eax, ecx ; ecx contains no of sectors to read
    out dx, al

    mov edx, 0x1F3 ; Port to send bit 0 - 7 of LBA
    mov eax, ebx ; Get LBA from EBX
    out dx, al

    mov edx, 0x1F4 ; Port to send bit 8 - 15 of LBA
    mov eax, ebx ; Get LBA from EBX
    shr eax, 8 ; Get bit 8 - 15 in AL
    out dx, al

    mov edx, 0x1F5 ; Port to send bit 16 - 23 of LBA
    mov eax, ebx ; Get LBA from EBX
    shr eax, 16  ; Get bit 16 - 23 in AL
    out dx, al

    mov edx, 0x1F7 ; Command port
    mov al, 0x20  ; Read with retry.
    out dx, al
    
    ; Read all sectors into memory
.next_sector:
    push ecx
    ; waiting for sector data to read
.try_again:
    mov edx, 0x1f7
    in al, dx   ; get the data from 0x1f7 port
    test al, 8 ; check if it is equal to 8
    jz .try_again ; if equals then data is not ready to read, try again
.reading_sectors:
    ; data from 1 sector is ready to read
    mov ecx, 256 ; going to read 256 words = 512 bits
    mov edx, 0x1F0 ; data port in and out
    rep insw ; rep instruction will do insw till ecx is not zero 
    ;insw = Input word from I/O port specified in DX into memory location specified in ES:(E)DI
    pop ecx ; get the no of sectors back in ecx
    loop .next_sector ; decrement ecx count, if val > 0 then jmp to .next_sector label
.done_reading_sectors:
    ;ending reading sectors into memory
    ret

times 510-($ - $$) db 0 ; need to make bootloader of 512 bytes so filling remaining bytes to 0
dw 0xAA55 ; 0x55AA is the boot signature which bios looks for in a boot loader before loading it dw flips the bytes and needed in little endian