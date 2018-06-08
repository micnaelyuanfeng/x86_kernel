#############################################
#                 MakeFile                  #
#############################################
BOOT_SRC := boot.S
LOAD_SRC := loader.S
KERN_SRC := kernel.S

BOOT_BIN := $(subst .S,.bin, $(BOOT_SRC))
LOAD_BIN := $(subst .S,.bin, $(LOAD_SRC))
KERN_BIN := $(subst .S,.bin, $(KERN_SRC))

DISK     := a.img

##############################################
#  Install Bootloader and Load Kernel Image  #
##############################################
.PHONY: all

all: $(BOOT_BIN) 

$(BOOT_BIN): $(BOOT_SRC)
	nasm $< -o $@

#$(LOAD_BIN): $(LOAD_SRC)
#	nasm $< -o $@

#$(KERN_BIN): $(KERN_SRC)
#	nasm -f elf32 $< -o $@

.PHONY: install
install:
	dd if=boot.bin of=$(DISK) bs=512 count=1 conv=notrunc
	sudo mount a.img -o loop /mnt/floppy
	#sudo cp loader.bin /mnt/floppy
	#sudo cp kernel.bin /mnt/floppy
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

