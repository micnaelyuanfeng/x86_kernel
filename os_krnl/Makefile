#############################################
#                 MakeFile                  #
#############################################
BOOT_SRC := boot.asm
LOAD_SRC := loader.asm
KERN_SRC := kernel.asm

BOOT_BIN := $(subst .asm,.bin, $(BOOT_SRC))
LOAD_BIN := $(subst .asm,.bin, $(LOAD_SRC))
KERN_BIN := $(subst .asm,.bin, $(KERN_SRC))

DISK     := a.img

##############################################
#  Install Bootloader and Load Kernel Image  #
##############################################
.PHONY: all

all: $(BOOT_BIN) $(LOAD_BIN) $(KERN_BIN)

$(BOOT_BIN): $(BOOT_SRC)
	nasm $< -o $@

$(LOAD_BIN): $(LOAD_SRC)
	nasm $< -o $@

$(KERN_BIN): $(KERN_SRC)
	nasm -f elf32 $< -o $@

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
	rm *.o 
	rm *.img
	rm *.bin
	rm *.elf

dump:
	objdump -D loader.bin