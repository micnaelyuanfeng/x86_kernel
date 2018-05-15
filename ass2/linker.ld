OUTPUT_FORMAT(binary)
ENTRY(kstart)

MEMORY 
{
    boot_region (RX) : ORIGIN = 0x00, LENGTH = 2M
}

SECTIONS 
{
     .text : 
     {
        *(.s16)

        *(.s32)
        
        *(.text)

        *(.*data*)

     } >boot_region

     /DISCARD/ : 
     {
           *(*)
     }
}