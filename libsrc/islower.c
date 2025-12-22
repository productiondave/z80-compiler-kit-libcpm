#include <stdint.h>

int islower(int c)
{
	return ((uint8_t)c >= 'a') && ((uint8_t)c <= 'z');
}
