[SECTION .gdt]

GDT_DEFAULT:    Descriptor 0,             0,                 0
GDT_CODE:       Descriptor 0,             SegCode32Len-1,    DA_C|DA_32
GDT_DATA:       Descriptor 0,             SegData32Len-1,    DA_DRW
GDT_VIDEO:      Descriptor 0xb8000,       0xFFFF,            DA_DRW

CALL_GATE:      Gate       selectorCodeTest, 0, 0, DA_386CGate + DA_DPL0


gdtLen          equ     $ - GDT_DEFAULT
gdtPtr          dw      gdtLen - 1
                ;dd      BaseOfLoader * 0x10 + GDT_DEFAULT
                dd      0 ;offset

selectorCode    equ GDT_CODE  - GDT_DEFAULT
selectorData    equ GDT_DATA  - GDT_DEFAULT
selectorVideo   equ GDT_VIDEO - GDT_DEFAULT