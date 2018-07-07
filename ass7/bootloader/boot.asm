org 0x7c00 								;pseduo code for linker
	; BPB header 
	; This is formating process which is to add this header
	; in boot section 
	jmp BOOT_STRAP_START 
	nop 
	BS_OEMName			DB		'BCFYOS01' ;Must be 8 bytes
	BPB_BytePerSec		DW		512
	BPB_SecPerClus		DB		1
	BPB_RsvdSecCnt		DW		1
	BPB_NumFATs			DB		2
	BPB_RootEntCnt		DW		224
	BPB_TotSec16		DW		2880
	BPB_Media			DB		0xF0
	BPB_FATSz16			DW		9
	BPB_SecPerTrk		DW		18
	BPB_NumHeads		DW		2
	BPB_HiddSec			DD		0
	BPB_TotSec32		DD		0
	BS_DrvNum			DB		0
	BS_Reserved1		DB		0
	BS_BootSig			DB		0x29
	BS_VolID			DD 		0
	BS_VolLab			DB		'Kernel_impl'
	BS_FileSysType		DB		'FAT12'

;Define and Variables
BaseOfLoader		 	equ		0x9000
OffsetOfLoader       	equ		0x100   	; predefined local memory address for sector
RootDirSectors		 	equ		14 			; changable but usually fixed
SectNoOfRootDir	     	equ		19      	; default RootDirSectors start sector number
SectorNoOfFAT1       	equ		1       	; sector number of FAT1 

wRootDirSizeForLoop  	dw	    RootDirSectors
wSectorNo			 	dw		0 			; initial value is 0
bOdd				 	db		0

LoaderFileName		 	db		"LOADER  BIN", 0

MessageLength		 	equ		9
BootMessage:		 	db		"Booting  "
Message1			 	db     	"Ready    "
Message2             	db     	"No Loader"

BaseOfStack			 	equ    	0x7c00
DeltaSectorNo        	equ    	17          ; sector offset to address loading area

;video_mode              equ     0xff2
;video_x_range           equ     0xff4
;video_y_range           equ     0xff6
;video_buffer            equ     0xff8
;                 
;                 |  Load Area            |
;                 ------------------------ sector x = dynamic and MAX = 14 = 33
;                |   RootDIR table       |
;                ------------------------ sector 19
;               |   FAT1 and 2          |
;               ------------------------ sector 1
;              |    512 Bytes          |
;              ------------------------  sector 0
; 
; FAT only store sector number offset to the FAT section
; e.g. FAT_ENTRY = 0x3 => 0x3 - 2 = 0x1 only offset 
;      and real offset = 33 + offset 

BOOT_STRAP_START:
RESET_FLOPPY:
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

	mov  dh, 0
	call DISPLAY_MESSAGE

	;mov byte [video_mode], 8
	;mov word [video_x_range], 320
	;mov word [video_y_range], 200 
	;mov dword [video_buffer], 0xa0000

 	xor ah, ah
 	xor dl, dl
 	int 0x13

 	mov word [wSectorNo], SectNoOfRootDir
SEARCH_LOADER_BIN_IN_DIR:
	cmp word [wRootDirSizeForLoop],	0
	jz  LOADER_NOT_EXIST     	;if check all entries in RootDir
	dec word [wRootDirSizeForLoop]

	;load 1 sector into Memory
	mov ax, BaseOfLoader
	mov es, ax
	mov bx, OffsetOfLoader
	mov ax, [wSectorNo]
	mov cl, 1
    
    	call READ_FROM_MEMORY_A_SECTOR
	;Load to buffer @ ES:BX and compare 
	mov si, LoaderFileName
	mov di, OffsetOfLoader
	cld                    		; set direction SI register

	;each sector has 16 enties
	mov dx, 0x10
SEARCH_LOADER_BIN_IN_SECTOR:
	cmp dx, 0 					;check if All entries in RootDir has been scanned
	jz  CHECK_NEXT_SECTOR
	dec dx
	mov cx, 0xB 				; fill name is 11 characters

CHECK_FILE_NAME:
	cmp cx, 0
	jz  LOADER_FOUND
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
	mov si, LoaderFileName
	jmp SEARCH_LOADER_BIN_IN_SECTOR

CHECK_NEXT_SECTOR:
	add word [wSectorNo], 1     ;update sector number to get floppy access paramters
	jmp SEARCH_LOADER_BIN_IN_DIR

LOADER_FOUND:
	jmp  LOAD_LOADER_START

LOADER_NOT_EXIST:
	mov  dh, 2
	call DISPLAY_MESSAGE
	jmp  $

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

LOAD_LOADER_START:
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

LOAD_LOADER_GO_ON:
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

	jz   LOADING_DONE

	; update Target Sector 
	push ax
	mov  dx, RootDirSectors
	add  ax, dx
	add  ax, DeltaSectorNo
	add	 bx, [BPB_BytePerSec]
	jmp  LOAD_LOADER_GO_ON


LOADING_DONE:
	mov  dh, 1
	call DISPLAY_MESSAGE
	jmp  BaseOfLoader:OffsetOfLoader

DISPLAY_MESSAGE:
	mov ax, MessageLength
	mul dh  				; multiply ax by dh = select msg element in array
	add ax, BootMessage 	; move to the last char of string
	mov bp, ax
	mov ax, ds 				; extra segment
	mov es, ax 				; set text starting point ES:BP
	mov cx, MessageLength 	; set loop counter
	mov ax, 0x1301 			; INT subvector number
	mov bx, 0x07

	;reset cursor position
	mov dl, 0
	int 0x10
	ret 					; ret and call pair

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

times 			510 - ($ - $$) db 0
				       dw 0xaa55
							 