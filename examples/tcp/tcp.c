// vim: set ts=2 sw=2 et:
/*
 * TCP example for the NABU Retronet Internet Adapter.
 *
 * Opens a TCP connection to an echo server.
 * - Reads a string from cpm (cpm_readstr)
 * - writes it to the TCP connection
 * - reads the reply
 * - echos reply.
 */
#include <nabu_retronet.h>
#include <cpm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 256

//Change `hostname` and the port below to match the machine running your server.
char *hostname = "127.0.0.1";
#define PORT 3001

uint8_t hostnamelen = 0;

char *rcv_buffer = NULL;
char *snd_buffer = NULL;
uint8_t bytes_to_send = 0;

/* Handle for the connection.  0xFF means "no connection" and is also what
 * rn_TCPOpen() returns on failure. */
uint8_t handle = 0xFF;

int32_t to_read = 0;    /* bytes waiting on the connection */
int32_t bytes_read = 0; /* bytes actually read */
size_t delay = 0;       /* poll counter */

void main()
{
  /* Receive buffer, cleared so that anything we print is NUL terminated. */
  rcv_buffer = malloc(BUFFER_LEN);
  snd_buffer = malloc(BUFFER_LEN);
  memset(rcv_buffer, 0, BUFFER_LEN);
  memset(snd_buffer, 0, BUFFER_LEN);
  hostnamelen = strlen(hostname);

  printf("Connecting to echo server...\n");

  /* Open the connection.  The last argument is the handle to use, 0xFF asks the
   * adapter to pick a free one for us. */
  handle = rn_TCPOpen(hostnamelen, hostname, PORT, 0xFF);
  if (handle != 0xFF) {
    printf("Connected.  Got handle: %d\n", handle);
    printf("type 'quit' to exit\n");

    while (true) {
      snd_buffer[0] = 8;
      cpm_readstr(snd_buffer);
      bytes_to_send = snd_buffer[1];
      /* cpm_readstr does not NUL terminate; do it so stale data from a longer
       * previous line is not seen. */
      snd_buffer[2 + bytes_to_send] = '\0';
      printf("[->] %s\n", &snd_buffer[2]);
      if (strcmp(&snd_buffer[2], "quit") == 0)  {
        rn_TCPHandleClose(handle);
        return;
      }
      // cpm_readstr records the number of bytes entered in buf[1], the data starts at buf[2]
      // Thats why data_offset below is set to 2.
      rn_TCPHandleWrite(handle, 2, bytes_to_send, (uint8_t*)snd_buffer);

      /* The echo may not have arrived yet.  Poll the handle until some bytes
       * are waiting (or give up), then read only what is there. */
      to_read = 0;
      for (delay = 0; delay < 65535; ++delay) {
        to_read = rn_TCPHandleSize(handle);
        if (to_read != 0) break;
      }
      if (to_read < 0) {
        printf("TCP Handle %d disconnected\n", handle);
        break;
      }
      if (to_read > BUFFER_LEN - 1) to_read = BUFFER_LEN - 1;

      bytes_read = rn_TCPHandleRead(handle, (uint8_t*)rcv_buffer, 0, (uint16_t)to_read);

      if (bytes_read > 0) {
        /* Success.  Terminate the data so it can be printed as a string. */
        rcv_buffer[bytes_read] = '\0';
        printf("[<-] %s\n", rcv_buffer);
      } else if (bytes_read == 0) {
        /* Connected, but nothing arrived in time. */
        printf("No data available on TCP Handle %d\n", handle);
      } else {
        printf("TCP Handle %d disconnected (error %d)\n", handle, bytes_read);
      }
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

