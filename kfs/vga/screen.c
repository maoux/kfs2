#include <kfs/vga.h>
#include <string.h>
#include <io.h>
#include <kfs/kernel.h>

static void		move_cursor(void);

static size_t *textmemptr;
static char attr = (char)0x0F;
static int	csr_x = 0, csr_y = 0;
static int max_width, max_height;

static uint8_t	screen_buffer_enabled = 0;
static uint8_t	screen_cursor; /* current screen being displayed */
static uint8_t	scroll_cursor; /* current part of buffer being displayed*/

static size_t	screens[SCREEN_NUMBER][80 * MAX_SCROLL * 2 + 3];

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
			if (scroll_cursor >= MAX_SCROLL) {
				/* move everything one line up and erase first line */
				memcpy(	screens[screen_cursor] + SCREEN_META_DATA_SIZE,
						screens[screen_cursor] + SCREEN_META_DATA_SIZE + tmp * max_width,
						(MAX_SCROLL - tmp) * max_width * 2);
				/* fill last line of screen buffer with blank */
				memsetw(screens[screen_cursor] + SCREEN_META_DATA_SIZE
						+ (MAX_SCROLL - tmp) * max_width,
						blank, max_width);
			} else {
				scroll_cursor++;
				screens[screen_cursor][2] = scroll_cursor;
			}
			screens[screen_cursor][1] = 24;
		}
		csr_y = 24;
	}
}

extern void		move_cursor_up(void)
{
	if (csr_y > 0) {
		csr_y--;
		if (screen_buffer_enabled) {
			screens[screen_cursor][1] = csr_y;
		}
		move_cursor();
	}
}

extern void		move_cursor_right(void)
{
	if (csr_x < max_width - 1) {
		csr_x++;
		if (screen_buffer_enabled) {
			screens[screen_cursor][0] = csr_x;
		}
		move_cursor();
	}
	else if (csr_y < max_height - 1) {
		csr_x = 0;
		csr_y++;
		if (screen_buffer_enabled) {
			screens[screen_cursor][0] = csr_x;
			screens[screen_cursor][1] = csr_y;
		}
		move_cursor();
	}
}

extern void		move_cursor_down(void)
{
	if (csr_y < max_height - 1) {
		csr_y++;
		if (screen_buffer_enabled) {
			screens[screen_cursor][1] = csr_y;
		}
		move_cursor();
	}
}

extern void		move_cursor_left(void)
{
	if (csr_x > 0) {
		csr_x--;
		if (screen_buffer_enabled) {
			screens[screen_cursor][0] = csr_x;
		}
		move_cursor();
	}
	else if (csr_y > 0) {
		csr_x = max_width - 1;
		csr_y--;
		if (screen_buffer_enabled) {
			screens[screen_cursor][0] = csr_x;
			screens[screen_cursor][1] = csr_y;
		}
		move_cursor();
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
			csr_x = max_width - 1;
			csr_y--;
		}
		cursor = textmemptr + (csr_y * max_width + csr_x);
		*cursor = ' ' | (attr << 8);
		if (screen_buffer_enabled) {
			screens[screen_cursor][(scroll_cursor + csr_y) * max_width + csr_x + SCREEN_META_DATA_SIZE] = ' ' | (attr << 8);
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
			screens[screen_cursor][(scroll_cursor  + csr_y) * max_width + csr_x + SCREEN_META_DATA_SIZE] = c | (attr << 8);
		}
		if (csr_x == max_width - 1) {
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

extern void		set_textcolor(const unsigned char bg,
							  const unsigned char fg)
{
	attr = (bg << 4) | (fg & 0x0F);
}

extern void		next_screen(void)
{
	if (screen_buffer_enabled && screen_cursor < SCREEN_NUMBER - 1) {
		screen_cursor++;
		clear_screen();
		scroll_cursor = (int)(screens[screen_cursor][2]);
		for (int i = 0; i < max_width * max_height; i++) {
			*(textmemptr + i) = screens[screen_cursor][(scroll_cursor * max_width) + i + SCREEN_META_DATA_SIZE];
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
		scroll_cursor = (int)(screens[screen_cursor][2]);
		for (int i = 0; i < max_width * max_height; i++) {
			*(textmemptr + i) = screens[screen_cursor][(scroll_cursor * max_width) + i + SCREEN_META_DATA_SIZE];
		}
		csr_x = (int)(screens[screen_cursor][0]);
		csr_y = (int)(screens[screen_cursor][1]);
		scroll_cursor = (int)(screens[screen_cursor][2]);
		move_cursor();
	}
}

extern void		buffer_scroll_up(void)
{
	if (screen_buffer_enabled && scroll_cursor > 0) {
		scroll_cursor--;
		screens[screen_cursor][2]--;
		clear_screen();
		for (int i = 0; i < max_width * max_height; i++) {
			*(textmemptr + i) = screens[screen_cursor][(scroll_cursor * max_width) + i + SCREEN_META_DATA_SIZE];
		}
		csr_x = (int)(screens[screen_cursor][0]);
		csr_y = (int)(screens[screen_cursor][1]);
		move_cursor();
	}
}

extern void		buffer_scroll_down(void)
{
	if (screen_buffer_enabled && scroll_cursor < MAX_SCROLL) {
		scroll_cursor++;
		screens[screen_cursor][2]++;
		clear_screen();
		for (int i = 0; i < max_width * max_height; i++) {
			*(textmemptr + i) = screens[screen_cursor][(scroll_cursor * max_width) + i + SCREEN_META_DATA_SIZE];
		}
		csr_x = (int)(screens[screen_cursor][0]);
		csr_y = (int)(screens[screen_cursor][1]);
		move_cursor();
	}
}

extern void		print_text_mode_intro(void)
{
	set_textcolor(VGA_COLOR_BLACK, VGA_COLOR_BLUE);
	printk(	KERN_NONE	"\t\t\t\t _    __    ___\n" 
						"\t\t\t\t| |__/ _|__|_  )\n"
						"\t\t\t\t| / /  _ (_</ /\n"
						"\t\t\t\t|_\\_\\_| /__/___|\n\n");
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
		scroll_cursor = 0;
	}
	clear_screen();
}