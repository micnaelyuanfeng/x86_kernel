org 0x100
 
BaseOfStack         	equ		0x100	;stack start
BaseOfKernel			equ		0x8000
OffsetOfKernel			equ		0x0
BaseOfLoaderPhyAddr		equ     (0x9000 << 4)
BaseOfKernelPhyAddr		equ     (0x8000 << 4)
KernelEntryPhyAddr		equ		0x030400 

	jmp LOADER_START

%include "fat12hdr.inc"
%include "pm.inc"
%include "gdt.inc"
%include "vm.inc"

[SECTION .real_mode_region]
[BITS 16] 

LOADER_START:
	mov ax, cs
	mov ds, ax 
	mov es, ax
	mov ss, ax
	mov sp, BaseOfStack

;-------------------- read memory segment information ---------------------------------------
	xor ebx, ebx
	xor eax, eax
	xor edx, edx
	mov di, _memSegInfoBuff

.readSegInfo:
	mov eax, 0x0E820		; predefined = Function Code
	mov ecx, 20       		; bufferSize = return structure size(Descriptor)
	mov edx, 0x0534D4150    ; required by INT15 = Signature = "SMAP"
							; BIOS use to verify if caller need system map info
	int 0x15                ; If INT15 Fail, CF flag will be set
	jc 	ReadMemInfoFail     ; Check CF Bit
	add di, 20
	inc dword [_memCR]
	inc dword [_memSegSize]
	cmp ebx, 0           	; ebx will be set != 0 means keep reading segInfo
							; cmp will set ZF bit when equal and clear not equal   
	jne .readSegInfo        ; jne = load EIP by result of preceding cmp
	jmp ReadMemeInfoDone

ReadMemInfoFail:
	mov dword [_memCR], 0
ReadMemeInfoDone:
	;Since BIOS INT does not work in PROTECTION MODE
	;MEMORY INFO must be set up before goto PROTECTION
	;MODE

	mov dh, 0
	call DisplayString

	xor ah, ah
 	xor dl, dl
 	int 0x13

	mov word [wSectorNo], SectNoOfRootDir
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
	;Load to buffer @ ES:BX and compare 
	mov si, KernelFileName
	mov di, OffsetOfKernel
	cld                    		; set direction SI register

	;each sector has 16 enties
	mov dx, 0x10

SEARCH_KERNEL_BIN_IN_SECTOR:
	cmp dx, 0 				;check if All entries in RootDir has been scanned
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
	jmp SEARCH_KERNEL_BIN_IN_SECTOR

CHECK_NEXT_SECTOR:
	add word [wSectorNo], 1     ;update sector number to get floppy access paramters
	jmp SEARCH_KERNEL_BIN_IN_DIR

KERNEL_FOUND:
	;mov  dh, 1
	;call DisplayString
	;jmp $
	jmp  LOAD_KERNEL_START
 
KERNEL_NOT_EXIST:
	mov  dh, 2
	call DisplayString
	jmp  $

LOAD_KERNEL_START:
	mov  ax, RootDirSectors
	and  di, 0xFFE0
	add  di, 0x1A
	mov  cx, word [es:di]

	push cx
	add  cx, ax
	add  cx, DeltaSectorNo
	mov  ax, BaseOfKernel
	mov  es, ax
	mov  bx, OffsetOfKernel
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
	call DisplayString

	;set VGA Mode
	;has to be done before protect mode
	;32 bit addressing and BIOS Int can
	;not use, because of different addressing
	;mode
	mov al, 0x3
	mov ah, 0x00 
	int 0x10

;---------------------------------- Enable Protect Mode --------------------------------------
	;load GDT Table
	;xor eax, eax
	;mov ax, ds               ; ds = ax = 0x9000
	;shl eax, 4			     ; ax = 0x9000 << 4
	;add eax, GDT_TABLE_START ; ax = ax + GDT compiled address offset
	;mov dword [gdtPtr + 2], eax ;write to the memory

	lgdt [gdtPtr]

	cli

	in al, 0x92
	or al, 0b010
	out 0x92, al

	;endbale protect mode
	xor eax, eax
	mov eax, cr0
	or  eax, 0x1
	mov cr0, eax

	;Since protect mode is on
	;addressing is all done by IP or mem
	;segment register is just an index of GDT
	;=> the value in register/mem is the real physical address
	;of code/data = 32 bit register addressing
	;jmp  BaseOfLoader:OffsetOfLoader
	jmp	dword selectorCode:(BaseOfLoaderPhyAddr + PROTECT_MDOE_START)

