#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <time.h>

#define Format_Bit_8 "%d%d%d%d%d%d%d%d"
#define Argument_Bit_8(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0)

int dbc (int x)
{
  static int a, r;
  r = (a & x);
  a = x;
  return r;
}

int main (int argc, char** argv)
{
  char line[256];
  char *tmp;
  int a;
  while (1)
  {
    fgets (line, sizeof(line), stdin);
    //printf("%i\n", strlen(line));
    //printf("%s\n", line);
    a = strtol (line, &tmp, 2);

    a = dbc(a);

    //printf ("Decimal ""%i\n", a);
    printf ("Binary  "Format_Bit_8"\n", Argument_Bit_8 (a));
  }

  {
    /* binary string to int */
    //char *tmp;
    //char *b = "0101";
    //printf("%d\n", strtol(b, &tmp, 2));
  }



  return 0;
}
