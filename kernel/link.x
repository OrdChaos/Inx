ENTRY(kernel_main)
OUTPUT_FORMAT(elf64-x86-64)

SECTIONS
{
    . = 0x100000;
    _kernel_begin = .;
	.text : ALIGN(0x1000)
	{
		*(.text)
	}
	.data : ALIGN(0x1000)
	{
		*(.data)
	}
	.rodata : ALIGN(0x1000)
	{
		*(.rodata)
	}
	.bss : ALIGN(0x1000)
	{
		*(COMMON)
		*(.bss)
	}
	_kernel_end = .;
}