ENTRY(_kstart)

MEMORY
{
	KRNL_IMG(RWX) : ORIGIN = 0x30400, LENGTH = 0xFFFFF
}

SECTIONS
{
	.text :
	{
		*(.text)
		*(.isr_entry)
		*(.irq_entry)
	}

	.data :
	{
		*(.data)
		*(.gdt)
	}

	.bss :
	{
		*(.bss)
	}
}
