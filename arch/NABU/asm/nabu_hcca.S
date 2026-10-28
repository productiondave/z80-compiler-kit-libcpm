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
; Assembly routines for dealing with HCCA
;
;****************************************************************************

#include "io.h"

  .export _hcca_write
  .export _hcca_read

  .code

;void hcca_write(uint8_t val);
_hcca_write:
  ld    a,l
  out   (IO_HCCA),a
  ret

;uint8_t hcca_read();
_hcca_read:
  in    a,(IO_HCCA)
  ld    l,a
  ld    h,0
  ret

