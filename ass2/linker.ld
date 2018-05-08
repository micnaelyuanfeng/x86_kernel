OUTPUT_FORMAT(binary)
ENTRY(loader_start)

MEMORY
{
	BOOT_REGION (RX) : ORIGIN = 0x7c00, LENGTH = 512
}

SECTIONS
{
	.text :
	{
		*(.loader_section)
		. = 64;
		*(.data)
		. = 128;
		*(.protected_mode_start)
		. = 510;
		SHORT(0xaa55)
	} > BOOT_REGION

}