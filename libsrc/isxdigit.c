#include <stdint.h>
#include <ctype.h>

int isxdigit(int c)
{
	uint8_t bc = c;
	if (isdigit(bc))
		return 1;
	bc |= 0x20;
	return ((bc >= 'a') && (bc <= 'f'));
}
