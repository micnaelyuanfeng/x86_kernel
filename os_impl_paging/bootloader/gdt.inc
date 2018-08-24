[SECTION .gdt]
GDT_TABLE_START:

GDT_DEFAULT:	Descriptor			0,			0,					0		
GDT_CODE_SYS:   Descriptor     		0,	  		0xFFFFFFFF,			DA_CR|DA_32|DA_LIMIT_4K 
GDT_DATA_SYS:	Descriptor			0,	  		0xFFFFFFFF,			DA_DRW|DA_32|DA_LIMIT_4K ;Data Segment Read and Write
GDT_VIDEO:		Descriptor	  		0xB8000,    0xFFFF,          	DA_DRW|DA_DPL3 ;Data Segment Read and Write

selectorCode		equ		GDT_CODE_SYS  - GDT_DEFAULT
selectorData		equ     GDT_DATA_SYS  - GDT_DEFAULT
selectorVideo       equ     GDT_VIDEO     - GDT_DEFAULT   + SA_RPL3

gdtLen				equ		$ - GDT_TABLE_START
gdtPtr				dw		gdtLen - 1 ;size
					dd      BaseOfLoaderPhyAddr + GDT_DEFAULT

;------------------------ GDT SECTION ---------------------------