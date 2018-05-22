org  0x100

BaseOfStack			equ	 0x100

BaseOfKernelFile	equ	 0x8000	
OffsetOfKernelFile	equ	 0x0	


	jmp	LOADER_START		; Start

%include	"fat12hdr.inc"
%include    "pm.inc"


LOADER_START:		
	; use INT13.0 BIOS to reset floppy drive
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, BaseOfStack
	
	;clear screen
	mov ax, 0x0600
	mov bx, 0x0700
	mov cx, 0x0
	mov dx, 0x0184F
	int 0x10

	;reset cursor
	mov ah, 0x2
	mov dx, 0x0
	int 0x10

	mov  dh, 0
	call DISPLAY_MESSAGE

 	xor ah, ah
 	xor dl, dl
 	int 0x13

	;set up GDT
	lgdt [gdtPtr]

	cli 

	

	mov	word [wSectorNo], SectorNoOfRootDirectory	
SEARCH_KERNEL_BIN_IN_DIR:
	cmp word [wRootDirSizeForLoop],	0
	jz  KERNEL_NOT_EXIST     	;if check all entries in RootDir
	dec word [wRootDirSizeForLoop]

	;load 1 sector into Memory
	mov ax, BaseOfKernelFile
	mov es, ax
	mov bx, OffsetOfKernelFile
	mov ax, [wSectorNo]
	mov cl, 1
    
    call READ_FROM_MEMORY_A_SECTOR
	;Load to buffer @ ES:BX and compare 
	mov si, KernelFileName
	mov di, OffsetOfKernelFile
	cld                    		; set direction SI register

	;each sector has 16 enties
	mov dx, 0x10
SEARCH_KERENL_BIN_IN_SECTOR:
	cmp dx, 0 					;check if All entries in RootDir has been scanned
	jz  CHECK_NEXT_SECTOR
	dec dx
	mov cx, 0xB 				; fill name is 11 characters

CHECK_FILE_NAME:
	cmp cx, 0
	jz  KERNEL_FOUND
	dec cx
	
	lodsb 						;update SI register
	cmp al, byte [es:di]
	jz  CHECK_NEXT_CHAR_IN_FILENAME
	jmp CHECK_NEXT_ENTRY

;update di to next character
CHECK_NEXT_CHAR_IN_FILENAME:
	inc di 						;update di to do next char comaprision
	jmp CHECK_FILE_NAME

;update di to next entry
CHECK_NEXT_ENTRY:
	and di, 0xFFE0
	add di, 0x20
	mov si, KernelFileName
	jmp SEARCH_KERENL_BIN_IN_SECTOR

CHECK_NEXT_SECTOR:
	add word [wSectorNo], 1     ;update sector number to get floppy access paramters
	jmp SEARCH_KERNEL_BIN_IN_DIR

KERNEL_NOT_EXIST:
	mov  dh, 2
	call DISPLAY_MESSAGE
	jmp  $

KERNEL_FOUND:			
	mov	ax, RootDirSectors
	and	di, 0FFF0h		

	;save kernel size
	push eax
	mov	 eax, [es : di + 0x01C]		
	mov	 dword [dwKernelSize], eax	 
	pop	 eax

	add	di, 01Ah		
	mov	cx, word [es:di]
	push	cx			
	add	cx, ax
	add	cx, DeltaSectorNo	
	mov	ax, BaseOfKernelFile
	mov	es, ax			
	mov	bx, OffsetOfKernelFile
	mov	ax, cx			

LOAD_KRNL_GO_ON:
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

	jz   LABEL_KRNL_FILE_LOADED

	; update Target Sector 
	push ax
	mov  dx, RootDirSectors
	add  ax, dx
	add  ax, DeltaSectorNo
	add	 bx, [BPB_BytsPerSec]
	jmp  LOAD_KRNL_GO_ON

LABEL_KRNL_FILE_LOADED:

	call KillMotor	

	mov	 dh, 1			
	call DISPLAY_MESSAGE			

	jmp	$

READ_FROM_MEMORY_A_SECTOR: 		;parameter is sector number => floppy reading arguments and call BIOS INT13
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

	mov	ax, BaseOfKernelFile
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
	mov	 bx, [BPB_BytsPerSec]
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

KillMotor:
	push	dx
	mov	dx, 03F2h
	mov	al, 0
	out	dx, al
	pop	dx
	ret

DISPLAY_MESSAGE:
	mov ax, MessageLength
	mul dh  				; multiply ax by dh = select msg element in array
	add ax, LoadMessage 	; move to the last char of string
	mov bp, ax
	mov ax, ds 				; extra segment
	mov es, ax 				; set text starting point ES:BP
	mov cx, MessageLength 	; set loop counter
	mov ax, 0x1301 			; INT subvector number
	mov bx, 0x07

	;reset cursor position
	mov dl, 0x0
	;add dh, 0x03
	int 0x10

	ret

wRootDirSizeForLoop		dw	RootDirSectors
wSectorNo				dw	0		
bOdd					db	0		
dwKernelSize			dd	0		


KernelFileName			db	"KERNEL  BIN", 0	
MessageLength			equ	9
LoadMessage:			db	"Loading  "
Message1				db	"Ready.   "
Message2				db	"No KERNEL"
Message3                db  "Protect M"
Message4				db  "In Ring 0"
Message5                db  "In Ring 3"

[SECTION .s32]
ALIGN 32
[BITS 32]

pm_start:
	