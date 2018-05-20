org 0x100

BaseOfStack         equ     0x100
BaseOfKernel        equ     0x8000
OffsetOfKernel      equ     0x0

    jmp LABEL_START

%include "fat12hdr.inc"     ;formating

LABEL_START:
    mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, BaseOfStack

	mov	dh, 0			; "Loading  "
	call	DispStr			

	mov	word [wSectorNo], SectorNoOfRootDirectory	

    ;reset floppy drive
	xor	ah, ah	
	xor	dl, dl	
	int	13h

SEARCH_KERNEL_BIN_IN_DIR:
	cmp word [wRootDirSizeForLoop],	0
	jz  KERNEL_NOT_EXIST     	;if check all entries in RootDir
	dec word [wRootDirSizeForLoop]

	;load 1 sector into Memory
	mov ax, BaseOfKernel
	mov es, ax
	mov bx, OffsetOfKernel
	mov ax, [wSectorNo]
	mov cl, 1
    
    call READ_FROM_MEMORY_A_SECTOR

    mov si, KernelFileName
    mov di, OffsetOfKernel
    cld

    ;each sector has 16 enties
	mov dx, 0x10
SEARCH_KERNEL_BIN_IN_SECTOR:
	cmp dx, 0 					;check if All entries in RootDir has been scanned
	jz  CHECK_NEXT_SECTOR_KERNEL
	dec dx
	mov cx, 0xB 				; fill name is 11 characters

CHECK_KERNEL_FILE_NAME:
	cmp cx, 0
	jz  LOADER_FOUND
	dec cx
	
	lodsb ;update SI register
	cmp al, [es:di]
	jz  CHECK_NEXT_CHAR_IN_KERNELFILENAME
	jmp CHECK_NEXT_ENTRY_KERNEL

;update di to next character
CHECK_NEXT_CHAR_IN_KERNELFILENAME:
	inc di 						;update di to do next char comaprision
	jmp CHECK_KERNEL_FILE_NAME

;update di to next entry
CHECK_NEXT_ENTRY_KERNEL:
	and di, 0xFFE0
	add di, 0x20
	mov si, LoaderFileName
	jmp SEARCH_KERNEL_BIN_IN_SECTOR

CHECK_NEXT_SECTOR_KERNEL:
	add word [wSectorNo], 1      ;update sector number to get floppy access paramters
	jmp SEARCH_KERNEL_BIN_IN_DIR  

READ_FROM_MEMORY_A_SECTOR: ;parameter is sector number => floppy reading arguments and call BIOS INT13
	;calculatr parameters to read
	push bp,
	mov  bp, sp
	sub  sp, 2

	mov byte [bp - 2], cl
	push bx

	mov bl, [BPB_SecPerTrk]
	div bl
	inc ah

	mov cl, ah
	mov dh, al
	shr al, 1
	mov ch, al
	and dh, 1

	pop bx
	mov dl, [BS_DrvNum]

 .GoOnReading:
 	mov ah, 2
 	mov al, byte [bp - 2]
 	int 0x13
 	jc  .GoOnReading

 	add sp, 2
 	pop bp

 	ret

GET_FAT_ENTRY:
	push es
	push bx
	push ax

	mov	ax, BaseOfLoader
	sub	ax, 0x0100	;  
	mov	es, ax		; 
	pop	ax
	mov	byte [bOdd], 0
	mov	bx, 3
	mul	bx			; dx:ax = ax * 3
	mov	bx, 2
	div	bx			; dx:ax / 2  
	cmp	dx, 0
	jz	LABEL_EVEN
	mov	byte [bOdd], 1

LABEL_EVEN:
	xor	 dx, dx			
	mov	 bx, [BPB_BytePerSec]
	div	 bx 
	push dx
	mov	 bx, 0 
	add	 ax, SectorNoOfFAT1 
	mov	 cl, 2
	call READ_FROM_MEMORY_A_SECTOR

	pop	dx
	add	bx, dx
	mov	ax, [es:bx]
	cmp	byte [bOdd], 1
	jnz	LABEL_EVEN_2
	shr	ax, 4
LABEL_EVEN_2:
	and	ax, 0x0FFF

LABEL_GET_FAT_ENRY_OK:

	pop	bx
	pop	es
	ret

KERNEL_NOT_EXIST:
    mov	 dh, 2
	call DispStr		

	jmp	$			

KERNEL_FILE_FOUND:

    mov  ax, RootDirSectors
	and  di, 0xFFE0
	add  di, 0x1A
	mov  cx, word [es:di]

	push cx
	add  cx, ax
	add  cx, DeltaSectorNo
	mov  ax, BaseOfLoader
	mov  es, ax
	mov  bx, OffsetOfLoader
	mov  ax, cx

LOAD_KERNEL_GO_ON:
	push ax			
	push bx			
	mov	 ah, 0x0E			
	mov	 al, '.'			
	mov	 bl, 0x0F			
	int	 0x10			
	pop	 bx			
	pop	 ax			
	mov  cl, 1

	call READ_FROM_MEMORY_A_SECTOR
	pop  ax

	call GET_FAT_ENTRY
	cmp  ax, 0x0FFF            ; FAT_ENTRY = 0xFFF means last FAT ENTRY and stop

	jz   KERNEL_FILE_LOAD_DONE

	; update Target Sector 
	push ax
	mov  dx, RootDirSectors
	add  ax, dx
	add  ax, DeltaSectorNo
	add	 bx, [BPB_BytePerSec]
	jmp  LOAD_KERNEL_GO_ON

KERNEL_FILE_LOAD_DONE:
    call KillMotor

	mov	 dh, 1			
	call DispStr		

	jmp	$

wRootDirSizeForLoop     	dw	    RootDirSectors	
wSectorNo		            dw	    0		
bOdd			            db  	0		
dwKernelSize		        dd	    0		


KernelFileName		        db  	"KERNEL  BIN", 0	

MessageLength		        equ	    9
LoadMessage:		        db	    "Loading  "
Message1		            db	    "Ready.   "
Message2		            db	    "No KERNEL"