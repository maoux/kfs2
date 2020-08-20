#include <keyboard.h>
#include <vga.h>
#include <kernel.h>

#define KEY_MAP_SIZE	94

uint8_t		shift = 0;
uint8_t		ctrl_l = 0;
uint8_t		ctrl_r = 0;
uint8_t		alt_l = 0;
uint8_t		alt_r = 0;


//US QWERTY standard keyboard set1 lower case
const char	key_map_1_lower[KEY_MAP_SIZE] = {
	0x0, 0x0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
	0x08, '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
	'\n', 0x0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0x0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0x0, '*',
	0x0, ' ', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	'7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

const char	key_map_1_upper[KEY_MAP_SIZE] = {
	0x0, 0x0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
	0x08, '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
	'\n', 0x0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
	0x0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0x0, '*',
	0x0, ' ', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	'7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', 0x0, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

extern void		print_key(uint32_t key, uint32_t status)
{
	(void)status;
	switch (key) {
		case 0x2a:
		case 0x36:
			shift = 1;
			return ;
		case 0xaa:
		case 0xb6:
			shift = 0;
			return ;
		case 0x1d:
			ctrl_l = 1;
			return ;
		case 0x9d:
			ctrl_l = 0;
			return ;
		case 0xe01d:
			ctrl_r = 1;
			return ;
		case 0xe09d:
			ctrl_r = 0;
			return ;
		case 0x38:
			alt_l = 1;
			return ;
		case 0xb8:
			alt_l = 0;
			return ;
		case 0xe038:
			alt_r = 1;
			return ;
		case 0xe0b8:
			alt_r = 0;
			return ;
		default:
			if (key < KEY_MAP_SIZE) {
				if (shift) {
					putchar(key_map_1_upper[key]);
					return ;
				}
				else {
					putchar(key_map_1_lower[key]);
				}
			}
			return ;
	}
}