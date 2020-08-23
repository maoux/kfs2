#include <kfs/vga.h>
#include <string.h>
#include <io.h>
#include <kfs/kernel.h>

size_t *textmemptr;
char attr = (char)0x0F;
int	csr_x = 0, csr_y = 0;
int max_width, max_height;
uint8_t	screen_buffer_enabled = 0;
uint8_t	screen_cursor;

size_t	screens[SCREEN_NUMBER][80 * 25 + 2];

static void		scroll(void)
{
	size_t		blank, tmp;

	blank = 0x20 | (attr << 8);
	if (csr_y >= max_height) {
		tmp = csr_y - max_height + 1;
		memcpy(textmemptr, textmemptr + tmp * max_width,
			   (max_height - tmp) * max_width * 2);
		memsetw(textmemptr + (max_height - tmp) * max_width,
				blank, max_width);
		if (screen_buffer_enabled) {
			memcpy(screens[screen_cursor] + 2, screens[screen_cursor] + 2 + tmp * max_width,
				   (max_height - tmp) * max_width * 2);
			memsetw(screens[screen_cursor] + 2 + (max_height - tmp) * max_width,
					blank, max_width);
			screens[screen_cursor][1] = 24;
		}
		csr_y = 24;
	}
}

static void		move_cursor(void)
{
	size_t	tmp;

	tmp = csr_y * max_width + csr_x;
	outb(0x3D4, 14);
	outb(0x3D5, tmp >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, tmp);
}

extern void		clear_screen(void)
{
	unsigned char	blank;

	blank = 0x20 | (attr < 8);
	memsetw(textmemptr, blank, max_width * max_height);
	csr_x = 0;
	csr_y = 0;
	move_cursor();
}

extern void		putchar(unsigned char c)
{
	size_t		*cursor;

	if (c == 0x08) {
		if (csr_x > 0) {
			csr_x--;
		}
		else if (csr_y > 0) {
			csr_x = 79;
			csr_y--;
		}
		cursor = textmemptr + (csr_y * max_width + csr_x);
		*cursor = ' ' | (attr << 8);
		if (screen_buffer_enabled) {
			screens[screen_cursor][csr_y * max_width + csr_x + 2] = ' ' | (attr << 8);
			screens[screen_cursor][0] = csr_x;
			screens[screen_cursor][1] = csr_y;
		}
	} else if (c == '\t') {
		//n & 000
		csr_x = (csr_x + 8) & ~(8 - 1);
		if (screen_buffer_enabled) {
			screens[screen_cursor][0] = csr_x;
		}
	} else if (c == '\r') {
		csr_x = 0;
		if (screen_buffer_enabled) {
			screens[screen_cursor][0] = csr_x;
		}
	} else if (c == '\n') {
		csr_y++;
		csr_x = 0;
		if (screen_buffer_enabled) {
			screens[screen_cursor][0] = csr_x;
			screens[screen_cursor][1] = csr_y;
		}
	} else if (c >= ' ') {
		cursor = textmemptr + (csr_y * max_width + csr_x);
		*cursor = c | (attr << 8);
		if (screen_buffer_enabled) {
			screens[screen_cursor][csr_y * max_width + csr_x + 2] = c | (attr << 8);
		}
		if (csr_x == 79) {
			csr_y++;
			csr_x = 0;
		}
		else {
			csr_x++;
		}
		if (screen_buffer_enabled) {
			screens[screen_cursor][0] = csr_x;
			screens[screen_cursor][1] = csr_y;
		}
	}
	scroll();
	move_cursor();
}

/*********************************************************/
/***************** putnbr functions **********************/

extern void		putnbr(int n)
{
	if (n < 0) {
		putchar('-');
		putnbr(-n);
	}
	if (n < 9) {
		putchar((char)n + 48);
		return ;
	}
	putnbr(n / 10);
	putchar((char)(n % 10) + 48);
}

