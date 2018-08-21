ENTRY(_kstart)

MEMORY
{
	KRNL_IMG(RWX) : ORIGIN = 0xC0040000, LENGTH = 0xFFFFFFFF
}

SECTIONS
{
	. = 0xC0040000;

	.text :
	{
		*(.text)
		*(.isr_entry)
		*(.irq_entry)
	}>KRNL_IMG

	.data :
	{
		*(.data)
		*(.gdt)
	}>KRNL_IMG

	.bss :
	{
		*(.bss)
	}>KRNL_IMG
} 
