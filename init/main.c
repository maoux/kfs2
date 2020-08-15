#include <kernel.h>
#include <vga.h>
#include <io.h>
#include <stdint.h>


static void		print_grub_meminfo(uint32_t *addr)
{
	uint8_t		*boot_device;

	printk(KERN_DEBUG "flags\t\t%016b\n", *addr);
	
	printk(KERN_DEBUG "mem lower\t%d Kb\n", *(addr + 1));
	printk(KERN_DEBUG "mem upper\t%d Kb\n", *(addr + 2));
	if ((*addr) & (1 << 1)) {
		boot_device = (uint8_t *)(addr + 3);
		printk(	KERN_DEBUG "boot_device :\n"
				"\tpart3 -> %x\n"
				"\tpart2 -> %x\n"
				"\tpart1 -> %x\n"
				"\tdrive -> %x\n",
				boot_device[0], boot_device[1],
				boot_device[2], boot_device[3]);
	}
	if ((*addr) & (1 << 2)) {
		if ((char *)(*addr + 16)) {
			printk(KERN_DEBUG "cmdline :\t%s\n", (char *)(*(addr + 16)));
		}
	}
	if ((*addr) & (1 << 4)) {
		printk(KERN_DEBUG "a.out sym table addr :\t%010#x\n", *(addr + 9));
	}	if ((*addr) & (1 << 5)) {
		printk(KERN_DEBUG "elf hdr table addr :\t%010#x\n", *(addr + 9));
	}
	if((*addr) & (1 << 9)) {
		printk(KERN_DEBUG "bootloader name :\t%s\n", (char *)(*(addr + 16)));
	}
	//printk("")
}

int		kmain(uint32_t magic, uint32_t *meminfo_offset)
{
	if (magic != 0x2badb002) {
		return (0);
	}
	init_video();

	printk(KERN_DEBUG "memory info location from grub : %010#x\n", (int)meminfo_offset);
	print_grub_meminfo(meminfo_offset);
	return (0);
}