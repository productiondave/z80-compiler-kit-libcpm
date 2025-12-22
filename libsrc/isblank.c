#include <stdint.h>

int isblank(int c)
{
	return ((uint8_t)c == ' ') || ((uint8_t)c == '\t');
}

