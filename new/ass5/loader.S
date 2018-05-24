BaseOfStack			equ		0x100

org	0100h
	jmp	LABEL_BEGIN

%include	"pm.inc"
%include    "gdt.inc"

[SECTION .s16]
[BITS	16]
LABEL_BEGIN:
	;In real mode, regs are 16 bits => all ops are 16 bits
	;Address Mode = SegReg << 4 + Offset
	;e.g Instruction Address = CodeSeg << 4 + IP
	;BIOS INT handlers are using this addressing mode
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

	;use VGA Mode does not affect cursor position
	mov ah, 0x13
	mov al, 0x1
	mov bh, 0x0
	mov bl, 0xb
	mov cx, LengthMsg0
	mov dh, 0x0
	mov dl, 0x0
	mov bp, Message0
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
	mov	ax, cs
	shl	eax, 4
	add	eax, STACK_START
	mov	word [GDT_KSTACK+ 2], ax
	shr	eax, 16
	mov	byte [GDT_KSTACK + 4], al
	mov	byte [GDT_KSTACK + 7], ah

	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, STACK_RING3_START
	mov	word [GDT_STACKRING3 + 2], ax
	shr	eax, 16
	mov	byte [GDT_STACKRING3 + 4], al
	mov	byte [GDT_STACKRING3 + 7], ah

	xor	eax, eax
	mov	ax, cs
	shl	eax, 4
	add	eax, TSS_START
	mov	word [GDT_TSS + 2], ax
	shr	eax, 16
	mov	byte [GDT_TSS + 4], al
	mov	byte [GDT_TSS + 7], ah

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
	add	eax, DATA_RING3_START
	mov	word [GDT_DATA_RING3 + 2], ax
	shr	eax, 16
	mov	byte [GDT_DATA_RING3 + 4], al
	mov	byte [GDT_DATA_RING3 + 7], ah


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
; PROTECT MODE Starts
; Address Mode changes here and BIOS INT handlers cannot be used
; Address Mode = GDT[selector].baseAddress + offset
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
	mov	edi, (80 * 1 + 0) * 2	
	cld
.1:
	lodsb
	test al, al
	jz	.2
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1
.2:
	;call selectorCallGate:0

	mov ax, selectorTSS	
	ltr ax

	; CPU Mode = Kernel Mode and User Mode
	; Use Far Call and Far Return to switch CPU mode
	; Far means change Segment
	; Near means within a Segment
	; Far can change CPU mode base on Intel Chip Design

	push selectorStackRing3
	push TopOfStackRing3
	push selectorCodeRing3
	push 0

	retf

PM_START_TEST:
	mov ax, selectorVideo
	mov gs, ax

	mov ax, selectorData
	mov ds, ax

	mov ah, 0x0C
	;mov al, 'C'
	xor esi, esi
	xor edi, edi

	mov esi, offsetMsg2
	mov edi, (80 * 3 + 0) * 2
	;mov [gs:edi], ax
	cld
.1:
	lodsb
	test al, al
	jz	.2
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1
.2:

	jmp $
SegCode32LenTest		equ $ - PM_START_TEST
SegCode32Len			equ	$ - PM_START

[SECTION .code_ring3]
ALIGN 32
[BITS 32]
CODE_RING3_START:
	mov ax, selectorVideo
	mov gs, ax

	mov ax, selectorDataRing3
	mov ds, ax ;cannot work DPL and touch ds will cause CPU does Priveillege check

	mov ah, 0x0C
	;mov al, '3'
	xor esi, esi
	xor edi, edi

	mov esi, offsetMsg4
	mov edi, (80 * 2 + 0) * 2
;	mov [gs:edi], ax
	cld
;cannot use it, no user level data secgment 
;DPL is not correct
.1:
	lodsb
	test al, al
	jz	.2
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1
.2:
	call selectorCallGate:0

SegCodeRing3Len 		equ $ - CODE_RING3_START

[SECTION .stack_region]
ALIGN 32
[BITS 32]

STACK_START:
	times	512			db	0
TopOfStack				equ	$ - STACK_START - 1

[SECTION .stack_region_ring3]
ALIGN 32
[BITS 32]
STACK_RING3_START:
	times   512     	db 0
TopOfStackRing3     	equ $ - STACK_RING3_START - 1

[SECTION .data]
ALIGN 32
[BITS 32]

DATA_START:
Message0				db  "In Real Mode", 0
LengthMsg0              equ $ - Message0

Message1                db  "In Protect Mode", 0
Message2				db  "Ring 0", 0

offsetMsg1				equ Message1 - $$
offsetMsg2				equ Message2 - $$

SegData32Len 			equ $ - DATA_START

[SECTION .data_ring3]
ALIGN 32
[BITS 32]

DATA_RING3_START:

Message4				db   "Ring 3", 0

offsetMsg4				equ   Message4 - $$

SegData32Ring3Len 		equ	$ - DATA_RING3_START


;TSS data structure
;TSS will be used to get Stack when switch from low ring to high ring
;Different ring has different stack
[SECTION .tss_region]
ALIGN 32
[BITS 32]

TSS_START:
	dd 		0
	dd      TopOfStack
	dd      selectorKstack
	dd 		0
	dd      0
	dd 		0
	dd      0
	dd 		0
	dd      0
	dd 		0
	dd      0
	dd 		0
	dd      0
	dd 		0
	dd      0
	dd 		0
	dd      0
	dd 		0
	dd      0
	dd 		0
	dd      0
	dd 		0
	dd      0
	dd 		0
	dd      0
	dw      0
	dw      $ - TSS_START + 2 ; I/O
	db      0x0FF             ; I/O 
tssLen equ $ - TSS_START