GET_FAT_ENTRY:
	push es
	push bx
	push ax

	mov	ax, BaseOfKernel
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
	
KillMotor:
	push	dx
	mov	dx, 03F2h
	mov	al, 0
	out	dx, al
	pop	dx
	ret

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

;Data Part
KernelFileName			db	"KERNEL  BIN", 0
MessageLength			equ	9
LoadMessage:			db	"Loading  " 
Message1                db  "Ready.   "
Message2                db  "No Kernel"

wRootDirSizeForLoop  	dw	RootDirSectors
wSectorNo	       		dw	0 			; initial value is 0
bOdd		 			db	0
dwKernelSize			dd	0


;----------------------------------------------------------------------------------------------
;--------------------------------- End of Real Mode -------------------------------------------
;----------------------------------------------------------------------------------------------

[SECTION .protect_mode_region]
[BITS 32]

%include "lib.inc"
%include "dataRing0Boot.inc"

PROTECT_MDOE_START:
 
	mov ax, selectorData
	mov ds, ax 
    mov ss, ax
    mov es, ax
    mov fs, ax
    
	mov ax, selectorVideo
	mov gs, ax

	mov esp, TopOfStackRing0
	
	;push memSegTitle
	;push dword [offsetColorCyan]
	;call DisplayStringProtectMode
	;add esp, 8

	;call displayMemSegInfo

	;call DisplayReturn
	
	call MigrateKernel

	mov edx, memSegInfoBuff
	mov ebx, dword [memCR]

	jmp dword selectorCode:KernelEntryPhyAddr
	;jmp $

displayMemSegInfo:
	push esi        ; easy convention = push all regs will use localy and
	push edi        ; pop before return
	push ecx

	mov esi, memSegInfoBuff ;offset to data segment
	mov ecx, [memCR]

.readGoOn:              ;read an entry which is 5 elementss
	mov edx, 0x5
	mov edi, memSegDesc ;memory address

.1:
	push dword [esi]    ;push 4 bytes on stack
	call DisplayIntegar
	pop  eax			;retrive initial address of segInfo buffer

	stosd               ;store eax value to ES:DI
						;copy values from buffer to var
    add esi, 0x4		;move down one dword		
	dec edx				;decrease the counter by 1
	cmp edx, 0x0
	jnz .1

	call DisplayReturn  ;this ADR display done

	cmp  dword [segType], 1 ; check this seg type
	jne  .2 

	mov eax, [segBaseAddrLo]
	add eax, [segLenLo]
	cmp eax, [memSize]
	jb .2
	mov [memSize], eax
.2:
	dec ecx
	cmp ecx, 0
	jne .readGoOn
	;jmp .readGoOn

Done:
	call DisplayReturn
 
	push ramSizeTitle 
	push dword [offsetColorCyan] 
	call DisplayStringProtectMode
	add  esp, 8

	push dword [memSize] ; convention
	call DisplayIntegar
	add  esp, 4

	pop ecx
	pop edi
	pop esi
 
	ret

MigrateKernel:
	push esi
	push ecx
	push eax
 
	xor esi, esi
	mov cx, word [BaseOfKernelPhyAddr + 0x2c] ;
	movzx ecx, cx

	mov esi, [BaseOfKernelPhyAddr + 0x1c]
	add esi, BaseOfKernelPhyAddr

Migratestart:
	mov eax, [esi + 0]
	cmp eax, 0x0
	jz  MigrationDone

	push dword [esi + 0x10]
	mov  eax, [esi + 0x4]

	add  eax, BaseOfKernelPhyAddr
	push eax
	push dword [esi + 0x8]
	call MemCopy
	add esp, 0xC

MigrationDone:
	add esi, 0x20
	dec ecx
	jnz Migratestart

	pop eax
	pop ecx
	pop esi

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