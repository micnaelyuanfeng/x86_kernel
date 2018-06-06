SRC := loader.S
BIN := $(subst .S,.bin, $(SRC))

$(BIN) : $(SRC)
	nasm $< -o $@

##############################################
#  Install Bootloader and Load Kernel Image  #
##############################################
.PHONY: install

install:
	sudo mount a.img -o loop /mnt/floppy
	sudo cp loader.bin /mnt/floppy
	sudo umount /mnt/floppy

	bochs -f bochsrc

.PHONY: uninstall

uninstall:
	sudo umount /mnt/floppy

.PHONY: remove

remove:
	rm *.o 
	rm *.img
	rm *.bin
	rm *.elf

.PHONY: dump

dump:
	objdump -D loader.bin
