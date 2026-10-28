#include <nabu_retronet.h>
#include <cpm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 1024

void main()
{
  char *hostname = "127.0.0.1";
  uint8_t hostnamelen = 0;
  uint8_t *buffer = NULL;
  uint8_t handle = 0xFF;
  uint32_t read_bytes = 0;

  buffer = malloc(BUFFER_LEN);
  memset(buffer, 0, BUFFER_LEN);
  hostnamelen = strlen(hostname);

  printf("Hello, World\n");
  handle = rn_TCPOpen(hostnamelen, hostname, 3000, 0xFF);
  read_bytes = rn_TCPHandleRead(handle, buffer, 0, 16);
  printf("Read %d bytes from TCP Handle %d\n", read_bytes, handle);
  printf("Buffer contents:\n");
  puts((char*)buffer);
  rn_TCPHandleClose(handle);
}

