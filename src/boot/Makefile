
BOOT_DIRECTORY=$(SRC_DIRECTOTY)/boot

.PHONY: all clean compile build

all: clean compile build

compile:

build: 
	echo "Building Bootloader"
	nasm -f bin $(BOOT_DIRECTORY)/boot.asm -o $(BIN_DIRECTORY)/boot.bin

clean:
	rm -rf $(BIN_DIRECTORY)/boot.bin
	rm -rf $(BIN_DIRECTORY)/disassemble_bootloader.txt

#disassemble the bootloader binary
open: dis
dis: disassemble_bin
disassemble_bin:
	ndisasm $(BIN_DIRECTORY)/boot.bin > $(BIN_DIRECTORY)/disassemble_bootloader.txt
