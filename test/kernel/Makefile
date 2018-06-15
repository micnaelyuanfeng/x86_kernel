#############################################
#                 MakeFile                  #
#############################################
KERN_SRC   := $(shell find . -name "*.asm")
UTIL_CSRC  := $(shell find . -name "*.c")

KERN_BIN  := $(patsubst %.asm, %.o, $(KERN_SRC))
UTILC_BIN := $(patsubst %.c, %.o, $(UTIL_CSRC))

##############################################
#                  FLAGS                     #
##############################################
CFLAG    = -m32 -fno-builtin

.PHONY: all

##############################################
#  Compile BootLoader and Load Kernel Image  #
##############################################
all: $(UTILC_BIN) $(KERN_BIN) kernel_make

%.o:%.c
	gcc $(CFLAG) $< -c -o $@	
	
%.o:%.asm
	nasm -f elf32 $< -o $@

kernel_make:
	ld -T linker.ld -m elf_i386 $(KERN_BIN) $(UTILC_BIN) -o kernel.bin
