#include <cpm.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ARGS 16

int main(int argc, char **argv)
{
  char c;
  size_t i;

  for (i=0; i<argc; i++)
  {
    puts(argv[i]);
    puts("\n");
  }

  puts("\n");

  printf("argc = %5d\n", argc);
  printf("argv[argc] = %s\n", (argv[argc] == 0) ? "NULL" : "NOT NULL"); // dereference a null pointer

  return 0;
}
