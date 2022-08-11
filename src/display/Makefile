.PHONY: all clean compile pre_install $(OBJ_FILES)

#current Directory
#$(shell ls $(PWD) -F | grep "\/") ; for reference
DIRECTORY = .
#Find all .c and .asm files in all the directories found
SRC_FILES = $(foreach d,$(DIRECTORY),$(shell find $(d) -name *.c -or -name *.asm))
#create object file names by subsituting extensions with .o , using filter helps to substitute multiple files with different patterns
OBJ_FILES = $(filter %.o,$(patsubst %.asm,%.asm.o,$(SRC_FILES)) $(patsubst %.c,%.o,$(SRC_FILES)))
# #extract the file name from the absolute file paths
# FILES = $(foreach d,$(OBJ_FILES),$(shell basename $(d)))
	
all: compile pre_install clean

compile: $(OBJ_FILES)

# .asm compilation
%.asm.o: %.asm
	nasm -f elf -g $^ -o $@

# .c compilation
%.o: %.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c $^ -o $@

pre_install:
	for file in $(OBJ_FILES); do \
        cp -rfv $$file $(BUILD_DIRECTORY)/. ; \
    done

clean:
	rm -rf $(OBJ_FILES)

#disassemble the bootloader binary
open: dis
dis: disassemble_bin
disassemble_bin: