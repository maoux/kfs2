#include <keyboard.h>
#include <vga.h>
#include <kfs/kernel.h>
#include <kfs/io.h>
#include <kfs/ps2_controller.h>

uint8_t		shift = 0;
uint8_t		ctrl_l = 0;
uint8_t		ctrl_r = 0;
uint8_t		alt_l = 0;
uint8_t		alt_r = 0;

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

/*
* TODO add usb legacy check and ACPI check
*/
extern uint8_t		init_ps2_keyboard(void)
{
	uint8_t		response;
	uint8_t		config;
	uint8_t		port1_status = 0x01;
	uint8_t		port2_status = 0x01;

	/* disable channel 1 ps2 controller */
	send_command(0x64, 0xad, 0x00, 0, 0);
	/* if exist, disable channel 2 */
	send_command(0x64, 0xa7, 0x00, 0, 0);

	/* flush output buffer */
	inportb(0x60);

	/* read controller configuration byte */
	wait_ps2_to_write();
	config = send_command(0x64, 0x20, 0x00, 0, 1);
	//config &= 0xbc; //0b1011100 clear translation and interupts


	/* perform self test */
	wait_ps2_to_write();
	response = send_command(0x64, 0xaa, 0, 0, 1);
	if (response == 0x55) {
		printk(KERN_INFO "Test PS/2 Controller passed\n");
	}
	else if (response == 0xfc) {
		printk(KERN_INFO "Test PS/2 Controller failed\n");
	}

	/*
		write configuration byte here because
		self test might overwrite it
	*/
	//wait_ps2_to_write();
	//send_command(0x64, 0x60, config, 1, 0);

	/* determine if there are 2 channels */
	if ((config & 0x20) == 1) {
		wait_ps2_to_write();
		send_command(0x64, 0xa8, 0x00, 0, 0);
		wait_ps2_to_write();
		config = send_command(0x64, 0x20, 0x00, 0, 1);
		if ((config & 0x20) == 1) {
			printk(KERN_INFO "PS/2 controller doesn't support dual channels\n");
		}
		else {
			printk(KERN_INFO "PS/2 controller support dual channels\n");
			wait_ps2_to_write();
			send_command(0x64, 0xa7, 0x00, 0, 0);
		}
	}

	/*test ps/2 ports*/
	wait_ps2_to_write();
	port1_status = send_command(0x64, 0xab, 0x00, 0, 1);
	if ((config & 0x20) == 0) {
		wait_ps2_to_write();
		port2_status = send_command(0x64, 0xa9, 0x00, 0, 1);
	}
	if (port1_status != 0x00 && port2_status != 0x00) {
		/* error no functionning ports left */
		return (1);
	}

	/* enable ports */
	if (port1_status == 0x00) {
		wait_ps2_to_write();
		send_command(0x64, 0xae, 0x00, 0, 0);
	}
	if (port2_status == 0x00) {
		wait_ps2_to_write();
		send_command(0x64, 0xa8, 0x00, 0, 0);
	}

	/* enable interupts */
	//config |= 0x03;
	//wait_ps2_to_write();
	//send_command(0x64, 0x60, config, 1);
	return (0);
}

extern void		keyboard_loop(void)
{
	uint8_t		status;
	uint8_t		key;


	__asm__ volatile("cli;");

	//get scan code set (between 41, 43 or 3f - 1, 2 or 3)
	key = send_command(0x60, 0xf0, 0x00, 1, 1);
	while (key == 0xfa) {
		wait_ps2_to_read();
		key = inportb(0x60);
	}
	printk(KERN_INFO "Current scan code set : %d\n", (key & 0x07));

	while (1) {
		status = wait_ps2_to_read();
		if (status != 0x01 && status != 0x20) {
			key = inportb(0x60);
			// if (key == 0x01) {
			// 	return ;
			// }
			print_key(key, status);
		}
	}
}