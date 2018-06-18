#############################################
#                 MakeFile                  #
#############################################
BOOTLOADER := bootloader
KERNEL     := kernel

DISK       := a.img

.PHONY: all

##############################################
#  Compile BootLoader and Load Kernel Image  #
##############################################
all: bootloader_make kernel_make make_done

bootloader_make:
	cd bootloader && $(MAKE)
	
kernel_make:
	cd kernel && $(MAKE)

make_done:
	echo "build successfully"

	
##############################################
#  Install Bootloader and Load Kernel Image  #
##############################################
.PHONY: install
install:
	dd if=$(BOOTLOADER)/boot.bin of=$(DISK) bs=512 count=1 conv=notrunc
	sudo mount a.img -o loop /mnt/floppy
	sudo cp $(BOOTLOADER)/loader.bin /mnt/floppy
	sudo cp $(KERNEL)/kernel.bin /mnt/floppy
	sudo umount /mnt/floppy

	bochs -f bochsrc

uninstall:
	sudo umount /mnt/floppy

remove:
	rm bootloader/*.bin
	rm kernel/*.o
	rm kernel/*.bin

dump:
	objdump -D loader.bin
