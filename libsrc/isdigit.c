#include <stdint.h>

int isdigit(int c)
{
	return ((uint8_t)c >= '0') && ((uint8_t)c <= '9');
}

