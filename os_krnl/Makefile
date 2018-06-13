#############################################
#                 MakeFile                  #
#############################################
BOOT_SRC  := boot.asm
LOAD_SRC  := loader.asm
KERN_SRC  := kernel.asm
UTIL_CSRC := start.c
UTIL_NSRC := utility.asm

BOOT_BIN := $(subst .asm,.bin, $(BOOT_SRC))
LOAD_BIN := $(subst .asm,.bin, $(LOAD_SRC))
KERN_BIN := $(subst .asm,.o, $(KERN_SRC))

UTILC_BIN := $(subst .c,.o, $(UTIL_CSRC))
UTILN_BIN := $(subst .asm,.o, $(UTIL_NSRC))



DISK     := a.img

##############################################
#                  FLAGS                     #
##############################################
CFLAG    = -m32 -fno-builtin


.PHONY: all

##############################################
#  Compile BootLoader and Load Kernel Image  #
##############################################
all: $(BOOT_BIN) $(LOAD_BIN) $(UTILC_BIN) $(UTILN_BIN) $(KERN_BIN)

$(BOOT_BIN): $(BOOT_SRC)
	nasm $< -o $@

$(LOAD_BIN): $(LOAD_SRC)
	nasm $< -o $@

$(UTILC_BIN): $(UTIL_CSRC)
	gcc $(CFLAG) $< -c -o $@	

$(UTILN_BIN): $(UTIL_NSRC)	
	nasm -f elf32 $< -o $@
	
$(KERN_BIN): $(KERN_SRC)
	nasm -f elf32 $< -o $@
	#ld -Ttext 0x30400 -m elf_i386 kernel.o -o kernel.bin
	ld -T linker.ld -m elf_i386 kernel.o $(UTILC_BIN) $(UTILN_BIN) -o kernel.bin

	
##############################################
#  Install Bootloader and Load Kernel Image  #
##############################################
install:
	dd if=boot.bin of=$(DISK) bs=512 count=1 conv=notrunc
	sudo mount a.img -o loop /mnt/floppy
	sudo cp loader.bin /mnt/floppy
	sudo cp kernel.bin /mnt/floppy
	sudo umount /mnt/floppy

	bochs -f bochsrc

uninstall:
	sudo umount /mnt/floppy

remove:
	rm *.bin
	rm *.o
dump:
	objdump -D loader.bin
