
DisplayAL:
	push	ecx
	push	edx
	push	edi

	mov	edi, [displayPosition]

	mov	ah, 0Eh			
	mov	dl, al
	shr	al, 4
	mov	ecx, 2
.begin:
	and	al, 01111b
	cmp	al, 9
	ja	.1
	add	al, '0'
	jmp	.2
.1:
	sub	al, 0Ah
	add	al, 'A'
.2:
	mov	[gs:edi], ax
	add	edi, 2

	mov	al, dl
	loop	.begin
	;add	edi, 2

	mov	[displayPosition], edi

	pop	edi
	pop	edx
	pop	ecx

	ret

DisplayIntegar:
	mov ah, 0x07
	mov al, '0'
	push	edi
	mov	edi, [displayPosition]
	mov	[gs:edi], ax
	add	edi, 0x2
	mov	[displayPosition], edi
	pop	edi

	mov ah, 0x07
	mov al, 'x'
	push	edi
	mov	edi, [displayPosition]
	mov	[gs:edi], ax
	add	edi, 0x2
	mov	[displayPosition], edi
	pop	edi

	mov	eax, [esp + 4]
	shr	eax, 24
	call	DisplayAL

	mov	eax, [esp + 4]
	shr	eax, 16
	call	DisplayAL

	mov	eax, [esp + 4]
	shr	eax, 8
	call	DisplayAL

	mov	eax, [esp + 4]
	call	DisplayAL

	mov ah, 0x0E
	mov al, ' '
	push	edi
	mov	edi, [displayPosition]
	mov	[gs:edi], ax
	add	edi, 0x2
	mov	[displayPosition], edi
	pop	edi

	mov ah, 0x0E
	mov al, ' '
	push	edi
	mov	edi, [displayPosition]
	mov	[gs:edi], ax
	add	edi, 0x2
	mov	[displayPosition], edi
	pop	edi

	mov ah, 0x0E
	mov al, ' '
	push	edi
	mov	edi, [displayPosition]
	mov	[gs:edi], ax
	add	edi, 0x2
	mov	[displayPosition], edi
	pop	edi

	mov ah, 0x0E
	mov al, ' '
	push	edi
	mov	edi, [displayPosition]
	mov	[gs:edi], ax
	add	edi, 0x2
	mov	[displayPosition], edi
	pop	edi

	ret

DisplayStringProtectMode:
	push	ebp
	mov	    ebp, esp
	push	ebx
	push	esi
	push	edi
    
	mov	esi, [ebp + 12]	; pszInfo
	mov	edi, [displayPosition]
	mov	ah, byte [ebp + 8]
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
	mov	[displayPosition], edi

	pop	edi
	pop	esi
	pop	ebx
	pop	ebp
	ret

DisplayReturn:
	push	returnSet
    push    returnSet
	call	DisplayStringProtectMode
	add	esp, 8

	ret

; void* MemCopy(void* destination, void* source, int size)
; arguments push on stack from right to left
;
; stack
;     | ---------  caller esp = callee ebp
;     | ---------  set current esp = callee ebp
;     | ---------  push callee save registers
;     | =========  get arguments from stack, 
;     | ---------  arg = size
;	  | ---------  arg = source_addr
;     | ---------  arg = destination_addr
;     |
;     |
;	  |


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







