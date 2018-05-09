OUTPUT_FORMAT(binary)
ENTRY(_start)

MEMORY 
{
    boot_region (RX) : ORIGIN = 0x7c00, LENGTH = 512
}

SECTIONS 
{
     .text : 
     {
        *(.START16)

        . = 64;
        
        *(.data*)

        . = 128;

        *(.START32)
        
        . = 510;
        
         SHORT(0xaa55)
     } >boot_region

     /DISCARD/ : 
     {
           *(*)
     }
}