#include <kfs/shell.h>
#include <kfs/system.h>

extern int		builtin_shutdown(char **opts)
{
	(void)(opts);
	shutdown();
	return (0);
}