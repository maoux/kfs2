#ifndef __KERNEL_H__
# define __KERNEL_H__

# define NULL 0

# define KERN_EMERG		"7"
# define KERN_ALERT		"6"
# define KERN_CRIT		"5"
# define KERN_ERR		"4"
# define KERN_WARNING	"3"
# define KERN_NOTICE	"2"
# define KERN_INFO		"1"
# define KERN_DEBUG		"0"

# define DEFAULT_PRIORITY_LEVEL	1

int		printk(const char *fmt, ...);

#endif