#include <kernel.h>
#include <stdint.h>
#include <string.h>
#include <vga.h>
/* non-opti version */

struct options {
	uint8_t		b_signed;
	uint8_t		b_zpadded;
	uint8_t		b_formatted;
	uint8_t		b_rightpadded;
	uint32_t	precision;
	uint32_t	width;
};

//statics

static size_t	printk_int(int arg);
static size_t	printk_hex(int arg);
static size_t	printk_bin(int arg);
static size_t	printk_char(char c);
static size_t	printk_string(char *str);

static size_t	printk_int(int arg)
{
	putnbr(arg);
	return (0);
}

static size_t	printk_hex(int arg)
{
	putnbr_base(arg, 16);
	return (0);
}

static size_t	printk_bin(int arg)
{
	putnbr_base(arg, 2);
	return (0);
}

static size_t	printk_char(char c)
{
	putchar(c);
	return (0);
}

static size_t	printk_string(char *str)
{
	putstring((const char *)str);
	return (0);
}

extern int	printk(const char *fmt, ...)
{
	char	**args = (char **) &fmt;
	struct options opts = {0, 0, 0, 0, 0, 0};

	args++;
	(void)opts;
	while (*fmt != '\0') {
		if (*fmt == '%') {
			fmt++;
			if (*fmt == '0') {
				opts.b_zpadded = 1;
				fmt++;
			}
			if (*fmt == '#') {
				opts.b_formatted = 1;
				fmt++;
			}
			if (*fmt== '-') {
				opts.b_rightpadded = 1;
				fmt++;
			}
			if (*fmt == '+') {
				opts.b_signed = 1;
				fmt++;
			}
			if (*fmt == 'd' || *fmt == 'i') {
				printk_int(*((int *) args++));
			}
			else if (*fmt == '%') {
				putchar('%');
			}
			else if (*fmt == 'x') {
				printk_hex(*((int *) args++));
			}
			else if (*fmt == 'b') {
				printk_bin(*((int *) args++));
			}
			else if (*fmt == 's') {
				if (*args == NULL) {
					printk_string("(null)");
				}
				else {
					printk_string(*args++);
				}
			}
			else if (*fmt == 'c') {
				printk_char((char)(*((int *) args++)));
			}
		}
		else {
			putchar(*fmt);
		}
		fmt++;
	}
	return (1);
}