ENTRY(_start)

SECTIONS
{
	. = 1M;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(.COMMON)
		*(.bss)
	}
	.stab BLOCK(4K) : ALIGN(4K)
	{
		*(.stab)
	}
	.stabstr BLOCK(4K) : ALIGN(4K)
	{
		*(.stabstr)
	}
}