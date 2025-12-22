#include <stdint.h>

int isprint(int c)
{
	return ((uint8_t)c >= 32) && ((uint8_t)c <= 126);
}

