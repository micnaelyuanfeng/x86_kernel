
BaseOfStack			equ		0x100

org	0100h
	jmp	LABEL_BEGIN

%include	"pm.inc"
%include    "gdt.inc"

[SECTION .s16]
[BITS	16]
LABEL_BEGIN:
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, BaseOfStack

	mov ax, 0x0600
	mov bx, 0x0700
	mov cx, 0x0
	mov dx, 0x0184F
	int 0x10

	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, PM_START
	mov	word [GDT_CODE + 2], ax
	shr	eax, 16
	mov	byte [GDT_CODE + 4], al
	mov	byte [GDT_CODE + 7], ah

	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, PM_START_TEST
	mov	word [GDT_CODE_TEST + 2], ax
	shr	eax, 16
	mov	byte [GDT_CODE_TEST + 4], al
	mov	byte [GDT_CODE_TEST + 7], ah

	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, CODE_RING3_START
	mov	word [GDT_CODE_RING3 + 2], ax
	shr	eax, 16
	mov	byte [GDT_CODE_RING3 + 4], al
	mov	byte [GDT_CODE_RING3 + 7], ah

	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, DATA_START
	mov	word [GDT_DATA + 2], ax
	shr	eax, 16
	mov	byte [GDT_DATA + 4], al
	mov	byte [GDT_DATA + 7], ah


	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, GDT_DEFAULT		
	mov	dword [gdtPtr + 2], eax	

	lgdt	[gdtPtr]

	cli

	in	al, 0x92
	or	al, 0b010
	out	0x92, al


	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

	jmp	dword selectorCode:0

[SECTION .s32]
[BITS	32]

PM_START:
	mov	ax, selectorData
	mov	ds, ax			

	mov ax, selectorVideo
	mov gs, ax

	mov esp, TopOfStack

	mov	ah,  0x0C			
	xor	esi, esi
	xor	edi, edi
	mov	esi, offsetMsg1	
	mov	edi, (80 * 0 + 0) * 2	
	cld
.1:
	lodsb
	test al, al
	jz	.2
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1
.2:
	call selectorCallGate:0

	jmp $

PM_START_TEST:
	mov ax, selectorVideo
	mov gs, ax

	mov ah, 0x0C
	xor esi, esi
	xor edi, edi

	mov esi, offsetMsg2
	mov edi, (80 * 1 + 0) * 2

.1:
	lodsb
	test al, al
	jz	.2
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1
.2:

	retf
	;jmp $
SegCode32LenTest	equ $ - PM_START_TEST
SegCode32Len		equ	$ - PM_START

[SECTION .code_ring3]
ALIGN 32
[BITS 32]
CODE_RING3_START:
	mov ax, selectorVideo
	mov gs, ax

	mov ah, 0x0C
	xor esi, esi
	xor edi, edi

	mov esi, offsetMsg3
	mov edi, (80 * 1 + 0) * 2

.1:
	lodsb
	test al, al
	jz	.2
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1
.2:
	retf

SegCodeRing3Len 	equ $ - CODE_RING3_START

[SECTION .stack_region]
ALIGN 32
[BITS 32]

STACK_START:
	times	512		db	0
TopOfStack			equ	$ - STACK_START - 1

[SECTION .data]
ALIGN 32
[BITS 32]

DATA_START:

Message1                db  "In Protect Mode", 0
Message2				db  "Ring 0", 0
Message3				db  "Ring 3", 0

offsetMsg1				equ Message1 - $$
offsetMsg2				equ Message2 - $$
offsetMsg3				equ Message3 - $$

SegData32Len		    equ	$ - DATA_START

