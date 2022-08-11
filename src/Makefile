
COMPLETE_KERNEL_OBJ = kernelfull.o
COMPLETE_KERNEL_BIN = kernel.bin

#Directories to Build
SUBDIR = $(shell ls -F | grep "\/" )
export INCLUDES = -I$(SRC_DIRECTORY)
export FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

#define phony
.PHONY: all clean build $(SUBDIR) boot

all: clean $(SUBDIR) build 
# all: clean boot

# boot:
# 	$(MAKE) -C /home/rkroshan/Desktop/myPeachOs_learn/src/utils $(MAKE_TARGET_CMD)

$(SUBDIR):
	$(MAKE) -C $@ $(MAKE_TARGET_CMD)

build: 
	echo "kernel building"
	i686-elf-ld -g -relocatable $(BUILD_DIRECTORY)/*.o -o $(BUILD_DIRECTORY)/$(COMPLETE_KERNEL_OBJ)
	i686-elf-gcc $(FLAGS) -T $(SRC_DIRECTORY)/linker.ld -o $(BIN_DIRECTORY)/$(COMPLETE_KERNEL_BIN) -ffreestanding -nostdlib $(BUILD_DIRECTORY)/kernelfull.o

clean:
	rm -rf $(BUILD_DIRECTORY)/*.o
	rm -rf $(BIN_DIRECTORY)/*.bin

#disassemble the bootloader binary
disassemble_bin: