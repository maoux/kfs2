#ifndef __VGA_H__
# define __VGA_H__

# include <stdint.h>

enum vga_colors {
	VGA_COLOR_BLACK			= 0,
	VGA_COLOR_BLUE			= 1,
	VGA_COLOR_GREEN			= 2,
	VGA_COLOR_CYAN			= 3,
	VGA_COLOR_RED			= 4,
	VGA_COLOR_MAGENTA		= 5,
	VGA_COLOR_BROWN			= 6,
	VGA_COLOR_LIGHT_GREY	= 7,
	VGA_COLOR_DARK_GREY		= 8,
	VGA_COLOR_LIGHT_BLUE	= 9,
	VGA_COLOR_LIGHT_GREEN	= 10,
	VGA_COLOR_LIGHT_CYAN	= 11,
	VGA_COLOR_LIGHT_RED		= 12,
	VGA_COLOR_LIGHT_MAGENTA	= 13,
	VGA_COLOR_LIGHT_BROWN	= 14,
	VGA_COLOR_WHITE			= 15
};

# define SCREEN_NUMBER	10

extern void		clear_screen(void);
extern void		putchar(const unsigned char c);
extern void		putstring(const char *str);
extern void		putnbr(int n);
extern void		putunbr(unsigned int n);
extern void		putnbr_base(int n, unsigned int base, uint8_t uppercase);
extern void		putunbr_base(unsigned int n, unsigned int base, uint8_t uppercase);
extern void		set_textcolor(const unsigned char bg, const unsigned char fc);
extern void		init_video(uint32_t	*framebuffer_addr,  uint32_t width, uint32_t height);

extern void		next_screen(void);
extern void		prev_screen(void);

extern void		print_text_mode_intro(void);

extern void		move_cursor_up(void);
extern void		move_cursor_right(void);
extern void		move_cursor_down(void);
extern void		move_cursor_left(void);

#endif