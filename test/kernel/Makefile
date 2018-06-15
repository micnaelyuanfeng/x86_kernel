#############################################
#                 MakeFile                  #
#############################################
KERN_SRC   := kernel.asm
UTIL_CSRC  := start.c

KERN_BIN := $(subst .asm,.o, $(KERN_SRC))
UTILC_BIN := $(patsubst %.c, %.o, $(UTIL_CSRC))

##############################################
#                  FLAGS                     #
##############################################
CFLAG    = -m32 -fno-builtin

.PHONY: all

##############################################
#  Compile BootLoader and Load Kernel Image  #
##############################################
all: $(UTILC_BIN) $(KERN_BIN)

$(UTILC_BIN): $(UTIL_CSRC)
	gcc $(CFLAG) $< -c -o $@	
	
$(KERN_BIN): $(KERN_SRC)
	nasm -f elf32 $< -o $@
	ld -T linker.ld -m elf_i386 kernel.o $(UTILC_BIN) -o kernel.bin