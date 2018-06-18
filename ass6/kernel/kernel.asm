[SECTION .text]	

SECLETOR_KERNEL_CS			equ 	0x8
SECLETOR_KERNEL_VIDEO		equ		0x18
BaseOfVideoPhyAddr			equ		0xB8000

extern cstart; 
extern gdtPtr
 
global _kstart	 

_kstart:
	;mov ebp, kernelStackTop 
	mov esp, kernelStackTop

	sgdt [gdtPtr] 

	call cstart 
;error note -> not defined memCpu
;           -> compile execced range of 16 bit mode	 
;		    -> data type cast
;           ->

;Tutuorial code bug => self copy

	lgdt [gdtPtr]

	mov ax, SECLETOR_KERNEL_VIDEO
	mov gs, ax

	mov	ah, 0x0A				 
	mov	al, 'P'
	;mov [BaseOfVideoPhyAddr + ((80 * 17 + 0) * 2)], ax
	mov [gs:((80 * 3 + 0) * 2)], ax

	jmp SECLETOR_KERNEL_CS:csinit

csinit:
	;mov ax, SECLETOR_KERNEL_VIDEO
	;mov ds, ax 
	;mov es, ax
	;mov ss, ax
	;mov gs, ax
	mov	ah, 0x0A				 
	mov	al, 'K'
	;mov [BaseOfVideoPhyAddr + ((80 * 17 + 0) * 2)], ax
	mov [gs:((80 * 4 + 0) * 2)], ax

	jmp $
	

;can have multiple global in one section

[SECTION .bss]
kernelStack				resb		8192  ;4KB size kernel stack
kernelStackTop			equ			$

[SECTION .data]
global bios_mem_num

bios_mem_segs			dw          0
bios_mem_num			dw          0
