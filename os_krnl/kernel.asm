[SECTION .text]	
SECLETOR_KERNEL_CS			equ 0x8
extern cstart; 
extern gdtPtr
 
global _kstart	

_kstart:
	mov ebp, kernelStackTop 
	mov esp, kernelStackTop

	sgdt [gdtPtr]

	;push dword gdtPhyAddr
	;push dword oldgdtPtrPhyAddr
	;push dword gdtPhyAddr + 2

	call cstart

	;add esp, 0xC
  
;error note -> not defined memCpu
;           -> compile execced range of 16 bit mode	 
;		    -> data type cast
;           ->
	lgdt [gdtPtr]

	mov	ah, 0x0A				 
	mov	al, 'P'
	mov	[gs:((80 * 20 + 0) * 2)], ax

	jmp SECLETOR_KERNEL_CS:csinit

csinit:
	jmp $

global MemCopy

MemCopy:
	push ebp	 ;save caller ebp
	mov  ebp, esp ;current caller esp is callee ebp

	push esi     ; index registers
	push edi     ; callee save for self using
	push ecx

	mov  edi, [ebp + 8]
	mov  esi, [ebp + 12]
	mov  ecx, [ebp + 16]

.cpyGoOn:
	cmp ecx, 0
	jz  .cpyDone

	mov al, [ds:esi]
	inc esi

	mov byte [es:edi], al
	inc edi

	dec ecx
	jmp .cpyGoOn

.cpyDone:
	mov eax, [ebp + 8] ;eax for return value = convention

	pop ecx
	pop edi
	pop esi
	mov esp, ebp
	pop ebp

	ret 

[SECTION .bss]
kernelStack				resb		2048
kernelStackTop			equ			$

