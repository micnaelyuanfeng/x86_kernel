
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

DisplayString:
    mov	ax, MessageLength
	mul	dh
	add	ax, LoadMessage
	mov	bp, ax
	mov	ax, ds
	mov	es, ax
	mov	cx, MessageLength	
	mov	ax, 0x01301
	mov	bx, 0x7
	mov	dl, 0x0
	add	dh, 0x3
	
	int	0x10
	
	ret
