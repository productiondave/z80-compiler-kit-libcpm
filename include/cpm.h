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

#ifndef _CPM_H
#define _CPM_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* FCB */
typedef struct fcb_t {
  uint8_t dr; // Drive
  char f[8];  // 8 char filename
  char t[3];  // 3 char file ext
  uint8_t ex;
  uint8_t s1;
  uint8_t s2;
  uint8_t rc;
  uint8_t al[16]; // doubles as new name in f_rename call
  uint8_t cr;
  size_t rn;
  uint8_t rn_o; // holds overflow of random access address
} FCB;

/* prepares an fcb for f_open, f_make, f_search etc.
 * returns false if invalid
 */
bool set_fcb_file(FCB *fcb, const char *filename);

/*
 ****************************************************************************
 * All of the following extern functions are to small assembly code
 * wrapper calls into CPM via the standard BDOS API
 ****************************************************************************
 */

/* Write char C to the terminal */
extern void cpm_conout(uint_fast8_t c);

/* Wait for character from the terminal, blocks */
extern char cpm_conin();

/* Returns characater if one is available else zero
 * processed.  This is a nonblocking call.
 */
extern char cpm_dc_in();

/* Output a character direct via BIOS CONOUT
 */
extern void cpm_dc_out(uint_fast8_t c);



/* CPM Writestr expects chars to be terminated with a `$` */
extern void cpm_writestr(char *s);

/* Read a line of text input from the terminal until the ENTER key is pressed.
 * The first position in the in the buf array at `buf[0]` contains the maximum
 * length to read. On return the second position buf[1] contains the actual
 * number of bytes read.
 */
extern void cpm_readstr(char *buf);

/* Returns true if a character is waiting in the serial terminal to be
 * processed.  This is a nonblocking call.
 */
extern bool cpm_constat();

/* Resets disc drives.
 * Logs out all discs and empties disc buffers. Sets the currently selected
 * drive to A:. Any drives set to Read-Only in software become Read-Write;
 * replacement BDOSses tend to leave them Read-Only. Logs in drive A: and
 * returns 0FFh if there is a file present whose name begins with a $,
 * otherwise 0.
 */
extern uint_fast8_t cpm_drv_allreset();

/* Selectively reset drives by given bitmap.
 * MSB = drive P, LSB = drive A
 * Returnes 0 if OK, else 0xFF if error
 */
extern uint_fast8_t cpm_drv_reset(size_t bitmap);

/* Sets the currently selected drive to the drive
 * logs in the disc. Returns 0 if successful or 0FFh if error.
 */
extern uint_fast8_t cpm_drv_set(uint_fast8_t drive);

/* returns currently selected drive. 0 => A; 1 => B etc.
 */
extern uint_fast8_t cpm_drv_get();

/* Set the current user number.  num should be 0-15.
 * Returns set number.
 */
extern uint_fast8_t cpm_f_usernum(uint_fast8_t num);

/* Opens a file to read or read/write.
 * The FCB is a 36-byte data structure, most of which is maintained by CP/M.
 * The FCB should have its DR, Fn and Tn fields filled in, and the four fields
 * EX, S1, S2 and RC set to zero. Note that CR should normally be reset to zero
 * if sequential access is to be used. On return from this function, A is 0FFh
 * for error, or 0-3 for success.
 */
extern uint_fast8_t cpm_f_open(FCB *fcb);

/* closes a file, and writes any pending data.
 * This function should always be
 * used when a file has been written to. On return from this function, result is
 * 0FFh for error, or 0-3 for success.*/
extern uint_fast8_t cpm_f_close(FCB *fcb);

/* Deletes all directory entries matching the specified filename.
 * The name can contain ? marks. Returns A=0FFh if error, otherwise 0-3
 */
extern uint_fast8_t cpm_f_delete(FCB *fcb);

/* Creates the file specified.
 * Returns 0xFF if directory is full.  If file exists already the default
 * action is to return to the command prompt... ??
 */
