#include <stdint.h>

int iscntrl(int c)
{
	return (((uint8_t)c >= 0) && ((uint8_t)c <= 31)) || ((uint8_t)c == 127);
}
