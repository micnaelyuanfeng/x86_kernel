; $ nasm -f elf kernel.asm -o kernel.o
; $ ld -s kernel.o -o kernel.bin   
[section .text]	

global _start	

_start:	
	mov	ah, 0Fh				; 0000: 黑底    1111: 白字
	mov	al, 'K'
	mov	[gs:((80 * 1 + 39) * 2)], ax	; 屏幕第 1 行, 第 39 列。
	jmp	$
