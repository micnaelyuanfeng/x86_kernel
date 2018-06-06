BaseOfStack				equ		0x100
BaseOfLoaderPhyAddr		equ		(0x9000 << 4)

org	0x100
	jmp REAL_MODE_START

[SECTION .real_mode_region]
[BITS 16]

;boot.asm load loader @ cs = 0x9000 and off = 0x100
;starting address of loader = 0x9000 << 4 + 0x100
;loader needs org 0x100 to get correct compiled address 
;for instruction and code.
;
;loader starts execting @ cs = 0x9000 << 4 + org 0x100
;data access  = ds << 4 + offset
;code access  = cs << 4 + offset
;stack access = ss << 4 + sp
;
;To get correct address,
;need to update all segment registers
;Since loader.S is compiled sequentially, not elf format =>
;all sections are sequentially labelled (assign address)
;

REAL_MODE_START:
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

	;debug print
	mov ah, 0x13
	mov al, 0x1
	mov bh, 0x0
	mov bl, 0xb
	mov cx, lengthDebugMsg0
	mov dh, 0x0
	mov dl, 0x0
	mov bp, debugMsg0
	int 0x10

	;read memory segment information
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

	;load GDT Table
	xor eax, eax
	mov ax, ds               ; ds = ax = 0x9000
	shl eax, 4			     ; ax = 0x9000 << 4
	add eax, GDT_TABLE_START ; ax = ax + GDT compiled address offset
	mov dword [gdtPtr + 2], eax ;write to the memory

	lgdt [gdtPtr]

	cli
	;endbale protect mode
	xor eax, eax
	mov eax, cr0
	or  eax, 0x1
	mov cr0, eax

	;Since protect mode is on
	;addressing is all done by IP or mem
	;segment register is just an index of GDT
	;=> the value in register/mem is the real physical address
	;of code/data
	jmp	dword selectorCode:(BaseOfLoaderPhyAddr + PROTECT_MDOE_START)

;----------------------- End of Real Mode -------------------------------------------

%include "dataRing0Boot.inc"
%include "gdt.inc"

;All Gate and GDT starting address is defined as
;In Protect Mode, addressing mode is changed => GDT[selector].baseAddr + offset
;Current segment defined in GDT, all base address is 0x0 start => all var addresses
;should be BaseOfLoaderPhyAddr + compiled address offset. Otherwise, cannot get corrent
;var address.
;After entering protect mode, all vars required to be accessed, all symbols need to be redefined

[SECTION .protect_mode_region]
[BITS 32]

PROTECT_MDOE_START:
	mov ax, selectorData
	mov ds, ax 

	mov ax, seletorVideo
	mov gs, ax

	mov esp, TopOfStackRing0

	call displayMemSegInfo