extern void		putunbr(unsigned int n)
{
	if (n < 9) {
		putchar((char)n + 48);
		return ;
	}
	putnbr(n / 10);
	putchar((char)(n % 10) + 48);
}

extern void		putnbr_base(int n, unsigned int base, uint8_t uppercase)
{
	static char	uset[16] =	{'0', '1', '2', '3', '4', '5',
							 '6', '7', '8', '9', 'A', 'B',
							 'C', 'D', 'E', 'F'};
	static char	lset[16] =	{'0', '1', '2', '3', '4', '5',
							 '6', '7', '8', '9', 'a', 'b',
							 'c', 'd', 'e', 'f'};

	if (base > 16) {
		return ;
	}
	if (n < 0) {
		putchar('-');
		putnbr_base(-n, base, uppercase);
	}
	if (n < (int)base) {
		uppercase ? putchar(uset[n]) : putchar(lset[n]);
		return ;
	}
	putnbr_base(n / base, base, uppercase);
	uppercase ? putchar(uset[n % base]) : putchar(lset[n % base]);	
}

extern void		putunbr_base(unsigned int n, unsigned int base, uint8_t uppercase)
{
	static char	uset[16] =	{'0', '1', '2', '3', '4', '5',
							 '6', '7', '8', '9', 'A', 'B',
							 'C', 'D', 'E', 'F'};
	static char	lset[16] =	{'0', '1', '2', '3', '4', '5',
							 '6', '7', '8', '9', 'a', 'b',
							 'c', 'd', 'e', 'f'};

	if (base > 16) {
		return ;
	}
	if (n < base) {
		uppercase ? putchar(uset[n]) : putchar(lset[n]);
		return ;
	}
	putnbr_base(n / base, base, uppercase);
	uppercase ? putchar(uset[n % base]) : putchar(lset[n % base]);	
}

extern void		putstring(const char *str)
{
	for (size_t i = 0; str[i]; i++) {
		putchar((const unsigned char)str[i]);
	}
}

/*********************************************************/
/*********************************************************/

extern void		set_textcolor(const unsigned char bg,
							  const unsigned char fg)
{
	attr = (bg << 4) | (fg & 0x0F);
}

extern void		next_screen(void)
{
	if (screen_buffer_enabled && screen_cursor < SCREEN_NUMBER) {
		screen_cursor++;
		clear_screen();
		for (int i = 0; i < max_width * max_height; i++) {
			*(textmemptr + i) = screens[screen_cursor][i + 2];
		}
		csr_x = (int)(screens[screen_cursor][0]);
		csr_y = (int)(screens[screen_cursor][1]);
		move_cursor();
	}
}

extern void		prev_screen(void)
{
	if (screen_buffer_enabled && screen_cursor > 0) {
		screen_cursor--;
		clear_screen();
		for (int i = 0; i < max_width * max_height; i++) {
			*(textmemptr + i) = screens[screen_cursor][i + 2];
		}
		csr_x = (int)(screens[screen_cursor][0]);
		csr_y = (int)(screens[screen_cursor][1]);
		move_cursor();
	}
}

extern void		print_text_mode_intro(void)
{
	set_textcolor(VGA_COLOR_BLACK, VGA_COLOR_BLUE);
	printk(	KERN_NONE	"\t\t\t\t _     __     _\n" 
 						"\t\t\t\t| | __/ _|___/ |\n"
 						"\t\t\t\t| |/ / |_/ __| |\n"
 						"\t\t\t\t|   <|  _\\__ \\ |\n"
						"\t\t\t\t|_|\\_\\_| |___/_|\n\n");
	set_textcolor(VGA_COLOR_BLACK, VGA_COLOR_WHITE);

}

extern void		init_video(uint32_t	*framebuffer_addr, uint32_t width,
							uint32_t height)
{
	textmemptr = (size_t *)framebuffer_addr;
	max_width = (int)width;
	max_height = (int)height;
	if (max_width == 80 && max_height == 25) {
		screen_buffer_enabled = 1;
		screen_cursor = 0;
	}
	clear_screen();
}