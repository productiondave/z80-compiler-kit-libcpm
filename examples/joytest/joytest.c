#include <stdio.h>
#include <joy.h>
#include <cpm.h>


int main()
{
  char c;
  bool running = true;
  uint8_t j0 = 0xFF;
  uint8_t j1 = 0xFF;
  printf("\nJOYSTICK TESTER: (ESCAPE TO QUIT)\n");

  while (running)
  {
    c = cpm_dc_in();
    if (c)
    {
      printf("\nKEY: %02x = %c", c, c);
      if (c == 0x1b) {
        running = false;
      }
    }
    j0 = joy(0);
    if (j0 != j1)
    {
      printf("\nJOY: %02x",j0);
      j1 = j0;
    }
  }

  return 0;
}
