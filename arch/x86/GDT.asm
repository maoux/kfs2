global gdt_flush
extern _GDTR

gdt_flush:
	cli

	lgdt [_GDTR]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:gdt_flush2
gdt_flush2:
	ret
