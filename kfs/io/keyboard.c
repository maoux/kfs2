#include <keyboard.h>
#include <vga.h>

//US QWERTY standard keyboard set1 lower case
const char	key_map_1_low[94] = {
	0x0, ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
	0x0, '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
	0x0, 0x0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0x0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0x0, '*',
	0x0, ' ', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	'7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

extern void		print_key(uint32_t key)
{
	putchar(key_map_1_low[key]);
}