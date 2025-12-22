#include <stdint.h>
#include <ctype.h>

int ispunct(int c)
{
	return isascii(c) && !iscntrl(c) && !isalnum(c) && !isspace(c);
}

