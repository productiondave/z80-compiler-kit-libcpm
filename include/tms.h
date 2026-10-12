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

#ifndef _TMS_H
#define _TMS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define TMS_COLLISION_BIT 1 << 5
#define TMS_5TH_SPRITE_BIT 1 << 4

enum {
  TRANSPRAENT=0,
  BLACK,
  MEDIUM_GREEN,
  LIGHT_GREEN,
  DARK_BLUE,
  LIGHT_BLUE,
  DARK_RED,
  CYAN,
  MEDIUM_RED,
  LIGHT_RED,
  DARK_YELLOW,
  LIGHT_YELLOW,
  DARK_GREEN,
  MAGENTA,
  GRAY,
  WHITE,
};

enum {
  MODE_G1 = 0,
  MODE_G2,
  MODE_MC,
  MODE_TEXT
};

typedef struct sprite_s {
  int y;          // location in Y axis (can be negative)
  int x;          // location in X axis (can be negative - value depends on ECB)
  uint8_t pattern;    // pattern ID in sprites attribute table
  uint8_t color;      // 0xE...CCCC : early clock bit + COLOR
} Sprite;

extern Sprite sprites[32];   // this array of sprites is flushed to the sprite
                      // attribute table in tms_flush_spr() until a Y value of
                      // 0xD0 is encountered.

/* global pointer to the framebuffer which will be allocated in one of the
 * tms_init_* routines
 */
extern char *tms_buf;

/* variables holding the base addresses of VDP memory tables */
extern uint16_t tms_name_tbl;      // REG: 2
extern uint16_t tms_color_tbl;     // REG: 3
extern uint16_t tms_patt_tbl;      // REG: 4
extern uint16_t tms_sp_attr_tbl;   // REG: 5
extern uint16_t tms_sp_patt_tbl;   // REG: 6

extern uint8_t  tms_mode;

extern uint16_t tms_n_tbl_len;
extern uint16_t tms_c_tbl_len;

/* Initialize the VDP in one of the modes */
void tms_init_g1(uint8_t fg, uint8_t bg, bool largesp, bool mag);
void tms_init_g2(uint8_t fg, uint8_t bg, bool largesp, bool mag);
void tms_init_mc(uint8_t fg, uint8_t bg, bool largesp, bool mag);
void tms_init_text(uint8_t fg, uint8_t bg);

/* Init G2 memory tables for bitmap mode graphics.
 * Inits the pattern and colour tables to 0
 * Inits the name table with all unique values in three rows of 0-255
 */
void tms_init_bitmap_mode();

/* Set a vdp register */
extern void tms_set_reg(uint8_t reg, uint8_t val);

/* set the vdp memory read address */
extern void tms_r_addr(uint16_t addr);

/* set the vdp memory write address */
extern void tms_w_addr(uint16_t addr);

/* write a single byte to VRAM at current address */
extern void tms_put(uint8_t c);

/* read a single byte from VRAM at current address */
extern uint8_t tms_get();

/* Fill the tms buffer with a single character */
void tms_fill_buf(const char c);

/* plot a single pixel in G2 bitmap mode.  Expects that tms_init_bitmap_mode
 * has been called to set up the bitmap mode first.
 *
 * Note: This function is taken from NABU-LIB by DJ Sures at 
 * https://github.com/djsures/nabu-lib.git before being modified to work
 * with the Fuzix-Compiler-Kit and the z80-retro-libcpm assembly routines.
 */
void tms_plot_g2(uint8_t x, uint8_t y, uint8_t fg, uint8_t bg);

/* plot a single fat pixel in multi color mode.  Returns true if the pixel at
 * the given location was greater than BLACK
 *
 * if direct = true then also send the pixel to the VDP RAM
 *
 * The original version of this function was taken from:
 * https://github.com/michalin/TMS9918_Arduino before being modified to work
 * with the Fuzix-Compiler-Kit and the z80-retro-libcpm assembly routines.
 */
bool tms_plot_mc(uint8_t x, uint8_t y, uint8_t c, bool direct);

/* wait for vsync and return status of vdp */
extern uint8_t tms_wait();
extern uint8_t tms_status;

/* flush buffer to VDP for the different modes */
extern void tms_g1flush(char *buf);
extern void tms_g2flush(char *buf);
extern void tms_txtflush(char *buf);

/* Load a tile pattern table into the VDP */
void tms_load_pat(char *pattern, size_t len);

/* Load a color table into the VDP */
void tms_load_col(char *color, size_t len);

/* Initialise the sprite table */
void tms_init_sprites();

/* Load a sprite pattern table into the VDP */
void tms_load_spr(char *sprites, size_t len);

/* flush sprite attributes */
void tms_flush_sprites();

/* Print a single character to the screen at x,y */
void tms_put_char(uint8_t x, uint8_t y, char c);

/* Print a stirng to the screen starting at location x,y */
void tms_puts_xy(uint8_t x, uint8_t y, char *s);

#endif //_TMS_H
