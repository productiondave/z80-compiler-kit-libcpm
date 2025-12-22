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
* https://elgibbor.hashnode.dev/building-your-own-printf-in-c-a-step-by-step-guide
*
*****************************************************************************
*/

#include <fcntl.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>

#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

void putchar(char c);

static char _buf[16]; // largest number is 15 digits long

/* -1 returned means erro of some sort */
int format_integer(char *dst, int16_t val, uint8_t base, uint8_t s, uint8_t width, uint8_t zeroPad) {
  uint8_t l;
  uint8_t j;
  char pad;
  l = 0;

  // Convert the number to ascii
  if (s)
    itoa(val, _buf, base);
  else
    uitoa(val, _buf);

  // Calculate how much padding is needed and output it.
  if (width){
    for (j = 0; _buf[j] != '\0'; j++) {
      ;
    }
    width = width - j;
    if (zeroPad)
      pad = '0';
    else 
      pad = ' ';

    for (j = 0; j < width; j++) {
      if (dst == NULL) putchar(pad);
      else *dst++ = pad;
      l++;
    }
  }
  
  for (j = 0; _buf[j] != '\0'; j++) {
    if (dst == NULL) putchar(_buf[j]);
    else *dst++ = _buf[j];
    l++;
  }
  return l;
}

va_list arg_list;

int _printf(char *dst, const char *format) {
  int len = 0;
  int i, j;
  size_t val;
  char c;
  char *str;


  for (i = 0; format[i] != '\0'; i++) {
    if (format[i] == '%') {
      int width = 0;
      int zero_pad = 0;
      i++;
      // check for '0' flag
      if (format[i] == '0') {
          zero_pad = 1;
          i++;
      }
      // parse width (e.g., the '4' in %04x)
      while (IS_DIGIT(format[i])) {
          width = width * 10 + (format[i] - '0');
          i++;
      }
      // char literal
      if (format[i] == 'c') {
        c = va_arg(arg_list, unsigned char);
        if (dst == NULL)
          putchar(c);
        else
          *dst++ = c;
        len++;
      }
      // string literal
      if (format[i] == 's') {
        str = va_arg(arg_list, char *);
        for (j = 0; str[j] != '\0'; j++) {
          if (dst == NULL)
            putchar(str[j]);
          else
            *dst++ = str[j];
          len++;
        }
      }
      // signed 16 bit decimal
      if (format[i] == 'd') {
        val = va_arg(arg_list, size_t);
        j = format_integer(dst, val, 10, 1, width, 0);
        if (j == -1)
          return -1;
        len += j;
        if (dst != NULL) dst += j;
      }

      // unsigned 16 bit decimal
      if (format[i] == 'u') {
        val = va_arg(arg_list, size_t);
        j = format_integer(dst, val, 10, 0, width, 0);
        if (j == -1)
          return -1;
        len += j;
        if (dst != NULL) dst += j;
      }

      // unsigned 16 bit hexadecimal
      if (format[i] == 'x') {
        val = va_arg(arg_list, size_t);
        j = format_integer(dst, val, 16, 1, width, zero_pad);
        if (j == -1)
          return -1;
        len += j;
        if (dst != NULL) dst += j;
      }
      /* other formatters */
    } else {
      if (dst == NULL) putchar(format[i]);
      else *dst++ = format[i];
      len++;
    }
  }
  va_end(arg_list);
  return len;
}

int sprintf(char *dst, const char* format, va_list args)
{
  va_start(arg_list, format);
  return _printf(dst, format);
}

int printf(const char *format, va_list args) {
  va_start(arg_list, format);
  return _printf(NULL, format); 
}

