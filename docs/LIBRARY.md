# LIBCPM Reference

There is really no substitute for reading the comments in the files in the
[include](../include) directory.

This document provides a brief overview and some commentary on certain routines
and headers.  Links to the headers are provided in each section.  Please review
the headers themselves for a complete description of each routine.   I have not
listed all the CPM functions here.  They are well documented in the [cpm.h](../include/cpm.h) header
file.

This is not intended to be a fully featured standard library set for CPM and the
FUZIX-Compiler-Kit.  Rather, it's meant to make development with C on the
Z80-Retro! and Z80-Nouveau platforms easier to manage without adding a
significant amount of overhead and bloat.

Compilation is fast on a modern desktop PC and the FUZIX compiler is easy to
install and use.

So while this is not a fully featured C development environment, I think there
is enough there to be enable development of some fairly advanced programs.

If you have questions or concerns, please raise them on the Z80-Retro! Discord
server or on GitHub by raising an issue.

## Main Signature

The signature of the main entry point is `int main()`.

The following points apply:

- the return value is ignored by CP/M 2.2
- `exit(uint8_t)` is provided in `stdlib` if you want to explicitly exit the
program.  If the exit code given is 0, the program returns to CP/M normally.  If
the value is non-zero, the library will print the HEX value before returning to
CP/M.
- Arguments are passed to main with the following signature: `int main(int argc,
  char **argv)`

## CPM

- [cpm.h](../lib/cpm.h)

Almost all the CPM functions are available in the [cpm.h](../include/cpm.h)
header file.  They are all prefixed with `cpm_` and are simple wrappers for the
standard BDOS functions.  Use these functions for low-level CPM integration
where the *stdlib*, *fcntl* and *stdio* routines are not complete enough or fail
your requirements in some other way.

Throughout the examples, you will see examples of `cpm_dc_in()` used to collect
keyboard input.  This is useful in games where a blocking input is not desired.

## STDLIB

- [stdlib.h](../include/stdlib.h)

These functions in STDLIB are some of the simpler ones that might be useful on
an 8-bit system.

    - int8_t brk(void *addr);
    - void *sbrk(uintptr_t incr);
    - int abs(int v);
    - void exit(uint8_t code);
    - void *malloc(size_t size);
    - void free(void *ptr);
    - void itoa(int16_t val, char *str, uint8_t radix);
    - void uitoa(uint16_t val, char *str);
    - void putchar(char c); Translates \n to \r\n
    - void puts(char *s);
    - bool kbhit(); Alias to cpm_const()

## CTYPE

See the [ctype.h](../include/ctype.h) for details.

## FCNTL

- [fcntl.h](../include/fcntl.h)

File abstraction for CPM file IO.  There can only be 4 files open at once.

```c
enum t_flags {
  O_NULL,
  O_CREAT,
};
```

```c
typedef struct s_file {
  FCB fcb;
  char dma[128];
  bool used;
} FILE;
```

## UNISTD

- [unistd.h](../lib/unitd.h)

There are no random access or seek type routines.  These routines take care of
the FCB management to make CPM BDOS calls.  Each open file has its own FCB.
This is why the 4 open file limit exists.  The more files open at once, the more
memory must be reserved.

> Note: The FILE type also carries a pointer to an initialized DMA block of
> memory which means there shouldn't ever be a situation where DMA operations
> for one file conflict with those of another.

    - int write(uint8_t fd, void *buf, size_t count);
    - int read(uint8_t fd, void *buf, size_t count);
    - int8_t close(int8_t fd);
    - size_t f_size(int8_t fd);

## STDINT

- [stdint.h](../lib/stdint.h)

The list of supported types are given in [stdint.h](../include/stdint.h).

## STDDEF

- [stddef.h](../lib/stddef.h)

Defines, `NULL`, `offsetof` and `size_t`.  This library considers `size_t` to be
an unsigned 16 bit integer such that it can hold a value as large as the CPU
addressable range.

## STDARG

- [stdarg.h](../lib/stdarg.h)

## STDIO

- [stdio.h](../lib/stdio.h)

    - int printf(const char *format, ...);
    - int sprintf(char *dst, const char *format, ...);

Only the following format specifiers are supported:

