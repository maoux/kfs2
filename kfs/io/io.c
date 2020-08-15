#include <io.h>

extern void		outportb(size_t port, unsigned char byte)
{
	__asm__ volatile ("outb %0, %w1;"
		 :
		 : "a" (byte), "Nd" (port)
		);
}

extern unsigned char	inportb(size_t port)
{
	unsigned char	byte;

	__asm__ volatile ("inb %1, %0"
			: "=r" (byte)
			: "Nd" (port)
	);
	return (byte);
}