#include <kernel.h>
#include <vga.h>

int		kernel_start(void)
{
	init_video();
	putstring("Hello World!\n");
	return (0);
}

int		kernel_main(void)
{
	kernel_start();
	return (0);
}