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
#include "nabu_constants.h"

  .export _hcca_write
  .export _hcca_read
  .export _hcca_rd_block
  .export _hcca_rd_resp

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


;uint16_t hcca_rd_block(uint8_t *buf, uint16_t len);
; Read len bytes from the HCCA into buf as fast as possible.  The HCCA only
; holds a single byte, so this loop must be tighter than the incoming byte
; rate or bytes are lost.  Gives up waiting for a byte after ~65k polls.
; Returns the number of bytes NOT read (0 on success).
_hcca_rd_block:
  pop   bc              ; return
  pop   hl              ; buf
  pop   de              ; len
  push  de
  push  hl
  push  bc

  ld    a,IO_PORTB
  out   (IO_AYREG),a    ; select AY port B (HCCA status)

rd_next:
  ld    a,d
  or    e
  jr    z,rd_done
  ld    bc,0
rd_wait:
  in    a,(IO_AYDATA)
  and   0x02
  jr    z,rd_ready
  dec   bc
  ld    a,b
  or    c
  jr    nz,rd_wait
  jr    rd_done         ; timed out
rd_ready:
  in    a,(IO_HCCA)
  ld    (hl),a
  inc   hl
  dec   de
  jr    rd_next
rd_done:
  ex    de,hl           ; HL = bytes remaining
  ret

;uint16_t hcca_rd_resp(uint8_t *buf, uint8_t *len);
; Read a 4 byte little endian length into len[0..3], then IMMEDIATELY read that
; many bytes into buf.  The adapter sends the payload straight after the length
; and the HCCA holds a single byte, so there must be no slow C code between
; the last length byte and the first data byte.  If the length does not fit in
; 16 bits (or is negative) no data is read.
; Returns the number of data bytes NOT read (0 on success).
_hcca_rd_resp:
  pop   bc              ; return
  pop   hl              ; buf
  pop   de              ; len
  push  de
  push  hl
  push  bc

  ld    a,IO_PORTB
  out   (IO_AYREG),a    ; select AY port B (HCCA status)

  ex    de,hl           ; HL = len ptr, DE = buf
  ld    b,4
rr_len:
  in    a,(IO_AYDATA)
  and   0x02
  jr    nz,rr_len
  in    a,(IO_HCCA)
  ld    (hl),a
  inc   hl
  djnz  rr_len

  dec   hl
  ld    a,(hl)          ; len[3]
  dec   hl
  or    (hl)            ; | len[2]
  jr    nz,rr_none
  dec   hl
  ld    b,(hl)          ; len[1]
  dec   hl
  ld    c,(hl)          ; len[0]
  ex    de,hl           ; HL = buf
  ld    d,b
  ld    e,c             ; DE = count
  jr    rd_next
rr_none:
  ld    de,0
  jr    rd_done
