[SECTION .text]	
SECLETOR_KERNEL_CS			equ 0x8

extern cstart; 
extern gdtPtr
 
global _kstart	 

_kstart:
	mov ebp, kernelStackTop 
	mov esp, kernelStackTop

	mov dword [bios_mem_num], ecx
	mov dword [bios_mem_segs], ebx
 
	xor ecx, ecx 
	xor ebx, ebx

	sgdt [gdtPtr] 

	call cstart 
  
;error note -> not defined memCpu
;           -> compile execced range of 16 bit mode	 
;		    -> data type cast
;           ->
	lgdt [gdtPtr]

	mov	ah, 0x0A				 
	mov	al, 'P'
	mov	[gs:((80 * 20 + 0) * 2)], ax

	mov	ah, 0x0A				 
	mov	al, [bios_mem_num]
	mov	[gs:((80 * 21 + 0) * 2)], ax

	jmp SECLETOR_KERNEL_CS:csinit

csinit:
	jmp $
	

;can have multiple global in one section

[SECTION .bss]
kernelStack				resb		4096  ;4KB size kernel stack
kernelStackTop			equ			$

[SECTION .data]
bios_mem_segs			dw          0
bios_mem_num			dw          0
