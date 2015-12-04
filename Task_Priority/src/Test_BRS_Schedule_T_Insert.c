#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <time.h>

#include "BRS_Schedule_T.h"

struct Test_Handler
{
  char Coal;
  struct BRS_Schedule_T_Node Node [1];
  long Itude;
};

int main (int argc, char** argv)
{

  struct Test_Handler Handler [10];
  struct BRS_Schedule_T Schedule [1];

  BRS_CDLL_Link_Self (Schedule->List);

  printf ("First %04i\n", BRS_Schedule_T_First (Schedule));
  assert (BRS_Schedule_T_First (Schedule) == 0);

  Handler[0].Node->Time = 2;
  Handler[1].Node->Time = 4;
  Handler[2].Node->Time = 7;
  Handler[3].Node->Time = 2;
  Handler[4].Node->Time = 55;
  Handler[5].Node->Time = 7;
  Handler[6].Node->Time = 4;
  Handler[7].Node->Time = 4;

  for (size_t I = 0; I < 8; I = I + 1)
  {
    printf ("Insert ");
    printf ("I %04i ", I);
    printf ("P %04i ", Handler[I].Node->Time);
    printf ("\n");
    BRS_Schedule_T_Insert (Handler[I].Node, Schedule);
  }

  printf ("First %04i\n", BRS_Schedule_T_First (Schedule));
  assert (BRS_Schedule_T_First (Schedule) == 2);


  getchar ();
}
