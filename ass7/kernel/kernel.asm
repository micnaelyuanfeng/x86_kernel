[SECTION .text]	

SECLETOR_KERNEL_CS			equ 	0x8
SECLETOR_KERNEL_VIDEO		equ		0x18
BaseOfVideoPhyAddr			equ		0xB8000

extern gdtPtr
extern idtPtr;

extern cstart;
extern init_8259A 
extern intr_test;
extern set_cursor
 
global _kstart	 

_kstart:
	;mov ebp, kernelStackTop 
	mov esp, kernelStackTop

	mov dword [bios_mem_segs], edx
	mov dword [bios_mem_num], ebx

	cli

	sgdt [gdtPtr] 

;error note -> not defined memCpu
;           -> compile execced range of 16 bit mode	 
;		    -> data type cast
;           ->

;Tutuorial code bug => self copy

	lgdt [gdtPtr]

	;call init_8259A

	call cstart 

	lidt [idtPtr]

	sti
	;call set_cursor;

	;mov ax, SECLETOR_KERNEL_VIDEO
	;mov gs, ax

	;mov	ah, 0x0A				 
	;mov	al, 'P'
	;mov [BaseOfVideoPhyAddr + ((80 * 20 + 0) * 2)], ax
	;mov [gs:((80 * 3 + 0) * 2)], ax

	;sti

	;call intr_test
	;int 0x10

	;call intr_test

	;mov	ah, 0x0A				 
	;mov	al, 'P'
	;mov [BaseOfVideoPhyAddr + ((80 * 20 + 0) * 2)], ax
	;mov [gs:((80 * 3 + 0) * 2)], ax
	
	;Set up Paging Table
	
	jmp $
	jmp SECLETOR_KERNEL_CS:csinit

csinit:
	;mov ax, SECLETOR_KERNEL_VIDEO
	;mov ds, ax 
	;mov es, ax
	;mov ss, ax
	;mov gs, ax
	;mov	ah, 0x0A				 
	;mov	al, 'K'
	;mov [BaseOfVideoPhyAddr + ((80 * 17 + 0) * 2)], ax
	;mov [gs:((80 * 4 + 0) * 2)], ax

	jmp $

setup_page_table:
	xor edx, edx
	

;can have multiple global in one section

[SECTION .bss]
kernelStack				resb		16384  ;4KB size kernel stack
kernelStackTop			equ			$

[SECTION .data]
global bios_mem_num
global bios_mem_segs

bios_mem_segs			dd          0
bios_mem_num			dd          0

;page tab;e
page_dir_base			equ         0x200000
page_table_base         equ         0x201000
page_coverage			equ         4096 << 10
