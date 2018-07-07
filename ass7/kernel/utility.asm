[SECTION .text]
GLOBAL MemCopy

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

GLOBAL DisplayString

DisplayString:
	disp_str:
	push	ebp
	mov	ebp, esp

	mov	esi, [ebp + 8]	
	mov	edi, [disp_pos]
	mov	ah, 0Fh
.1:
	lodsb
	test	al, al
	jz	.2
	cmp	al, 0Ah	
	jnz	.3
	push	eax
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1

.2:
	mov	[disp_pos], edi

	pop	ebp
	ret

GLOBAL WriteMem

WriteMem:
	push ecx
	push eax

	mov ecx, [esp + 4]
	mov al,  [esp + 8]
	mov [ecx], al

	pop eax
	push ecx

	ret

[SECTION .data]
disp_pos			dd			0x0