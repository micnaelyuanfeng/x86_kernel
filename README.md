# x86_kernel

Bootloader Implemented

To do list:
Loader = setup GDT, IDT, Page Table

.S = can add preprocess in compilation 
.s and .asm = .s and .asm are the same, there is no preprocess in compilation

Linux Platform
All object file should have a format before linking, the preferred one is elf
To get object file in elf format:
ASM code = nasm -f elf32/elf64 file.S -o file.o
	        = nasm file.S -o file.bin
C code = gcc -c -Wall - m32/64 file.c -o file.o
 -c is used to block linking process of gcc

After all these, there are separated objects for linking and linking command is
ld -T linker.ld -m elf_i386/(others) object_files -o boot.bin
