/*
 * TCP example for the NABU Retronet Internet Adapter.
 *
 * Opens a TCP connection to a server, waits for the server to send some data,
 * reads it into a buffer, prints it and closes the connection.
 *
 * The server is expected to send a short message (for example "hellorld") as
 * soon as a client connects.  Change `hostname` and the port below to match
 * the machine running your server.
 */
#include <nabu_retronet.h>
#include <ay-3-8910.h>
#include <nabu_constants.h>
#include <cpm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 1024

void main()
{
  /* IP address (or hostname) of the server and its length.  The adapter wants
   * the length passed separately because the name is not NUL terminated on the
   * wire. */
  char *hostname = "192.168.0.230";
  uint8_t hostnamelen = 0;

  char *buffer = NULL;

  /* Handle for the connection.  0xFF means "no connection" and is also what
   * rn_TCPOpen() returns on failure. */
  uint8_t handle = 0xFF;

  int32_t to_read = 0;    /* bytes waiting on the connection */
  int32_t bytes_read = 0; /* bytes actually read */
  size_t delay = 0;       /* poll counter */

  /* Receive buffer, cleared so that anything we print is NUL terminated. */
  buffer = malloc(BUFFER_LEN);
  memset(buffer, 0, BUFFER_LEN);
  hostnamelen = strlen(hostname);

  printf("Connecting\n");

  /* Open the connection.  The last argument is the handle to use, 0xFF asks the
   * adapter to pick a free one for us. */
  handle = rn_TCPOpen(hostnamelen, hostname, 3001, 0xFF);
  if (handle != 0xFF) {
    printf("Connected.  Got handle: %d\n", handle);

    /* Data does not necessarily arrive straight away.  Poll the handle until
     * the adapter reports that some bytes are waiting, or give up after a
     * while. */
    for (delay=0; delay<65535; ++delay) {
      to_read = rn_TCPHandleSize(handle);
      if (to_read > 0) break;
    }
    printf("rn_TCPHandleSize returned %d\n", to_read);

    /* Read everything that is waiting.  Arguments are: handle, buffer, offset
     * into the buffer to start writing at, and the number of bytes to read.
     * The return value is the number of bytes read, 0 if there was none, or a
     * negative value if the server disconnected. */
    bytes_read = rn_TCPHandleRead(handle, (uint8_t*)buffer, 0, to_read);

    if (bytes_read > 0) {
      /* Success.  Terminate the data so it can be printed as a string. */
      buffer[to_read] = '\0';
      printf("Read %d bytes from TCP Handle %d\n", to_read, handle);
      printf("Buffer contents:\n");
      printf("%s\n", buffer);
      rn_TCPHandleClose(handle);
      return;
    } else if (bytes_read == 0) {
      /* Connected, but nothing arrived in time. */
      printf("No data available on TCP Handle %d\n", handle);
    } else {
      printf("TCP Handle %d disconnected (error %d)\n", handle, bytes_read);
    }
  } else {
    /* Could not reach the server, so there is no handle to close. */
    printf("Connection could not be established.\n");
    return;
  }

  /* Error paths fall through to here.  Always release the handle so the
   * adapter can reuse it. */
  rn_TCPHandleClose(handle);
  return;
}

