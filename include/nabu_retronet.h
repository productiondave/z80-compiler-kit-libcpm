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
 * Original work by:
 * NABU-LIB C Library - MAKE SOMETHING
 * DJ Sures (c) 2024
 * https://nabu.ca
 *
 * https://github.com/linuxplayground/z80-compiler-kit-libcpm
 *
 *****************************************************************************
 */

/*
 * This library is for dealing with the HCCA for retronet operations on the
 * nabu
 */

#ifndef _NABU_RETRONET_H
#define _NABU_RETRONET_H
#include <stdint.h>
#include <nabu_hcca.h> /* for hcca_rd_block() and hcca_rd_resp() */

/* Note: the HCCA holds a single byte and the adapter streams data back to back,
 * so reading a payload in C is too slow and loses bytes.  Use hcca_rd_block()
 * for any bulk read.  rn_TCPHandleRead() already does. */

/**************************************************************************
 * Connects to the host and returns a file handle that will be used for all file
 *functions.
 *
 * 1) Specify a hostname and receive an automatic file handle. For example...
 *    uint8_t fileHandle = rn_requestStoreOpenFile(10, "myFile.txt",
 *OPEN_FILE_FLAG_READWRITE, 0xff);
 *
 * 2) Specify a hostname and specify your a handle. For example...
 *    uint8_t fileHandle = rn_requestStoreOpenFile(22,
 *"a:\\personal\\myFile.txt", OPEN_FILE_FLAG_READWRITE, 0xff);
 *
 * When you are done with the connection, you must Close the connection to
 * release the handle with with rn_tcpClose(); If the NABU is reset and the
 * program therefore cannot close the connection, the Internet Adapter will close
 * all connections when an INIT command is received.
 *
 * - hostnameLen is the length of the filename to open
 * - hostname is a pointer to the hostname (127.0.0.1 for example). 
 * - port is the TCP port on the host to connect to
 * - fileHandle can be a handle that you specify or 0xff (255) for the server to
 *   assign one for you
 */

/* Returns a handle to the connection for other TCP functions -or- returns 0xff
 * if connection could not be established
 */
uint8_t rn_TCPOpen(uint8_t hostnameLen, char *hostname, uint16_t port,
                   uint8_t fileHandle);
/* Closes and releases the connection with the specified Handle. The handle can
 * be used again for another connection once it has been released.
 *
 * When you are done with the connection, you must Close the handle to release
 * the handle with with rn_TCPCloseFile(); If the NABU is reset and the program
 * therefore cannot close the connection, the Internet Adapter will close all
 * connections when an INIT command is received.
 */
void rn_TCPHandleClose(uint8_t fileHandle);

/* Get the length of readable data for the TCP handle.
 * A -1 will be returned if there is no connection
 */
int32_t rn_TCPHandleSize(uint8_t fileHandle);

/* Read data from the specified tcp client
 *
 * - Handle is the obtained by rn_TCPOpen()
 * - buffer is a pointer to a buffer that the data will be written to.
 * - bufferOffset is the offset within the buffer where the data will be written. Use 0 if you're
 *   writing to the beginning of the buffer, for example.
 * - readLength is the amount of data that you will be reading.
 *
 * Returns: the number of bytes read or -1 if client is disconnected
 */
int32_t rn_TCPHandleRead(uint8_t tcpHandle, uint8_t *buffer,
                         uint16_t bufferOffset, uint16_t readLength);

/* Write data to the host with the handle. Returns the number of bytes written, or 0 if disconnected.
 *
 * - tcpHandle is the obtained by rn_TCPOpen()
 * - dataOffset is the offset of the data that will be written
 * - dataLen is the length of data that will be written 
 * - data is a pointer to the data
 *
 * Returns: the number of bytes written or -1 if client is disconnected
 */
int32_t rn_TCPHandleWrite(uint8_t tcpHandle, uint16_t dataOffset,
                          uint16_t dataLen, uint8_t *data);

#endif //_NABU_RETRONET_H
