
#build the bootloader
build: clean build_bootloader_append_msg

build_bootloader_append_msg:build_bootloader
#appending the message to the same hard drive
	dd if=./message.txt >> ./boot.bin
#creating extra sector padded with zero so that message should be read from that sector	
	dd if=/dev/zero bs=512 count=1 >> ./boot.bin

build_bootloader:
	nasm -f bin boot.asm -o boot.bin

#run the emulator
run: run_qemu_system_x86

run_qemu_system_x86:
	qemu-system-x86_64 -hda ./boot.bin

#disassemble the bootloader binary
open: dis
dis: disassemble_bin
disassemble_bin:
	ndisasm boot.bin > disassemble_bin.txt

#clean 
clean:
	- rm boot.bin