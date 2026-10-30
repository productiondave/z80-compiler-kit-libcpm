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
*    The details of tihs library implimentation are taken from NABU-LIB
*    NABU-LIB C Library
*    DJ Sures (c) 2024
*    https://nabu.ca
*
*    Read the NABU-LIB.h file for details of each function in this file.
*
*****************************************************************************
*/

#include <ay-3-8910.h>
#include <nabu_constants.h>
#include <nabu_hcca.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t _rnFS_INT_BACKUP = 0;
volatile uint8_t _rxBuffer[256] = {0};
volatile uint8_t _rxBufferReadPos = 0;
volatile uint8_t _rxBufferWritePos = 0;

void hcca_DiFocusInterrupts() {
  di();
  _rnFS_INT_BACKUP = ay_read(IO_PORTA);
  ay_write(IO_PORTA, INT_MASK_HCCARX);
}

void hcca_DiRestoreInterrupts() {
  ay_write(IO_PORTA, _rnFS_INT_BACKUP);
  ei();
}

// WRITE BYTES AND VALUES
void hcca_DiWrByte(uint8_t c) {
  ay_write(IO_PORTA, INT_MASK_HCCATX);
  ay_wr_reg(IO_PORTB);
  while (ay_rd_data() & 0x04)
    ;
  hcca_write(c);
  ay_write(IO_PORTA, INT_MASK_HCCARX);
}

void hcca_DiWrUInt16(uint16_t val) {

  hcca_DiWrByte(val & 0xff);
  hcca_DiWrByte((val >> 8) & 0xff);
}

void hcca_DiWrInt16(int16_t val) {

  hcca_DiWrByte(val & 0xff);
  hcca_DiWrByte((val >> 8) & 0xff);
}

void hcca_DiWrUInt32(uint32_t val) {

  hcca_DiWrByte(val & 0xff);
  hcca_DiWrByte((val >> 8) & 0xff);
  hcca_DiWrByte((val >> 16) & 0xff);
  hcca_DiWrByte((val >> 24) & 0xff);
}

void hcca_DiWrInt32(int32_t val) {

  hcca_DiWrByte(val & 0xff);
  hcca_DiWrByte((val >> 8) & 0xff);
  hcca_DiWrByte((val >> 16) & 0xff);
  hcca_DiWrByte((val >> 24) & 0xff);
}

void hcca_DiWrString(uint8_t *str) {
  while (*str != 0) {
    hcca_DiWrByte(*str++);
  }
}

void hcca_DiWrBytes(uint16_t offset, uint16_t length, uint8_t *bytes) {

  uint8_t *start = bytes + offset;
  uint8_t *end = start + length;

  while (start != end) {

    hcca_DiWrByte(*start);

    start++;
  }
}

// READ BYTES AND VALUES
uint8_t hcca_DiRdByte() {

  ay_wr_reg(IO_PORTB);
  while (ay_rd_data() & 0x02)
    ;
  return hcca_read();
}

uint16_t hcca_DiRdUInt16() {

  return (uint16_t)hcca_DiRdByte() | ((uint16_t)hcca_DiRdByte() << 8);
}

int16_t hcca_DiRdInt16() {

  return (int16_t)hcca_DiRdByte() | ((int16_t)hcca_DiRdByte() << 8);
}

void hcca_DiRdBytes(uint16_t offset, uint16_t bufferLen, uint8_t *buffer) {

  uint8_t *start = buffer + offset;
  uint8_t *end = start + bufferLen;

  ay_wr_reg(IO_PORTB);
  while (start != end) {

    while (ay_rd_data() & 0x02)
      ;
    *start = hcca_read();

    start++;
  }
}

uint32_t hcca_DiRdUInt32() {

  uint8_t ret[4];
  ret[0] = hcca_DiRdByte();
  ret[1] = hcca_DiRdByte();
  ret[2] = hcca_DiRdByte();
  ret[3] = hcca_DiRdByte();
  return *((uint32_t *)ret);
}

int32_t hcca_DiRdInt32() {

  uint8_t ret[4];
  ret[0] = hcca_DiRdByte();
  ret[1] = hcca_DiRdByte();
  ret[2] = hcca_DiRdByte();
  ret[3] = hcca_DiRdByte();
  return *((int32_t *)ret);
}

uint8_t rn_TCPOpen(uint8_t hostnameLen, char *hostname, uint16_t port,
                   uint8_t fileHandle) {

  // 0xd0
  uint8_t t = 0;
  hcca_DiFocusInterrupts();

  hcca_DiWrByte(0xd0);

  hcca_DiWrByte(hostnameLen);

  hcca_DiWrBytes(0, hostnameLen, (uint8_t*)hostname);

  hcca_DiWrUInt16(port);

  hcca_DiWrByte(fileHandle);

  t = hcca_DiRdByte();

  hcca_DiRestoreInterrupts();

  return t;
}

void rn_TCPHandleClose(uint8_t fileHandle) {

  // 0xd1

  hcca_DiFocusInterrupts();

  hcca_DiWrByte(0xd1);

  hcca_DiWrByte(fileHandle);

  hcca_DiRestoreInterrupts();
}

int32_t rn_TCPHandleSize(uint8_t fileHandle) {

  // 0xd2
  int32_t t = 0;

  hcca_DiFocusInterrupts();

  hcca_DiWrByte(0xd2);

  hcca_DiWrByte(fileHandle);

  t = hcca_DiRdInt32();

  hcca_DiRestoreInterrupts();

  return t;
}

int32_t rn_TCPHandleRead(uint8_t tcpHandle, uint8_t *buffer,
                         uint16_t bufferOffset, uint16_t readLength) {
  // 0xd3

  uint8_t *start = NULL;
  uint8_t *end = NULL;
  uint32_t toRead = 0;
  size_t i;

  hcca_DiFocusInterrupts();

  start = buffer + bufferOffset;

  hcca_DiWrByte(0xd3);

  hcca_DiWrByte(tcpHandle);

  hcca_DiWrUInt16(readLength);

  toRead = hcca_DiRdInt32();

  if (toRead > 0) {

    end = start + toRead;

    do {
      i = 0;
      while ((ay_rd_data() & 0x02) & (++i < 65535))
        ;
      if (i == 0) break;
      else *start = hcca_read();

      start++;

       _rxBufferReadPos++;
    } while (start != end);
  }

  hcca_DiRestoreInterrupts();

  return toRead;
}


int32_t rn_TCPHandleWrite(uint8_t tcpHandle, uint16_t dataOffset,
                          uint16_t dataLen, uint8_t *data) {

  int32_t wrote = 0;
  // 0xd4
  hcca_DiFocusInterrupts();

  hcca_DiWrByte(0xd4);

  hcca_DiWrByte(tcpHandle);

  hcca_DiWrUInt16(dataLen);

  hcca_DiWrBytes(dataOffset, dataLen, data);

  wrote = hcca_DiRdInt32();

  hcca_DiRestoreInterrupts();

  return wrote;
}
