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
 * This library is for systems that have the AY-3-8910 either in hardware or
 * emulated on an FPGA.  The accompanying assembly routine for writing to the
 * register defines the two ports as IO_AYREG and IO_AYDAT.  If these are not
 * at 0xB0 and 0xB1 respectively, then you must edit the assembly source too.
 *
 * On the Nabu they are 0x40 and 0x41.  I don't know how to manage this in the
 * Fuzix-Compiler-Kit ecosystem.
 */

#ifndef _AY38910_H
#define _AY38910_H

#include <stdint.h>
#include <stdbool.h>

#define AY_MIX_TONE_A         0x7E //0b01111110
#define AY_MIX_TONE_B         0x7D //0b01111101
#define AY_MIX_TONE_C         0x7B //0b01111011
#define AY_MIX_NOISE_A        0x77 //0b01110111
#define AY_MIX_NOISE_B        0x6F //0b01101111
#define AY_MIX_NOISE_C        0x5F //0b01011111
#define AY_MIX_ALL_OFF        0x7F //0b01111111

// these are actually indexes into the PERIOD table below.
#define AY_CHANNEL_A          0x00
#define AY_CHANNEL_B          0x02
#define AY_CHANNEL_C          0x04

#define AY_PERIOD_TONE_A_F    0x00
#define AY_PERIOD_TONE_A_C    0x01
#define AY_PERIOD_TONE_B_F    0x02
#define AY_PERIOD_TONE_B_C    0x03
#define AY_PERIOD_TONE_C_F    0x04
#define AY_PERIOD_TONE_C_C    0x05

#define AY_PERIOD_NOISE       0x06
#define AY_MIXER              0x07

#define AY_VOLUME_A           0x08
#define AY_VOLUME_B           0x09
#define AY_VOLUME_C           0x0a

#define AY_ENVELOPE_F         0x0b
#define AY_ENVELOPE_C         0x0c

#define AY_ENVELOPE_SHAPE     0x0d

#define AY_ENV_SHAPE_CONTINUE 0x08
#define AY_ENV_SHAPE_ATTACK   0x04
#define AY_ENV_SHAPE_ALTERNATE 0x02
#define AY_ENV_SHAPE_HOLD     0x01

/*
;  /\  /\  /\  /\  /\
; /  \/  \/  \/  \/  \
*/
#define AY_ENV_SHAPE_TRIANGLE (AY_ENV_SHAPE_CONTINUE | AY_ENV_SHAPE_ATTACK | AY_ENV_SHAPE_ALTERNATE)

/*
; \  /\  /\  /\  /\  /
;  \/  \/  \/  \/  \/
*/
#define AY_ENV_SHAPE_TRIANGLE2 (AY_ENV_SHAPE_CONTINUE | AY_ENV_SHAPE_ALTERNATE)

/*
;  /------------------
; /
*/
#define AY_ENV_SHAPE_FADE_IN   (AY_ENV_SHAPE_CONTINUE | AY_ENV_SHAPE_ATTACK | AY_ENV_SHAPE_HOLD)

/*
; \
;  \__________________
*/
#define AY_ENV_SHAPE_FADE_OUT  (AY_ENV_SHAPE_CONTINUE | AY_ENV_SHAPE_HOLD)

/*
; \ |\ |\ |\ |\ |\ |\ |
;  \| \| \| \| \| \| \|
*/
#define AY_ENV_SHAPE_SAW1      (AY_ENV_SHAPE_CONTINUE)

/*
;  /| /| /| /| /| /| /|
; / |/ |/ |/ |/ |/ |/ |
*/
#define AY_ENV_SHAPE_SAW2      (AY_ENV_SHAPE_CONTINUE | AY_ENV_SHAPE_ATTACK)

/*
;  /|
; / |__________________
*/
#define AY_ENV_SHAPE_FADE_IN_STOP (AY_ENV_SHAPE_ATTACK)

/* Write a value to an AY register */
extern void ay_write(uint8_t reg, uint8_t val);

/* Read a value from an AY register */
extern uint8_t ay_read(uint8_t reg);


/* Write a value to the AY REG port */
extern void ay_wr_reg(uint8_t val);
/* Read a value form the AY DATA port */
extern uint8_t ay_rd_data();

/* Pay a note on a given channel with a fade out delay.  If delay is zero do
 * not apply envelope shape.
 */
void ay_play_note_delay(uint8_t key, uint8_t channel, uint16_t delay);

#endif //_AY38910_H
