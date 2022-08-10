
.PHONY: all clean compile build

COMPLETE_KERNEL_OBJ = kernelfull.o
COMPLETE_KERNEL_BIN = kernel.bin

FILES =	$(BUILD_DIRECTORY)/kernel.asm.o \
		$(BUILD_DIRECTORY)/kernel.o

INCLUDES = -I$(SRC_DIRECTOTY)

FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: clean compile build	

compile: $(FILES)

# kernel.asm compilation
$(BUILD_DIRECTORY)/kernel.asm.o:
	nasm -f elf -g $(SRC_DIRECTOTY)/kernel.asm -o $(BUILD_DIRECTORY)/kernel.asm.o

# kernel.c compilation
$(BUILD_DIRECTORY)/kernel.o:
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $(SRC_DIRECTOTY)/kernel.c -o $(BUILD_DIRECTORY)/kernel.o

build: 
	echo "kernel building"
	i686-elf-ld -g -relocatable $(FILES) -o $(BUILD_DIRECTORY)/$(COMPLETE_KERNEL_OBJ)
	i686-elf-gcc $(FLAGS) -T $(SRC_DIRECTOTY)/linker.ld -o $(BIN_DIRECTORY)/$(COMPLETE_KERNEL_BIN) -ffreestanding -nostdlib $(BUILD_DIRECTORY)/kernelfull.o

clean:
	rm -rf $(FILES)
	rm -rf $(BUILD_DIRECTORY)/$(COMPLETE_KERNEL_OBJ)
	rm -rf $(BIN_DIRECTORY)/$(COMPLETE_KERNEL_BIN)

#disassemble the bootloader binary
open: dis
dis: disassemble_bin
disassemble_bin: