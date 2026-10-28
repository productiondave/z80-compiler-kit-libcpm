// vim: set ts=2 sw=2:
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

/*
 * This library is for dealing with the HCCA for retronet operations on the
 * nabu
 */

#ifndef _HCCA_H
#define _HCCA_H

#include <stdint.h>
#include <stdbool.h>

/* Write a value to an AY register */
extern void hcca_write(uint8_t val);

/* Read a value from an AY register */
extern uint8_t hcca_read();


#endif //_HCCA_H
