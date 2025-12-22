#include <stdint.h>
int isspace(int c)
{
	return ((uint8_t)c == ' ') ||
	       ((uint8_t)c == '\t') ||
		   ((uint8_t)c == '\n') ||
		   ((uint8_t)c == '\r') ||
		   ((uint8_t)c == '\f') ||
		   ((uint8_t)c == '\v');
}

