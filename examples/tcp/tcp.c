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
  char *hostname = "192.168.0.230";
  uint8_t hostnamelen = 0;
  char *buffer = NULL;
  uint8_t handle = 0xFF;
  int32_t to_read = 0;
  int32_t bytes_read = 0;
  size_t delay = 0;


  buffer = malloc(BUFFER_LEN);
  memset(buffer, 0, BUFFER_LEN);
  hostnamelen = strlen(hostname);

  printf("Connecting\n");
  handle = rn_TCPOpen(hostnamelen, hostname, 3001, 0xFF);
  if (handle != 0xFF) {
    printf("Connected.  Got handle: %d\n", handle);

    for (delay=0; delay<65535; ++delay) {
      to_read = rn_TCPHandleSize(handle);
      if (to_read > 0) break;
    }
    printf("rn_TCPHandleSize returned %d\n", to_read);

    bytes_read = rn_TCPHandleRead(handle, (uint8_t*)buffer, 0, to_read);

    if (bytes_read > 0) {
      buffer[to_read] = '\0';
      printf("Read %d bytes from TCP Handle %d\n", to_read, handle);
      printf("Buffer contents:\n");
      printf("%s\n", buffer);
      rn_TCPHandleClose(handle);
      return;
    } else if (bytes_read == 0) {
      printf("No data available on TCP Handle %d\n", handle);
    } else {
      printf("TCP Handle %d disconnected (error %d)\n", handle, bytes_read);
    }
  } else {
    printf("Connection could not be established.\n");
    return;
  }
  rn_TCPHandleClose(handle);
  return;
}