```text
%c - single 8 bit character
%s - Null terminated string
%u - Unsigned INT ( 16 bits - DECIMAL)
%d - Signed INT ( 16 bits - DECIMAL)
%x - Unsigned INT (16 bits - HEX)
```

Zero padding is supported in `%x` like this: `%2x`.

Left-space padding is supported in the various decimal formatters like this: `%6d`.

### STRING

- [string.h](../lib/string.h)

The string functions are all pretty much as you'd expect them to be.  This is
the full list.

    - char toupper(char c);
    - char to(char c);
    - void *memcpy(void *dst, void *src, size_t n);
    - void *memset(void *dest, uint8_t c, size_t n);
    - int8_t memcmp(void *s1, void *s2, size_t n);
    - int8_t strcmp(const char *s1, const char *s2);
    - int8_t strncmp(const char *s1, const char *s2, size_t n);
    - char *strchr(const char *str, int8_t c);
    - char *strrchr(const char *s, int c);
    - char *strcpy(char *dst, const char *src);
    - size_t strlen(const char *s);
    - char *strtok(char *s, const char *delim);
    - size_t strspn(const char *s, const char *accept);
    - char *strpbrk(const char *str, const char *set);


## TMS9xx8

- [tms.h](../include/tms.h)
- [tms_patterns.h](../include/tms_patterns.h)

### General
VDP Colors:

```text
TRANSPRAENT, BLACK, MEDIUM_GREEN, LIGHT_GREEN, DARK_BLUE, LIGHT_BLUE,
DARK_RED, CYAN, MEDIUM_RED, LIGHT_RED, DARK_YELLOW, LIGHT_YELLOW, DARK_GREEN,
MAGENTA, GRAY, WHITE
```

VDP Graphics Modes:

```test
MODE_G1, MODE_G2, MODE_MC, MODE_TEXT
```

VDP Sprite Structrue and array:

```c
typedef struct sprite_s {
  int y;          // location in Y axis (can be negative)
  int x;          // location in X axis (can be negative - value depends on ECB)
  uint8_t pattern;    // pattern ID in sprites attribute table
  uint8_t color;      // 0xE...CCCC : early clock bit + COLOR
} Sprite;

extern Sprite sprites[32];   // this array of sprites is flushed to the sprite
                      // attribute table in tms_flush_spr() until a Y value of
                      // 0xD0 is encountered.
```

Framebuffer which is used by the various plot routines to draw tiles.  The
framebuffer must be streamed into the VDP as soon as possible after a VDP VSYNC
signal is recieved.  The `tms_??flush` routines listed below will copy the
framebuffer to VDPRAM when called.

In order to know when the VDP VSYNC signal is ready, the `tms_wait` routine
should be used.  This is a blocking routine that returns only when the VSYNC
signal is present on the VDP status register.

The following variables are globally available and will be initilaised correctly
by the `tms_init_??` routines.

```c
extern uint16_t tms_name_tbl;      // REG: 2
extern uint16_t tms_color_tbl;     // REG: 3
extern uint16_t tms_patt_tbl;      // REG: 4
extern uint16_t tms_sp_attr_tbl;   // REG: 5
extern uint16_t tms_sp_patt_tbl;   // REG: 6

extern uint8_t  tms_mode;

extern uint16_t tms_n_tbl_len;
extern uint16_t tms_c_tbl_len;
```

### Init Routines

The VDP Init routines are responsible for setting up the VDP registers for
specific graphics modes.  They also allocate the framebuffer memory and set the
above variables according to the graphics mode being established.

    - void tms_init_g1(uint8_t fg, uint8_t bg, bool largesp, bool mag);
        - 32x24 tiled graphics mode, 32 colors in groups of 8 tiles
    - void tms_init_g2(uint8_t fg, uint8_t bg, bool largesp, bool mag);
        - 256x192 "bitmap" mode.  2 colours per 8 pixel line.
    - void tms_init_mc(uint8_t fg, uint8_t bg, bool largesp, bool mag);
        - 64x48 Multi-color mode.
    - void tms_init_text(uint8_t fg, uint8_t bg);
        - 40x24 monochrome text mode.  No border.

