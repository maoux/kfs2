#include <kfs/kernel.h>
#include <kfs/vga.h>
#include <kfs/keyboard.h>

static void		print_grub_meminfo(t_grub_info *grub_info)
{
	t_palette_color_info	*palette;
	t_color					*color;
	// t_mmap		*mmap;
	uint32_t	flags;

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
	if (IS_GFLAG(flags, GFLAG_FRAMEBUFFER)) {
		printk(KERN_DEBUG "bootloader framebuffer addr:\t%#010x\n", grub_info->framebuffer_addr_low);
		printk(KERN_DEBUG "bootloader framebuffer pitch:\t%d\n", grub_info->framebuffer_pitch);
		printk(KERN_DEBUG "bootloader framebuffer width:\t%d\n", grub_info->framebuffer_width);
		printk(KERN_DEBUG "bootloader framebuffer height:\t%d\n", grub_info->framebuffer_height);
		printk(KERN_DEBUG "bootloader framebuffer bpp:\t\t%d\n", grub_info->framebuffer_bpp);
		printk(KERN_DEBUG "bootloader framebuffer type:\t\t%d\n", grub_info->framebuffer_type);
		if (grub_info->framebuffer_type == 0) {
			palette = (t_palette_color_info *)(&(grub_info->color_info));
			for (uint32_t i = 0; i < palette->framebuffer_palette_num_colors; i++) {
				color = (t_color *)palette->framebuffer_palette_addr + i * sizeof(t_color);
				printk(KERN_DEBUG "color %d:\tr=%d g=%d b=%d\n", i, color->r, color->g, color->b);
			}
		}
		if (grub_info->framebuffer_type == 2) {
			printk(KERN_DEBUG "EGA standard text mode enabled\n");
		}
	}
	// if (IS_GFLAG(flags, GFLAG_MMAP)) {
	// 	mmap = (t_mmap *)grub_info->mmap_addr;
	// 	while ((uint32_t)mmap < grub_info->mmap_addr + grub_info->mmap_length) {
	// 		printk(KERN_DEBUG "mmap struct size: %d\n", mmap->size);
	// 		printk(KERN_DEBUG "mmap: base addr %010#x - length %d type %d\n", mmap->base_addr_low, mmap->length_low, mmap->type);
	// 		mmap = (t_mmap *)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
	// 	}
	// }
}

extern int		kmain(uint32_t magic, uint32_t *meminfo_offset)
{
	t_grub_info	*grub_info;

	if (magic != 0x2badb002) {
		return (0);
	}

	grub_info = (t_grub_info *)meminfo_offset;
	if (IS_GFLAG(grub_info->flags, GFLAG_FRAMEBUFFER)) {
		init_video((uint64_t *)grub_info->framebuffer_addr_low,
		grub_info->framebuffer_width, grub_info->framebuffer_height);
	}
	else {
		/*
			assume video buffer is on location 0xb8000 for now
			with standard text mode size
		*/
		init_video((uint64_t *)0xb8000, 80, 25);
	}
	printk(KERN_DEBUG "memory info location from grub : %010#x\n", (int)meminfo_offset);
	print_grub_meminfo(grub_info);

	if (init_ps2_keyboard() == 1) {
		printk(KERN_ERR "PS/2 Controller tests failed\n");
		return (0);
	}
	keyboard_loop();

	return (0);
}