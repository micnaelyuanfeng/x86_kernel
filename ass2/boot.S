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
	mov gs, ax
	mov ss, ax
	mov sp, 0x100

	call gdt_init

	lgdt [gdtr]

	cli 

	in  al, 0x92
	or  al, 0b010
	out 0x92, al

	mov eax, cr0
	or  eax, 0x1
	mov cr0, eax

	jmp	dword SelectorCode32:0

[SECTION .s32]
	global Display_Msg
Display_Msg:
	mov ax,  SelectorData
	mov ds,  ax
	mov ax,  SelectorVideo
	mov gs,  ax

	;mov ah,  0x0c
	;xor esi, esi
	;xor edi, edi
	;mov esi, PMMsgLength
	;mov edi, (80 * 10 + 0) * 2

	mov	edi, (80 * 12 + 0) * 2	
	mov	ah, 0x0c			
	mov	al, 'L'
	mov	[gs:edi], ax

	jmp $
