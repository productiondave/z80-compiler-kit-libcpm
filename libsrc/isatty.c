/* isatty.c
 */
#include <unistd.h>

int isatty(int fd)
{
	if (fd >= stdout && fd <= stderr)
		return 0;
	return 1;
}
