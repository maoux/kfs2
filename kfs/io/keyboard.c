#include <keyboard.h>
#include <vga.h>
#include <kfs/kernel.h>
#include <kfs/io.h>

#define KEY_MAP_SIZE	94

uint8_t		shift = 0;
uint8_t		ctrl_l = 0;
uint8_t		ctrl_r = 0;
uint8_t		alt_l = 0;
uint8_t		alt_r = 0;

static uint8_t		wait_ps2_to_write(void);
static uint8_t		wait_ps2_to_read(void);

static uint8_t	send_command(uint8_t port, uint8_t cmd,
							 uint8_t data, uint8_t send_data);


static void		print_key(uint32_t key, uint32_t status);

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

static uint8_t		wait_ps2_to_write(void)
{
	uint8_t		status;

	//wait for bit 1 to be clear
	do {
		status = inportb(0x64);
	} while ((status & 0x02) == 1);
	return (status);
}

static uint8_t		wait_ps2_to_read(void)
{
	uint8_t		status;

	//wait for bit 0 to be set
	do {
		status = inportb(0x64);
	} while ((status & 0x01) == 0);
	return (status);
}

static uint8_t	send_command(uint8_t port, uint8_t cmd, uint8_t data, uint8_t send_data)
{
	uint8_t		response;	

	wait_ps2_to_write();
	//send command code to controller
	outportb(port, cmd);
	if (send_data) {
		wait_ps2_to_write();
		//send data byte to data port
		outportb(0x60, data);
	}
	wait_ps2_to_read();
	//read response from data port
	response = inportb(0x60);
	return (response);
}


static void		print_key(uint32_t key, uint32_t status)
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


extern void		keyboard_loop(void)
{
	uint8_t		status;
	uint8_t		key;


	__asm__ volatile("cli;");

	key = send_command(0x64, 0xaa, 0, 0);
	if (key == 0x55) {
		printk(KERN_INFO "Test PS/2 Controller passed\n");
	}
	else if (key == 0xfc) {
		printk(KERN_INFO "Test PS/2 Controller failed\n");
	}

	//get scan code set (between 41, 43 or 3f - 1, 2 or 3)
	key = send_command(0x60, 0xf0, 0x00, 0x01);
	while (key == 0xfa) {
		wait_ps2_to_read();
		key = inportb(0x60);
	}
	printk(KERN_INFO "Current scan code set : %d\n", (key & 0x07));

	while (1) {
		status = wait_ps2_to_read();
		if (status != 0x01 && status != 0x20) {
			key = inportb(0x60);
			print_key(key, status);
		}
	}
}