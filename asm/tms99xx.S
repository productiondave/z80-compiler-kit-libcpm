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

  .export _tms_set_reg
  .export _tms_w_addr
  .export _tms_r_addr
  .export _tms_put
  .export _tms_get
  .export _tms_wait
  .export _tms_g1flush
  .export _tms_g2flush
  .export _tms_txtflush

#ifndef NABU
  .export _tms_status
#endif

#include "io.h"
  .code

;void tms_set_reg(uint8_t reg, uint8_t val);
_tms_set_reg:
  pop   bc  ; return
  pop   hl  ; reg
  pop   de  ; val

  push  de
  push  hl
  push  bc

  push  bc

  ld    c,IO_TMSREG
  out   (c),e
  ld    a,l
  or    0x80
  out   (c),a

  pop   bc
  ret

;void tms_set_w_addr(uint16_t addr);
_tms_w_addr:
#ifdef NABU
  di
#endif
  push  bc
  ld    c,IO_TMSREG
  out   (c),l
  ld    a,h
  or    0x40
  out   (c),a
  pop   bc
#ifdef NABU
  ei
#endif
  ret

;void tms_r_addr(uint16_t addr);
_tms_r_addr:
#ifdef NABU
  di
#endif
  push  bc
  ld    c,IO_TMSREG
  out   (c),l
  out   (c),h
  pop   bc
#ifdef NABU
  ei
#endif
  ret

; void tms_put(uint8_t c);
_tms_put:
  ld    a,l
  out   (IO_TMSRAM),a
  ret

; uint8_t tms_get();
_tms_get:
  in    a,(IO_TMSRAM)
  ld    l,a
  ld    h,0
  ret

; uint8_t tms_wait();
_tms_wait:
#ifdef NABU
  ld    a,(_tms_is_ready)
  or    a
  jr    z,_tms_wait
  xor   a
  ld    (_tms_is_ready), a
  ld    a,(_tms_status)
  ld    l,a
  ld    h,0
  ret
#else
  in    a,(IO_JOY0)   ; read the /INT status via bodge wire 
  and   0x02           ; check U6, pin 4 (D1)
  jr    nz,_tms_wait
  in    a,(IO_TMSREG) ; read the VDP status register to reset the IRQ
  ld    (_tms_status),a
  ld    l,a
  ld    h,0
  ret
_tms_status: .ds 1
#endif

; Make sure that tms_wait is called immediately prior to thes flush routines.

; extern void tms_g1flush(char *buf);
_tms_g1flush:
  push  bc
  ld    de, 0x1400
  ld    bc, 0x300
  jp    tms_write_fast

; extern void tms_g2flush(char *buf);
_tms_g2flush:
  push  bc
  ld    de, 0x3800
  ld    bc, 0x300
  jp    tms_write_fast

; extern void tms_g2flush(char *buf);
_tms_txtflush:
  push  bc
  ld    de, 0x800
  ld    bc, 960

  ;jp    tms_write_fast
  ; FALL THROUGH

tms_write_fast:
#ifdef NABU
  di
#endif
  ld    a,e
  out   (IO_TMSREG),a
  ld    a,d
  or    0x40
  out   (IO_TMSREG),a
#ifdef NABU
  ei
#endif
  ld    d,b
  ld    e,c
  ld    c,IO_TMSRAM
; goldilocks
  ld    b,e
  inc   e
  dec   e
  jr    z,tms_wrfastlp
  inc   d
  ;call  _tms_wait
tms_wrfastlp:
  outi
  ex    de,hl
  ex    de,hl
  jp    nz,tms_wrfastlp
  dec   d
  jp    nz,tms_wrfastlp
  pop   bc
  ret

