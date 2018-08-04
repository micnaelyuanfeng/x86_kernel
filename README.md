# x86_kernel

Bootloader   := Implemented	
Loader       := Implemented	
GDT          := Implemented	
IDT          := Implemented	
8259A        := Implemented	
Keyboard     := Implemented	


To do list:
-> Spicous Interrupt Handler
-> Paging Table = Paging Enable
-> Physical Memory Management = Malloc and Free
-> Simple System Call

Postpone list
-> Process Creation
-> Page Table 
-> File System


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

9000: 03fe (.ctxt) add byte ptr ds:[bx + si], al ; 0000


https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/4/html/Using_ld_the_GNU_Linker/assignments.html
