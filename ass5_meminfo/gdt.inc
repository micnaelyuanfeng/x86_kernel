[SECTION .gdt]
[BITS 32]
GDT_DEFAULT:    Descriptor 0,             0,                 0
GDT_CODE:       Descriptor 0,             SegCode32Len-1,    DA_C|DA_32
GDT_DATA:       Descriptor 0,             SegData32Len-1,    DA_DRW
GDT_CODE_TEST:  Descriptor 0,             SegCode32LenTest-1, DA_C|DA_32
GDT_VIDEO:      Descriptor 0xb8000,       0xFFFF,            DA_DRW|DA_DPL3
GDT_CODE_RING3: Descriptor 0, 			  SegCodeRing3Len-1, DA_C|DA_32|DA_DPL3
GDT_TSS:		Descriptor 0,             tssLen-1,          DA_386TSS     ;need to be DPL3
GDT_DATA_RING3: Descriptor 0,             SegData32Ring3Len-1, DA_DRW|DA_DPL3

GDT_KSTACK:     Descriptor 0,             TopOfStack,      DA_DRW|DA_32
GDT_STACKRING3: Descriptor 0,             TopOfStackRing3, DA_DRW|DA_32|DA_DPL3


CALL_GATE:      Gate       selectorCodeTest, 0, 0, DA_386CGate + DA_DPL3


gdtLen          	equ     $ - GDT_DEFAULT
gdtPtr          	dw      gdtLen - 1
                	;dd      BaseOfLoader * 0x10 + GDT_DEFAULT
                	dd      0 ;offset

selectorCode    	equ GDT_CODE  - GDT_DEFAULT
selectorData    	equ GDT_DATA  - GDT_DEFAULT
selectorVideo   	equ GDT_VIDEO - GDT_DEFAULT
selectorCodeTest	equ GDT_CODE_TEST - GDT_DEFAULT
selectorCallGate    equ CALL_GATE - GDT_DEFAULT
selectorTSS			equ GDT_TSS   - GDT_DEFAULT


selectorCodeRing3   equ GDT_CODE_RING3 - GDT_DEFAULT + SA_RPL3
selectorDataRing3   equ GDT_DATA_RING3 - GDT_DEFAULT + SA_RPL3
selectorKstack      equ GDT_KSTACK - GDT_DEFAULT
selectorStackRing3  equ GDT_STACKRING3 - GDT_DEFAULT + SA_RPL3
