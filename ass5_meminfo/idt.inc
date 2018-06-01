[SECTION .idt]
ALIGN 32
[BITS 32]

IDT_TABLE:

%rep 128
	Gate	selectorCode, genericIntHandler, 0, DA_386IGate 
%endrep

.0x80:		Gate	selectorCode, genericIntHandler, 0, DA_386IGate


idtLen			equ 		$ - IDT_TABLE
idtPtr          dw			idtLen - 1
				dd          0x0