#include <io.h>

extern void		outportb(size_t port, unsigned char byte)
{
	__asm__("outb %0, %w1;"
		 :
		 : "a" (byte), "Nd" (port)
		);
}