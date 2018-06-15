#############################################
#                 MakeFile                  #
#############################################
BOOTLOADER := bootloader
KERN_SRC   := kernel.asm
UTIL_CSRC  := start.c

KERN_BIN := $(subst .asm,.o, $(KERN_SRC))

UTILC_BIN := $(patsubst %.c, %.o, $(UTIL_CSRC))

DISK     := a.img

##############################################
#                  FLAGS                     #
##############################################
CFLAG    = -m32 -fno-builtin

.PHONY: all

##############################################
#  Compile BootLoader and Load Kernel Image  #
##############################################
all: bootloader_make kernel_make 

bootloader_make:
	cd bootloader && $(MAKE)
	
kernel_make:
	cd kernel && $(MAKE)

	
##############################################
#  Install Bootloader and Load Kernel Image  #
##############################################
install:
	dd if=bootloader/boot.bin of=$(DISK) bs=512 count=1 conv=notrunc
	sudo mount a.img -o loop /mnt/floppy
	sudo cp bootloader/loader.bin /mnt/floppy
	sudo cp kernel/kernel.bin /mnt/floppy
	sudo umount /mnt/floppy

	bochs -f bochsrc

uninstall:
	sudo umount /mnt/floppy

remove:
	rm bootloader/*.bin
	rm kernel/*.bin
	rm kernel/*.o
dump:
	objdump -D loader.bin