There is a special function for initializing graphics mode 2 into bitmap mode.
This routine will set up the name table such that any write to a pattern or
color in the pattern and color table will result in updates to specific pixels
in the 256x192 space.

    - void tms_init_bitmap_mode();

For working directly with VDP registers.

    - void tms_set_reg(uint8_t reg, uint8_t val);
    - void tms_r_addr(uint16_t addr);
    - void tms_w_addr(uint16_t addr);

### Initialising Data Routines

Routines for initializing data in VDP memory.   For example loading a tileset or
a sprite sheet.

> Note: The `tms_patterns.h` file contains a default font for use.  Use it or
> use any other font you like.  This fon't is not really appropriate for
> TEXT_MODE.  It will work, but it was not *designed* for TEXT_MODE.

    - void tms_load_pat(char *pattern, size_t len);
        - load pattern table with font or tileset.
    - void tms_load_col(char *color, size_t len);
        - load the colour table with values.
    - void tms_init_sprites();
        - usually called by the vdp graphics mode init routines.
    - void tms_load_spr(char *sprites, size_t len);
        - load the sprite sheet
    - void tms_flush_sprites();
        - Copy the srpite attribute table to VDP memory.  This is when sprites will
          be updated.  You can also use tms_put to update the sprite attribute table
          directly, but this method is more convenvient.


### Direct Acccess to VDP Memory

Read and write data to video memory at the currently defined address.

    - void tms_put(uint8_t b);
    - uint8_t tms_get();

### Framebuffer Routines

Routines for working with the framebuffer.

    - void tms_fill_buf(const char c);
    - void tms_plot_g2(uint8_t x, uint8_t y, uint8_t fg, uint8_t bg);
    - bool tms_plot_mc(uint8_t x, uint8_t y, uint8_t c);
        - This routine will return true if the pixel being drawn replaces any colour
          on the screen that's not transparent or black.
    - void tms_put_char(uint8_t x, uint8_t y, char c);
    - void tms_puts_xy(uint8_t x, uint8_t y, char *s);

Routines for copying the framebuffer to VDP RAM.

    - uint8_t tms_wait();
    - uint8_t tms_status;
        - a global variable (also returned by tms_wait(), that holds the last value
          of the VDP Status Register)
    - void tms_g1flush(char *buf);
    - void tms_g2flush(char *buf);
    - void tms_mcflush(char *buf);
    - void tms_txtflush(char *buf);

## AY-3-8910

- [ay-3-8910.h](../incude/ay-3-8910.h)
- [ay-notes.h](../include/ay-notes.h)

AY-3-8910 is not available on the RETRO platform.  There is no harm in including
it as the port allocated for the AY-3-8910 is reserved for future use.  Any
program that emits AY-3-8910 register writes will succeed even if the device at
that port is not present.

The header file contains a few useful defines for the various bitfields and
registers.  It's best to just look at the header file to know what these are.

    - void ay_write(uint8_t reg, uint8-t val);
        - Low level register write operations
    - void ay_play_note_delay(uint8_t key, uint8_t channel, uint16_t delay);
        - Play a specific note on a specific channel with a decay period determined
          by delay

### AY-NOTES

The NOUVEAU platform is clocked at 1.793712 MHz.  This clock frequency is then
used to calculate the tone generator periods required to achieve the various
notes on a piano.  You can find the
[piano-notes-worksheet.ods](./piano-notes-worksheet.ods) in the docs folder to
see how these values are calculated.

The notes themselves are then organised into two arrays of 8 bit values.  A
course period and a fine period.  The `ay_play_note_delay` routine accepts a
`key` value as an input.  This key is the index of the notes arrays.

The [ay-notes.h](../include/ay-notes.h) header file should make this clear.

## Joystick

- [joy.h](../include/joy.h)

Joystick support is currently limited to Atari style joysticks.  There is one
function which returns the current status of a joystick given by the parameter.

    - uint8_t joy(uint8_t i);

## Rand

- [rand.h](../include/rand.h)

Pseudo random number generator.

    - void srand(uint16_t seed);
    - uint16_t rand();
    - uint8_t fastrand(); much faster but less effective random number generator

<!-- vim: set ft=markdown tw=80 cc=80: -->
