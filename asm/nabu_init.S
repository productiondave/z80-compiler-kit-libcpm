; vim: set ft=z80 ts=2 sw=2 et:
;****************************************************************************
;
;    Copyright (C) 2025 David Latham
;
;    This library is free software; you can redistribute it and/or
;    modify it under the terms of the GNU Lesser General Public
;    License as published by the Free Software Foundation; either
;    version 2.1 of the License, or (at your option) any later version.
;
;    This library is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;    Lesser General Public License for more details.
;
;    You should have received a copy of the GNU Lesser General Public
;    License along with this library; if not, write to the Free Software
;    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
;    USA
;
; https://github.com/linuxplayground/z80-compiler-kit-libcpm
;
; Assembly routines for dealing with TMS VDP Registers
;
;****************************************************************************


#ifdef NABU

#include "io.h"
#include "nabu_constants.h"

  .export init_nabu
  .export _tms_is_ready
  .export _tms_status

  .code

;===============================================================================
; Read a register from the AY-3-8910
; INPUT: A = Register to read
; OUTPUT: A = value
; CLOBBERS: AF
;===============================================================================
ay_read:
  out   (IO_AYREG),a
  in    a,(IO_AYDATA)
  ret

;===============================================================================
; Write a value to the AY-3-8910 register
; INPUT: B = Register to write to, C = value to write
; OUTPUT: void
; CLOBBERS: none
;===============================================================================
ay_write:
  ld    a,b
  out   (IO_AYREG),a
  ld    a,c
  out   (IO_AYDATA),a
  ret


init_nabu:
  ;Turn off the rom
  ld    a,0x03          ;CONTROL_ROMSEL|CONTROL_VDOBUF
  out   (IO_CONTROL),a
  di    ; disable interrupts

  im    2
  ld    hl, isrVdp
  ld    (0xff00+6), hl

  ld    b,0x07
  ld    c,01111111b
  call  ay_write        ; configure PORTA for writing and port B for reading

  ld    a,IO_PORTA
  call  ay_read         ; get the current interrupt mask from AY Port A
  or    0x30            ; INT_MASK_KEYBOARD|INT_MASK_VDP
  ld    c,a
  ld    b,IO_PORTA
  call  ay_write
  ei    ; enable interrupts
  ret

isrVdp:
  push  af
  in    a,(IO_TMSREG)
  ld    (_tms_status),a
  ld    a,0x01
  ld    (_tms_is_ready), a
  pop   af
  ei
  reti


origint:                  .ds 1
_tms_is_ready:            .ds 1
_tms_status:              .ds 1

#endif
