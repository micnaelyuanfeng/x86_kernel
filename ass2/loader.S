; selector definition
%define CODE_SELECTOR  0x08
%define DATA_SELECTOR  0x10
%define TEST_SELECTOR  0x18
%define STACK_SELECTOR 0x20

%define CURSOR_POS     ((80 * 11 + 79) * 2)

[SECTION .START16]
ALIGN 32
[BITS 16]
	global loader_start

	extern gdtr
	extern gdt_init

loader_start:
	;clear screen
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x1000

	mov ax, 0x0600
	mov bx, 0x0700
	mov cx, 0x0
	mov dx, 0x0184F
	int 0x10

	;cannot do this here, its not pm yet
	;mov ax, DATA_SELECTOR
	;mov gs, ax
	;mov ah, 0xF
	;mov al, 'P'
	;mov [gs:CURSOR_POS], ax

	call gdt_init

	lgdt [gdtr]

	cli

	;enable A20 Address line
	in  al, 0x92
	or  al, 0b000010
	out 0x92, al

	;set PE bit of CR0
	mov  eax, cr0
	or   eax, 0x01
	mov  cr0, eax

	;jmp $
	jmp dword CODE_SELECTOR:0

[SECTION .START32]
ALIGN 32
[BITS 32]
	global pm_start
pm_start:
	mov ax, cs
	mov ds, ax
	mov ss, ax
	mov es, ax

	mov ax, DATA_SELECTOR
	mov gs, ax

	mov edi, CURSOR_POS
	mov ah,  0x0c
	mov al, 'C'
	mov [gs:edi], ax

	int 0x13
	
	jmp $