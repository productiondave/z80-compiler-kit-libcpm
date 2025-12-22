#include <stdint.h>

int isascii(int c)
{
	return !((uint8_t)c & 0x80);
}

