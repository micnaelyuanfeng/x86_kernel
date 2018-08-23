xor eax, eax

mov eax, loaderBase
shl eax, 4
or  eax, loaderOffset ;get absolute address

shr eax, 12
and eax, 0x3FF    ;get PTE index

;PageTableBase[(loaderBase >> 12) & 0x3FF]
mov edi, PageTableBase
add edi, eax

mov eax, loaderBase
shl eax, 4
shl eax, 12
or  eax, PG_P | USS | RWW

mov ecx, 1024

.2:
  mov dword [es:edi], eax
  add eax, 4096 ;0x1000
  add edi, 4
  .loop .2
