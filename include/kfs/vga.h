#ifndef __VGA_H__
# define __VGA_H__

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

extern void		clear_screen(void);
extern void		putchar(const unsigned char c);
extern void		putstring(const char *str);
extern void		putnbr(int n);
extern void		putnbr_base(int n, int base);
extern void		set_textcolor(const unsigned char bg, const unsigned char fc);
extern void		init_video(void);

#endif