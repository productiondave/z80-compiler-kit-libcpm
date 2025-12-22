#include <stdint.h>

int isgraph(int c)
{
	return ((uint8_t)c >= 33) && ((uint8_t)c <= 126);
}