extern uint_fast8_t cpm_f_make(FCB *fcb);

/* Renames the file specified to the new name, stored at FCB+16. This function
 * cannot rename across drives so the "drive" bytes of both filenames should be
 * identical. Returns A=0-3 if successful; A=0FFh if error.
 *
 * FCB+16 == fcb->al which is 16 bytes long. The values in these 16 bytes
 * should match the same format as the first 16 bytes of the FCB struct but
 * with the desired new name.
 * TODO: Check that this is accurate.
 */
extern uint_fast8_t cpm_f_rename(FCB *fcb);

/* Set DMA offset
 * Set the Direct Memory Access address; a pointer to where CP/M should read or
 * write data.
 */
extern void cpm_f_dmaoff(char *dma);

/* Search for file.
 * Search for the first occurrence of the specified file; the filename should
 * be stored in the supplied FCB. The filename can include ? marks, which match
 * anything on disc. If the first byte of the FCB is ?, then any directory
 * entry (including disc labels, date stamps etc.) will match. The EX byte is
 * also checked; normally it should be set to zero, but if it is set to ? then
 * all suitable extents are matched.

 Returns A=0FFh if error
*/
extern uint_fast8_t cpm_f_sfirst(FCB *fcb);

/* Search for File
 * This function should only be executed immediately after function f_sfirst or
 * another invocation of this function. No other disc access functions should
 * have been used. Behaves exactly as number f_sfirst, but finds the next
 * occurrence of the specified file after the one returned last time.
 */
extern uint_fast8_t cpm_f_snext(FCB *fcb);

/* Load a 128 byte record at the previously specified DMA address. Values
 * returned are:
 * - 0: OK,
 *   1: end of file,
 *   9: invalid FCB,
 *   10: (CP/M) media changed,
 *   0FFh: hardware error.
 */
extern uint_fast8_t cpm_f_read(FCB *fcb);

/* Write a 128 byte record from the previously defined DMA address. Values
 * returned are:
 * - 0: OK,
 *   1: directory full,
 *   2: disc full,
 *   9: invalid FCB,
 *  10: (CP/M) media changed
 */
extern uint_fast8_t cpm_f_write(FCB *fcb);

/* Set the random record count bytes of the FCB to the number of 128-byte
 * records in the file.
 */
extern void cpm_f_size(FCB *fcb);

/* Update the random record count bytes of the FCB to the number of the last
 * record read/written by the sequential I/O calls.
 */
extern void cpm_f_randrec(FCB *fcb);

/* Random access read record.
 * Read the record specified in the random record count area of the FCB, at the
 * DMA address. The pointers in the FCB will be updated so that the next record
 * to read using the sequential I/O calls will be the record just read. Error
 * numbers returned are:
 *
 * 0: OK
 * 1: Reading unwritten data
 * 4: Reading unwritten extent (a 16k portion of file does not exist)
 * 6: Record number out of range
 * 9: Invalid FCB
 * 10: Media changed (CP/M);
 */
extern uint_fast8_t cpm_f_readrand(FCB *fcb);

/* Random access write record.
 * Write the record specified in the random record count area of the FCB, at
 * the DMA address. The pointers in the FCB will be updated so that the next
 * record to write using the sequential I/O calls will be the record just
 * written. Error numbers returned are:
 *
 * 0: OK
 * 2: Disc full
 * 3: Cannot close extent
 * 5: Directory full
 * 6: Record number out of range
 * 9: Invalid FCB
 * 10: Media changed
 *
 * If the record indicated is beyond the end of the file, the record will be
 * written and the file may contain a gap; attempting to read this gap may give
 * "reading unwritten data" errors, or nonsense
 */
extern uint_fast8_t cpm_f_writerand(FCB *fcb);

/* Write random zero fill
 * Same as f_writerand but if the write was to a newly allocated disc block,
 * the remainder of the block is filled with zeroes.
 */
extern uint_fast8_t cpm_f_writezf(FCB *fcb);

#endif //_CPM_H
