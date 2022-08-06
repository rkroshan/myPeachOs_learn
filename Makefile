
#build the bootloader
build: clean build_bootloader

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
	ndisasm boot.bin

#clean 
clean:
	rm boot.bin