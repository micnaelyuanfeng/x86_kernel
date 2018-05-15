%define SelectorCode32	1 << 3
%define SelectorData32  2 << 3
%define SelectorVideo   3 << 3

[SECTION .s16]
[BITS 16]

	extern gdt_init

	extern gdtr

	GLOBAL kstart
kstart:
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x100

	;clear screen
	mov ax, 0x0600
	mov bx, 0x0700
	mov cx, 0x0
	mov dx, 0x0184F
	int 0x10

	call gdt_init

	cli

	lgdt [gdtr] 

	in  al, 0x92
	or  al, 0b010
	out 0x92, al

	mov eax, cr0
	or  eax, 0x1
	mov cr0, eax

	;cannot use SelectorVideo here, is only real mode addressing mode
	;cannot get correct memory address
	;jmp $

	jmp	dword SelectorCode32:0

[SECTION .s32]
[BITS 32]
	global Display_Msg
Display_Msg:
	
	mov ax,  SelectorData32
	mov ds,  ax
	mov ax,  SelectorVideo
	mov gs,  ax

	mov	edi, (80 * 12 + 0) * 2	
	mov	ah, 0x0c			
	mov	al, 'L'
	mov	[gs:edi], ax

	jmp $
