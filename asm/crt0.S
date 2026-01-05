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
;****************************************************************************

COMMAND_TAIL_LEN equ 0x80
COMMAND_TAIL     equ 0x81
MAX_ARGS         equ 16

  .bss

_argc:        .word 0
_argv_table:  .ds MAX_ARGS * 2
_cmdcopy:     .ds 128

  .code
#ifndef NABU
  di    ; disable interrupts on everything.
#endif
  ld    hl,(0x0006)
  ;dec   hl
  ld    sp,hl
#ifdef NABU
  call  init_nabu
#endif
  call  __init_sys
  call  copy_cmd_tail
  call  parse_cpm_tail
  ld    hl,_argv_table
  push  hl
  ld    hl,(_argc)
  push  hl
  call  _main
  jp    0

copy_cmd_tail:
  ld    a, (COMMAND_TAIL_LEN)
  or    a
  ret   z
  ld    c, a
  ld    b, 0
  ld    de, _cmdcopy
  ld    hl, COMMAND_TAIL
  ldir
  ret

parse_cpm_tail:
  ld    a,(COMMAND_TAIL_LEN)    ; get length of tail
  or    a
  jr    z, no_args
  ld    b, a
  ld    hl, _cmdcopy
  ld    ix, _argv_table
  ld    de, 0

  ; handle argv[0] - CP/M doesn't provide the program name
  ld    hl, dummy_name
  ld    (ix+0),l
  ld    (ix+1),h
  inc   de
  inc   ix
  inc   ix

  ld    hl, _cmdcopy ; reset HL to start of tail

parse_loop:
  ; 1. skip leading whitespace
skip_space:
  ld    a,(hl)
  cp    ' '
  jr    nz,find_word_end
  inc   hl
  djnz  skip_space
  jr    finish_parse

find_word_end:
  ; 2. found a word! save address to argv table
  ld    (ix+0),l
  ld    (ix+1),h
  inc   de          ; increment argc
  inc   ix
  inc   ix

scan_word:
  ld    a,(hl)
  cp    ' '
  jr    z,terminate_word
  inc   hl
  djnz  scan_word
  jr    finish_parse

terminate_word:
  ld    (hl), 0
  inc   hl
  djnz  parse_loop

finish_parse:
  ld    (_argc),de
  ret

no_args:
  ld    hl,1
  ld    (_argc),hl
  ld    hl,dummy_name
  ld    (_argv_table),hl
  ret

.data
dummy_name: .ascii"program"
            .byte 0
