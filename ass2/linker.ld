
ENTRY(loader_start)

MEMORY 
{
    boot_region (RX) : ORIGIN = 0x00, LENGTH = 1024
}

SECTIONS 
{
     .text : 
     {
        *(.START16)
        
        *(.data)

        *(.START32)
        *(.text)

        
     }
}