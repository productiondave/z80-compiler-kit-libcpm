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

#include <tms.h>
#include <string.h>
#include <stdlib.h>

// Initialise the name table with unique values over each band.
// https://github.com/michalin/TMS9918_Arduino
void init_mc_nt()
{
  size_t i,j;
  tms_w_addr(tms_name_tbl);
  for (j = 0; j < 24; j++)
    for (i = 0; i < 32; i++)
      tms_put(i + 32 * (j / 4));
}

void tms_init_mc(uint8_t fg, uint8_t bg, bool largesp, bool mag)
{
  size_t i;
  uint8_t sprite_flags = 0;
  uint8_t *p;

  sprite_flags |= largesp << 1;
  sprite_flags |= mag << 0;

  tms_name_tbl = 0x1400;
  tms_color_tbl = 0x0; // not used
  tms_patt_tbl = 0x800;
  tms_sp_attr_tbl = 0x1000;
  tms_sp_patt_tbl = 0x0;

  tms_mode = MODE_MC;

  tms_set_reg(0, 0x00);
  tms_set_reg(1, 0xE8|sprite_flags); //16K, enable display, enable int + sprite settings
  tms_set_reg(2, 0x05);
  tms_set_reg(3, 0x00); // not used
  tms_set_reg(4, 0x01);
  tms_set_reg(5, 0x20);
  tms_set_reg(6, 0x00);
  tms_set_reg(7, bg & 0x0F);

#ifdef NABU
  di();
#endif
  init_mc_nt();
  tms_buf = malloc(0x600);
  memset(tms_buf, 0x11, 0x600);

  tms_init_sprites();
#ifdef NABU
  ei();
#endif
  // Flush the initial black screen slowly by doing half the screen each frame over two frames.
  tms_w_addr(tms_patt_tbl);
  p = (uint8_t*)tms_buf;
  tms_wait();
  for (i=0; i<0x300; ++i) {
    tms_put(*p++);
  }
  tms_wait();
  for (i=0; i<0x300; ++i) {
    tms_put(*p++);
  }
}

// Returns true if a colision with an existing pixel in the buffer was found and the value
// of the existing pixel was not TRANSPARENT or BLACK.  If direct = true, then also write
// the new pixel to vram.
// Collisions are tracked in a shadow buffer in system memory.
bool tms_plot_mc(uint8_t x, uint8_t y, uint8_t c, bool direct)
{
  uint16_t addr;
  uint8_t dot;
  bool collide = false;

  addr = 8 * (x / 2) + y % 8 + 256 * (y / 8);
  dot = tms_buf[addr];

  if (x & 1) {// Odd columns
    if ((dot & 0x0F) > BLACK)
      collide = true;
    dot = (dot & 0xF0) + (c & 0x0F);
  } else {
    if (((dot & 0xF0) >> 4) > BLACK)
      collide = true;
    dot = (dot & 0x0F) + (c << 4);
  }
  tms_buf[addr] = dot;

  if (direct) {
    tms_w_addr(tms_patt_tbl + addr);
    tms_put(dot);
  }
  return collide;
}

