[SECTION .gdt]
[BITS 32]

GDT_TABLE_START:
GDT_DEFAULT:    Descriptor 0,             0,                 0
GDT_CODE:       Descriptor 0,             0xfffff,    		 DA_C|DA_32|DA_LIMIT_4K
GDT_DATA:       Descriptor 0,             0xfffff,    		 DA_DRW|DA_32|DA_LIMIT_4K
GDT_VIDEO:      Descriptor 0xb8000,       0xFFFF,            DA_DRW|DA_DPL3

selectorCode		equ		GDT_CODE  - GDT_DEFAULT
selectorData		equ     GDT_DATA  - GDT_DEFAULT
selectorVideo       equ     GDT_VIDEO - GDT_VIDEO

gdtLen          	equ     $ - GDT_TABLE_START 
gdtPtr				dw		gdtLen - 1
					dd      BaseOfLoaderPhyAddr + $