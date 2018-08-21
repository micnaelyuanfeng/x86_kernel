[SECTION .gdt]
GDT_TABLE_START:

GDT_DEFAULT:	Descriptor			0,			0,					0		
GDT_CODE_SYS:   Descriptor     		0,	  		0xFFFFFFFF,			DA_CR|DA_32|DA_LIMIT_4K 
GDT_DATA_SYS:	Descriptor			0,	  		0xFFFFFFFF,			DA_DRW|DA_32|DA_LIMIT_4K ;Data Segment Read and Write
GDT_VIDEO:		Descriptor	  		0xB8000,    0xFFFF,          	DA_DRW|DA_DPL3 ;Data Segment Read and Write
GDT_PAGE_DIR:	Descriptor          PageDirBase, 4095,				DA_DRW         ;System Data Segment
GDT_PAGE_TBL:   Descriptor			PageTblBase, 4096 * 8 - 1,      DA_DRW		   ;System Data Segment

selectorCode		equ		GDT_CODE_SYS  - GDT_DEFAULT
selectorData		equ     GDT_DATA_SYS  - GDT_DEFAULT
selectorVideo       equ     GDT_VIDEO     - GDT_DEFAULT   + SA_RPL3
selectorPageDir		equ		GDT_PAGE_DIR  - GDT_DEFAULT
selectorPageTbl	    equ 	GDT_PAGE_TBL  - GDT_DEFAULT

gdtLen			equ		$ - GDT_TABLE_START
gdtPtr			dw		gdtLen - 1 ;size
				dd      BaseOfLoaderPhyAddr + GDT_DEFAULT

;------------------------ GDT SECTION ---------------------------