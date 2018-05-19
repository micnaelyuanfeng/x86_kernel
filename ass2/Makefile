.PHONY: install

install:
	sudo mount a.img -o loop /mnt/floppy
	sudo cp loader.bin /mnt/floppy
	sudo umount /mnt/floppy

	bochs -f bochsrc

.PHONY: uninstall

uninstall:
	sudo umount /mnt/floppy