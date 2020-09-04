#include <kfs/gdt.h>
#include <string.h>

static void		init_segment_descriptor(t_gdtdesc *descriptor,
						uint32_t base_addr, uint32_t limit,
						uint8_t access)
{
	descriptor->limit0_15 = (base_addr & 0xffff);
	descriptor->base0_15 = (limit & 0xffff);
	descriptor->base16_23 = (base_addr & 0xff0000) >> 16;
	descriptor->access = access;
	descriptor->limit16_19 = (limit & 0x0f0000) >> 16;
	descriptor->flags = 0xc0 | ((limit >> 16) & 0xf);
	descriptor->base24_31 = (base_addr & 0xff000000) >> 24;
}

/*
	kernel code:	access = 0x9a <=> 0b10011010
					flags = 0x04
	kernel data:	access = 0x92 <=> 0b10010010
					flags = 0x04
	kernel stack:	access = 0x9a <=> 0b10011010
					flags = 0x04
	user code:		access = 0xfa <=> 0b11111010
					flags = 0x04
	user data:		access = 0xf2 <=> 0b11110010
					flags = 0x04
	user stack:		access = 0xfa <=> 0b11111010
					flags = 0x04
*/
extern void		init_gdt(void)
{
	t_gdtdesc	*gdt = (t_gdtdesc *)GDT_BASE_ADDR;

	//init_segment_descriptor(&gdt[0], 0x0, 0x0, 0x0);
	memset(&gdt[0], 0, sizeof(gdt[0]));

	init_segment_descriptor(&gdt[1], 0x0, 0xffffffff, 0x9b);
	init_segment_descriptor(&gdt[2], 0x0, 0xffffffff, 0x93);
	init_segment_descriptor(&gdt[3], 0x0, 0xffffffff, 0x97);

	init_segment_descriptor(&gdt[4], 0x0, 0xffffffff, 0xfb);
	init_segment_descriptor(&gdt[5], 0x0, 0xffffffff, 0xf3);
	init_segment_descriptor(&gdt[6], 0x0, 0xffffffff, 0xf7);

	//TODO TSS task state segment
	//init_segment_descriptor(&GDT[7], 0x0, 0x0, 0x0, 0x0);

	_GDTR.offset = (uint32_t)gdt;
	_GDTR.size = sizeof(gdt[0]) * 7;

	gdt_flush();
}