#include <cpm.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ARGS 16

int main()
{
  char *argv[MAX_ARGS];
  char c;
  size_t i;
  uint8_t argc = cpm_parse_args(argv, MAX_ARGS);

  for (i=0; i<argc; i++)
  {
    puts("\n");
    puts(argv[i]);
  }
  puts("\n");

  printf("argc = %5d\n", argc);

  printf("argv[argc] = %s\n", (argv[argc] == 0) ? "NULL" : "NOT NULL"); // dereference a null pointer
  puts("\npress a key\n");
  for (;;)
  {
    c = cpm_dc_in();
    if (c) break;
  }
  printf("You pressed : %c\n", c);
  i = printf("Hello, %s\n", "Dave Latham");
  printf("The number of chars written was: %d\n", i);
  printf("You are %d years old.\n", 50);
  printf("Your rom starts at 0x%x.\n", 0xD400);
  printf("Zero looks like 0x%x.\n", 0);
  printf("200-350 = %d.\n", 200-350);
  printf("signed abs(-150) %d.\n", abs(-150));
  printf("signed MAX_INT %d.\n", 65535);
  printf("unsigned MAX_INT %u.\n", 65535);
  return 0;
}
