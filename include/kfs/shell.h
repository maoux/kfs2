#ifndef __SHELL_H__
# define __SHELL_H__

# define BUF_SIZE	1024
# define MAX_LINE	250
struct shell_cmd {
	const char	*cmd;
	int			(*f)(char **opts);
};
typedef struct shell_cmd	t_shell_cmd;


extern void		shell(void);

/* builins */
extern int		builtin_shutdown(char **opts);

#endif