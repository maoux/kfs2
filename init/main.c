#include <kernel.h>
#include <vga.h>
#include <io.h>
#include <stdint.h>


static void		print_grub_meminfo(uint32_t *addr)
{
	uint8_t		*boot_device;

	printk("flags\t\t%b\n", *addr);
	
	printk("mem lower\t%d Kb\n", *(addr + 1));
	printk("mem upper\t%d Kb\n", *(addr + 2));
	if ((*addr) & (1 << 1)) {
		boot_device = (uint8_t *)(addr + 3);
		printk(	"boot_device :\n"
				"\tpart3 -> %x\n"
				"\tpart2 -> %x\n"
				"\tpart1 -> %x\n"
				"\tdrive -> %x\n",
				boot_device[0], boot_device[1],
				boot_device[2], boot_device[3]);
	}
	//printk("")
}

static int		kernel_start(void)
{
	return (0);
}

int		kmain(uint32_t magic, uint32_t *meminfo_offset)
{
	if (magic != 0x2badb002) {
		return (0);
	}
	init_video();

	printk("memory info location from grub : %h\n", (int)meminfo_offset);
	print_grub_meminfo(meminfo_offset);
	kernel_start();
	return (0);
}