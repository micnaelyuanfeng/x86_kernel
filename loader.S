[SECTION .loader_start]
[BITS 16]

;declare variable and macro
Message                 db      "Loader Start", 0        

BaseOfStack			 	equ    	0x7c00

;set new stack
	global  loader_start

loader_start:
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov gs, ax
	mov fs, ax
	mov sp, BaseOfStack

	;clear screen
	mov ax, 0x0600
	mov bx, 0x0700
	mov cx, 0x0
	mov dx, 0x0184F
	int 0x10

	call DISPLAY_STRING

	jmp $


DISPLAY_STRING:
	push ax		; context saving
	push bx
	push cx
	push dx

	mov  al, 0x1
	mov  ah, 0x13
	mov  bh, 0x0
	mov  bl, 0x7
	mov  ax, ds
	mov  es, ax

	mov  cx, 0xC

	int  0x13

	pop  dx
	pop  cx
	pop  bx
	pop  ax
	ret