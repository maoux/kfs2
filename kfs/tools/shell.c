#include <kfs/kernel.h>
#include <kfs/keyboard.h>
#include <kfs/shell.h>

static void		prompt(void);

static void		prompt(void)
{
	printk(KERN_NONE "$> ");
}

extern void		shell()
{
	static char	buffer[1];

	prompt();
	while (1) {
		if (read(buffer, 1) > 0) {
			printk(KERN_NONE "%c", buffer[0]);
		}
	}
}