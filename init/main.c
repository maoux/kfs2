#include <kernel.h>
#include <vga.h>
#include <io.h>

static void		print_grub_meminfo(uint32_t *addr)
{
	t_grub_info	*grub_info;
	// t_mmap		*mmap;
	uint32_t	flags;

	grub_info = (t_grub_info *)addr;
	flags = grub_info->flags;
	printk(KERN_DEBUG "flags\t\t%016b\n", grub_info->flags);
	
	printk(KERN_DEBUG "mem lower\t%d Kb\n", grub_info->mem_lower);
	printk(KERN_DEBUG "mem upper\t%d Kb\n", grub_info->mem_upper);
	if (IS_GFLAG(flags, GFLAG_DEVICES)) {
		printk(	KERN_DEBUG "boot_device :\n"
				"\tpart3 -> %x\n"
				"\tpart2 -> %x\n"
				"\tpart1 -> %x\n"
				"\tdrive -> %x\n",
				grub_info->boot_device[0], grub_info->boot_device[1],
				grub_info->boot_device[2], grub_info->boot_device[3]);
	}
	if (IS_GFLAG(flags, GFLAG_CMDLINE)) {
		if (grub_info->cmdline) {
			printk(KERN_DEBUG "cmdline :\t%s\n", grub_info->cmdline);
		}
	}
	if (IS_GFLAG(flags, GFLAG_FMTAOUT)) {
		printk(KERN_DEBUG "a.out sym table addr :\t%010#x\n", grub_info->fmt_info.elf_hdr_table_info.addr);
	}
	if (IS_GFLAG(flags, GFLAG_FMTELF)) {
		printk(KERN_DEBUG "elf hdr table addr :\t%010#x\n", grub_info->fmt_info.elf_hdr_table_info.addr);
	}
	if (IS_GFLAG(flags, GFLAG_NAME)) {
		printk(KERN_DEBUG "bootloader name :\t%s\n", grub_info->bootloader_name);
	}
	// if (IS_GFLAG(flags, GFLAG_MMAP)) {
	// 	mmap = (t_mmap *)grub_info->mmap_addr;
	// 	while ((uint32_t)mmap < grub_info->mmap_addr + grub_info->mmap_length) {
	// 		printk(KERN_DEBUG "mmap struct size: %d\n", mmap->size);
	// 		printk(KERN_DEBUG "mmap: base addr %010#x - length %d type %d\n", mmap->base_addr_low, mmap->length_low, mmap->type);
	// 		mmap = (t_mmap *)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
	// 	}
	// }
	//printk("")
}

extern int		kmain(uint32_t magic, uint32_t *meminfo_offset)
{
	if (magic != 0x2badb002) {
		return (0);
	}
	init_video();

	printk(KERN_DEBUG "memory info location from grub : %010#x\n", (int)meminfo_offset);
	print_grub_meminfo(meminfo_offset);
	return (0);
}