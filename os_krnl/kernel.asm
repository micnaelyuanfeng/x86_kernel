[SECTION .text]	

global _kstart	

_kstart:
	mov ebp, kernelStackTop 
	mov esp, kernelStackTop

	sgdt [oldgdtPtrPhyAddr]

	push dword gdtPhyAddr
	push dword oldgdtPtrPhyAddr
	push dword gdtPhyAddr + 2

	call MemCopy

	add esp, 0xC

	lgdt [gdtPhyAddr]

	mov	ah, 0x0A				 
	mov	al, 'P'
	mov	[gs:((80 * 1 + 39) * 2)], ax

	jmp	$

MemCopy:
	push ebp	 ;save caller ebp
	mov  ebp, esp ;current caller esp is callee ebp

	push esi     ; index registers
	push edi     ; callee save for self using
	push ecx
	push eax

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

	pop eax
	pop ecx
	pop edi
	pop esi
	mov esp, ebp
	pop ebp

	ret 

[SECTION .bss]
kernelStack				resb		2048
kernelStackTop			equ			$

[SECTION .gdt]
oldGdtPtr:
	times         6 	db      0
oldgdtPtrPhyAddr		equ     KernelEntryPhyAddr + oldGdtPtr

[SECTION .data]
%include "pm.inc"

KernelEntryPhyAddr		equ		0x030400 

GDT_TABLE_START:
GDT_DEFAULT:	Descriptor			0,			0,					0		
GDT_CODE_SYS:   Descriptor     		0,	  		0xFFFFF,			DA_CR|DA_32|DA_LIMIT_4K 
GDT_DATA_SYS:	Descriptor			0,	  		0xFFFFF,			DA_DRW|DA_32|DA_LIMIT_4K ;Data Segment Read and Write
GDT_VIDEO:		Descriptor	  		0xB8000,    0xFFFF,          	DA_DRW|DA_DPL3 ;Data Segment Read and Write

selectorCode		equ		GDT_CODE_SYS  - GDT_DEFAULT
selectorData		equ     GDT_DATA_SYS  - GDT_DEFAULT
selectorVideo       equ     GDT_VIDEO     - GDT_DEFAULT   + SA_RPL3

gdtLen				equ		$ - GDT_TABLE_START
gdtPtr				dw		gdtLen - 1 
					dd      KernelEntryPhyAddr + GDT_DEFAULT
gdtPhyAddr			equ		KernelEntryPhyAddr + gdtPtr
