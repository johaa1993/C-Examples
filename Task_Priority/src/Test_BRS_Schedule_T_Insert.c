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
  struct BRS_CDLL_Node Node [1];
  size_t Time;
  long Itude;
};

int main (int argc, char** argv)
{

  struct Test_Handler Handler [10];
  struct BRS_Schedule_T Schedule [1];
  struct BRS_CDLL_Node * Node;

  BRS_CDLL_Link_Self (Schedule->List);

  Handler[0].Time = 33;
  Handler[1].Time = 4;
  Handler[2].Time = 7;
  Handler[3].Time = 3;
  Handler[4].Time = 55;
  Handler[5].Time = 7;
  Handler[6].Time = 4;
  Handler[7].Time = 4;

  Node = BRS_Schedule_T_Pull_Temporal (Schedule, 999);
  assert (Node == NULL);

  for (size_t I = 0; I < 8; I = I + 1)
  {
    printf ("Insert ");
    printf ("I %04i ", I);
    printf ("P %04i ", Handler[I].Time);
    printf ("\n");
    BRS_Schedule_T_Insert (Handler[I].Node, Schedule);
  }

  Node = BRS_Schedule_T_Pull_Temporal (Schedule, 2);
  assert (Node == NULL);

  Node = BRS_Schedule_T_Pull_Temporal (Schedule, 10);
  assert (Node == Handler[3].Node);

  Node = BRS_Schedule_T_Pull_Temporal (Schedule, 10);
  assert (Node == Handler[1].Node);

  getchar ();
}
