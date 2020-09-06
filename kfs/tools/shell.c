#include <kfs/kernel.h>
#include <kfs/keyboard.h>
#include <kfs/shell.h>
#include <kfs/vga.h>

static void		prompt(void);
static void		shell_move_cursor_right(void);
static void		shell_move_cursor_left(void);

static uint8_t	position;

static void		prompt(void)
{
	printk(KERN_NONE "$> ");
}

static void		shell_move_cursor_right(void)
{
	if (position < MAX_LINE) {
		position++;
		move_cursor_right();
	}
}

static void		shell_move_cursor_left(void)
{
	if (position > 0) {
		position--;
		move_cursor_left();
	}
}

extern void		shell(void)
{
	uint8_t				key = 0;
	static char			buffer[MAX_LINE];

	static uint8_t		cmds_nbr = 1;
	static t_shell_cmd	cmds[] = {
		{.cmd = "shutdown", .f = builtin_shutdown}
	};

	load_shortcut(0xe04d, shell_move_cursor_right); /* arrow right */
	load_shortcut(0xe04b, shell_move_cursor_left); /* arrow left */

	prompt();

	while (1) {
		position = 0;
		do {
			key = read(&(buffer[position]), 1);
			if (key != 0) {
				printk(KERN_NONE "%c", buffer[position]);
			}
		} while (key != 0x1c); /* return key */
	}
}