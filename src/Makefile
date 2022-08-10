
.PHONY: all clean compile build

FILES =	$(BUILD_DIRECTORY)/kernel.asm.o 
COMPLETE_KERNEL_OBJ = kernelfull.o
COMPLETE_KERNEL_BIN = kernel.bin

all: clean compile build	

compile: $(FILES)

# kernel.asm compilation
$(BUILD_DIRECTORY)/kernel.asm.o:
	nasm -f elf -g $(SRC_DIRECTOTY)/kernel.asm -o $(BUILD_DIRECTORY)/kernel.asm.o


build: 
	echo "kernel building"
	i686-elf-ld -g -relocatable $(FILES) -o $(BUILD_DIRECTORY)/$(COMPLETE_KERNEL_OBJ)
	i686-elf-gcc -T $(SRC_DIRECTOTY)/linker.ld -o $(BIN_DIRECTORY)/$(COMPLETE_KERNEL_BIN) -ffreestanding -nostdlib $(BUILD_DIRECTORY)/kernelfull.o

clean:
	rm -rf $(FILES)
	rm -rf $(BUILD_DIRECTORY)/$(COMPLETE_KERNEL_OBJ)
	rm -rf $(BIN_DIRECTORY)/$(COMPLETE_KERNEL_BIN)

#disassemble the bootloader binary
open: dis
dis: disassemble_bin
disassemble_bin: