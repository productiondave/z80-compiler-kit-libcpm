#include <stdint.h>

int isupper(int c)
{
	return ((uint8_t)c >= 'A') && ((uint8_t)c <= 'Z');
}
