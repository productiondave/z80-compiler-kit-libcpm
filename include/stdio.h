/*
*****************************************************************************
*
*    Copyright (C) 2025 David Latham
*
*    This library is free software; you can redistribute it and/or
*    modify it under the terms of the GNU Lesser General Public
*    License as published by the Free Software Foundation; either
*    version 2.1 of the License, or (at your option) any later version.
*
*    This library is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public
*    License along with this library; if not, write to the Free Software
*    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
*    USA
*
* https://github.com/linuxplayground/z80-compiler-kit-libcpm
*
*****************************************************************************
*/

#ifndef _STDIO_H
#define _STDIO_H

#include <stdbool.h>
#include <stdint.h>

/* wait for a character and return it */
char getchar();

/* print a single ascii character to the console.
 * translate `\n` to `\r\n`
 */
void putchar(char c);

/* Print a zero terminated C string using putchar
 */
void puts(char *s);


/* This printf implimentation is tiny.  It only supports a few  format
 * specifiers at this time.
 *
 * - %c for single character
 * - %s for string literals
 * - %d for signed ints decimal (16bit)
 * - %x for unsigned ints hexadecimal (16bit)
 * - %u for unsigned ints decimal (16bit)
 *
 *   uses cpm_conout to print to stdout.
 */

int printf(const char *format, ...);

/* printf into memory buffer instead of stdout.
 *
 * UNSAFE as there are no bounds checks.
 */
int sprintf(char *dst, const char *format, ...);

/* Add the ASCII representation of both nibbles of an unsigned byte `b`
 * the buffer pointed to by `*buf`
 *
 * UNSAFE as there are no bounds checks.
 */
void bin2hex(uint8_t b, char *buf);
#endif //_STDIO_H
