#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <time.h>

#include "BRS_Schedule_P.h"

struct Test_Handler
{
  struct BRS_CDLL_Node Node [1];
  size_t Time;
  size_t Priority;
};

#define Task_Handler_Entry(Iterator) \
BRS_CDLL_Node_Entry (Iterator, struct Test_Handler, Node)

int main (int argc, char** argv)
{

  struct Test_Handler Handler [10];
  struct BRS_Schedule_P Schedule [1];
  struct BRS_CDLL_Node Schedule_Lists [100];

  BRS_Schedule_P_Initialize (Schedule_Lists, 100, Schedule);

  Handler[0].Priority = 2;
  Handler[1].Priority = 4;
  Handler[2].Priority = 7;
  Handler[3].Priority = 2;
  Handler[4].Priority = 0;
  Handler[5].Priority = 7;
  Handler[6].Priority = 4;
  Handler[7].Priority = 4;

  for (size_t I = 0; I < 8; I = I + 1)
  {
    printf ("Insert ");
    printf ("I %04i ", I);
    printf ("P %04i ", Handler[I].Priority);
    printf ("\n");
    BRS_Schedule_P_Insert (Handler[I].Node, Handler[I].Priority, Schedule);
  }

  printf ("Priority 3 Count %04i\n", BRS_Schedule_P_Count (3, Schedule));
  assert (BRS_Schedule_P_Count (3, Schedule) == 0);
  printf ("Promote Handler[7] to Priority 3\n");
  BRS_Schedule_P_Promote (Handler[7].Node, 3, Schedule);
  printf ("Priority 3 Count %04i\n", BRS_Schedule_P_Count (3, Schedule));
  assert (BRS_Schedule_P_Count (3, Schedule) == 1);


  getchar ();
}
