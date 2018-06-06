[SECTION .data_section_ring0]
ALIGN 32
[BITS 32]

;memory information
_memSegTitle                db      "BaseAddrLo    BaseAddrHi    LengthLo      LengthHi      Type", 0x0A, 0
_ramSize                    db      "RAM Size: ", 0
_stRetValue                 db      0x0A, 0x0

_memCR                      dd      0
_displayPosition            dd      (80 * 7 + 0) * 2
_memSize                    dd      0
_memSegDesc:
    _segBaseAddrLo          dd      0
    _segBaseAddrHi          dd      0
    _segLenLo               dd      0
    _segLenHi               dd      0
    _segType                dd      0

_memSegInfoBuff:
    times   256             db      0

;memory symbol section
memSegTitle                 equ     BaseOfLoaderPhyAddr + _memSegTitle
ramSizeTitle                equ     BaseOfLoaderPhyAddr + _ramSize
displayPosition             equ     BaseOfLoaderPhyAddr + _displayPosition
returnSet                   equ     BaseOfLoaderPhyAddr + _stRetValue
memCR                       equ     BaseOfLoaderPhyAddr + _memCR          
memSize                     equ     BaseOfLoaderPhyAddr + _memSize         
memSegDesc                  equ     BaseOfLoaderPhyAddr + _memSegDesc      
    segBaseAddrLo           equ     BaseOfLoaderPhyAddr + _segBaseAddrLo   
    segBaseAddrHi           equ     BaseOfLoaderPhyAddr + _segBaseAddrHi   
    segLenLo                equ     BaseOfLoaderPhyAddr + _segLenLo        
    segLenHi                equ     BaseOfLoaderPhyAddr + _segLenHi        
    segType                 equ     BaseOfLoaderPhyAddr + _segType         

memSegInfoBuff              equ     BaseOfLoaderPhyAddr + _memSegInfoBuff

;Debug Information
debugMsg0                   db      "In Real Mode", 0
lengthDebugMsg0             equ     BaseOfLoaderPhyAddr + $

degbugMsg1                   db      "In Protect Mode", 0
lengthDebugMsg1             equ     BaseOfLoaderPhyAddr + $

colorCyan                   db      0x3

colorLightCyan              db      0xB
colorLightBlue              db      0x9

;Stack Ring0 Information
[SECTION .stack_region_ring0]
ALIGN 32
[BITS 32]
STACK_RING0_START:
    times           1024    db  0
TopOfStackRing0             equ BaseOfLoaderPhyAddr + $

[SECTION .tss_region]
ALIGN 32
[BITs 32]
TSS_START:
    dd      0
    dd      TopOfStackRing0
    dd      selectorCode
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dd      0
    dw      0
    dw      $ - TSS_START + 2 ; I/O
    db      0x0FF             ; I/O 
tssLen equ $ - TSS_START
