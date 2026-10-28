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
; Assembly routines for dealing with AY-3-8910 registers
;
;****************************************************************************

#include "io.h"

  .export _ay_write
  .export _ay_read
  .export _ay_wr_reg
  .export _ay_rd_data

  .code

;void ay_write(uint8_t reg, uint8_t val);
_ay_write:
  pop   bc  ; return
  pop   hl  ; reg
  pop   de  ; val

  push  de
  push  hl
  push  bc

  push  bc

  ld    c,IO_AYREG
  out   (c),l
  ld    c,IO_AYDATA
  out   (c),e
  pop   bc

  ret

;uint8_t ay_read(uint8_t reg);
_ay_read:
  ld    a,l
  out   (IO_AYREG),a
  in    a,(IO_AYDATA)
  ld    l,a
  ld    h,0
  ret

; void ay_wr_reg(uint8_t val);
_ay_wr_reg:
  ld    a,l
  out   (IO_AYREG),a
  ret

; uint8_t ay_rd_data();
_ay_rd_data:
  in    a,(IO_AYDATA)
  ld    l,a
  ld    h,0
  ret
