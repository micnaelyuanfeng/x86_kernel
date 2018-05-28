MEM_INFO_BLK:  

_memSegTitle                db      "BaseAddrLo  BaseAddrHi  LengthLo LengthHi  Type", 0x0A, 0
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

;symbol section
memSegTitle                 equ     _memSegTitle    - $$
ramSizeTitle                equ     _ramSize        - $$
displayPosition             equ     _displayPosition- $$
memCR                       equ     _memCR          - $$
memSize                     equ     _memSize        - $$
memSegDesc                  equ     _memSegDesc     - $$
    segBaseAddrLo           equ     _segBaseAddrLo  - $$
    segBaseAddrHi           equ     _segBaseAddrHi  - $$
    segLenLo                equ     _segLenLo       - $$
    segLenHi                equ     _segLenHi       - $$
    segType                 equ     _segType        - $$

memSegInfoBuff              equ     _memSegInfoBuff - $$
memBLKDateLen               equ     $ - MEM_INFO_BLK