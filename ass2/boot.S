%define CODE_SELECTOR  0x8
%define DATA_SELECTOR  0x10
%define CURSOR_POS     ((80 * 11 + 0) * 2)

[SECTION .loader_section]
[BITS 16]
	global loader_start

	extern gdtr

loader_start:
	lgdt [gdtr]

	;enable A20 Address line
	in  al, 0x92
	or  al, 0b10
	out 0x92, al

	;set PE bit of CR0
	mov  eax, cr0
	or   eax, 0x01
	mov  cr0, eax

	jmp dword CODE_SELECTOR:0

[SECTION .protected_mode_start]
[BITS 32]
	mov ax, DATA_SELECTOR
	mov gs, ax

	mov edi, CURSOR_POS
	mov ah, 0x0c
	mov al, 'P'
	mov [gs:edi], ax

	int 0x13
	jmp $