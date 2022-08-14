#Absoulte Paths
export SRC_DIRECTORY=${PWD}/src
export BIN_DIRECTORY=${PWD}/bin
export BUILD_DIRECTORY=${PWD}/build

#Directories to Build
DIRECTORIES = $(SRC_DIRECTORY)

#MACROS
OS_BIN = os.bin 

.PHONY: clean all $(DIRECTORIES) run make_os

all: $(DIRECTORIES) make_os
all_r: $(DIRECTORIES) clean_os

$(DIRECTORIES):
	$(MAKE) -C $@ $(MAKE_TARGET_CMD)

make_os: clean_os
	dd if=$(BIN_DIRECTORY)/boot.bin >> $(BIN_DIRECTORY)/$(OS_BIN)
	dd if=$(BIN_DIRECTORY)/kernel.bin >> $(BIN_DIRECTORY)/$(OS_BIN)
	dd if=/dev/zero bs=512 count=100 >> $(BIN_DIRECTORY)/$(OS_BIN)

clean_os:
	rm -rf $(BIN_DIRECTORY)/$(OS_BIN)

#run the emulator
run: run_qemu_system_x86

run_qemu_system_x86:
	qemu-system-x86_64 -hda $(BIN_DIRECTORY)/$(OS_BIN)

#disassemble the bootloader binary
open: dis
dis: disassemble_bin
disassemble_bin:
	# ndisasm $(BIN_DIRECTORY)/os.bin > $(BIN_DIRECTORY)/disassemble_os.txt