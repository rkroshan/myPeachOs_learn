
SRC_DIRECTOTY=${PWD}/src
BIN_DIRECTORY=${PWD}/bin
BUILD_DIRECTORY=${PWD}/build

FILES = $(BUILD_DIRECTORY)/kernel.asm.o 

all: clean build_bootloader $(BIN_DIRECTORY)/kernel.bin disassemble_bin
	dd if=$(BIN_DIRECTORY)/boot.bin >> $(BIN_DIRECTORY)/os.bin
	dd if=$(BIN_DIRECTORY)/kernel.bin >> $(BIN_DIRECTORY)/os.bin
	dd if=/dev/zero bs=512 count=100 >> $(BIN_DIRECTORY)/os.bin	

#build kernel.asm.o
$(BUILD_DIRECTORY)/kernel.asm.o: $(SRC_DIRECTOTY)/kernel.asm
	nasm -f elf -g $^ -o $@

#building kernel.bin
$(BIN_DIRECTORY)/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o $(BUILD_DIRECTORY)/kernelfull.o
	i686-elf-gcc -T $(SRC_DIRECTOTY)/linker.ld -o $@ -ffreestanding -nostdlib $(BUILD_DIRECTORY)/kernelfull.o

#build the bootloader
build_bootloader:
	nasm -f bin $(SRC_DIRECTOTY)/boot/boot.asm -o $(BIN_DIRECTORY)/boot.bin

#run the emulator
run: run_qemu_system_x86

run_qemu_system_x86:
	qemu-system-x86_64 -hda $(BIN_DIRECTORY)/os.bin

#disassemble the bootloader binary
open: dis
dis: disassemble_bin
disassemble_bin:
	ndisasm $(BIN_DIRECTORY)/boot.bin > $(BIN_DIRECTORY)/disassemble_bootloader.txt
	# ndisasm $(BIN_DIRECTORY)/os.bin > $(BIN_DIRECTORY)/disassemble_os.txt

#clean 
clean:
	rm -rf $(BIN_DIRECTORY)/*.bin
	rm -rf $(BIN_DIRECTORY)/*.txt
	rm -rf $(FILES)
	rm -rf $(BUILD_DIRECTORY)/kernelfull.o