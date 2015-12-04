#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <time.h>

#include "BRS_CDLL.h"
#include "BRS_Schedule_P.h"
#include "BRS_Semaphore.h"



struct Task_Handler
{
  long Itude;
  struct BRS_CDLL_Node Node [1];
  size_t Priority;
  char Coal;
};

#define Task_Handler_Entry(Iterator) \
BRS_CDLL_Node_Entry (Iterator, struct Task_Handler, Node)


int main (int argc, char** argv)
{

  struct Task_Handler Handlers [10];

  struct BRS_Schedule_P Schedule [1];
  struct BRS_CDLL_Node Schedule_Lists [100];

  struct BRS_Semaphore Semaphore [1];
  struct BRS_CDLL_Node Semaphore_Lists [100];

  BRS_Semaphore_Initialize (Semaphore_Lists, 100, Semaphore);
  BRS_Schedule_P_Initialize (Schedule_Lists, 100, Schedule);


  Handlers[0].Priority = 2;
  Handlers[3].Priority = 2;
  BRS_Schedule_P_Insert (Handlers[0].Node, Handlers[0].Priority, Schedule);
  BRS_Schedule_P_Insert (Handlers[3].Node, Handlers[3].Priority, Schedule);
  assert (BRS_Schedule_P_Count (2, Schedule) == 2);

  Handlers[2].Priority = 7;
  Handlers[5].Priority = 7;
  BRS_Schedule_P_Insert (Handlers[2].Node, Handlers[2].Priority, Schedule);
  BRS_Schedule_P_Insert (Handlers[5].Node, Handlers[5].Priority, Schedule);
  assert (BRS_Schedule_P_Count (7, Schedule) == 2);

  Handlers[4].Priority = 0;
  BRS_Schedule_P_Insert (Handlers[4].Node, Handlers[4].Priority, Schedule);
  assert (BRS_Schedule_P_Count (0, Schedule) == 1);

  Handlers[1].Priority = 4;
  Handlers[6].Priority = 4;
  Handlers[7].Priority = 4;
  BRS_Schedule_P_Insert (Handlers[1].Node, Handlers[1].Priority, Schedule);
  BRS_Schedule_P_Insert (Handlers[6].Node, Handlers[6].Priority, Schedule);
  BRS_Schedule_P_Insert (Handlers[7].Node, Handlers[7].Priority, Schedule);
  assert (BRS_Schedule_P_Count (4, Schedule) == 3);

  //Highest priority inserted so far is 7.
  assert (Schedule->Priority == 7);



  int R;

  //First handler to take the semaphore should be successful.
  R = BRS_Semaphore_Take (Handlers[7].Node, Handlers[7].Priority, Semaphore, Schedule);
  assert (R == 1);

  //Second handler to take the semaphore should fail.
  //Semaphore schedule should have one pending handler.
  //Ready schedule should have one less that handler.
  R = BRS_Semaphore_Take (Handlers[1].Node, Handlers[1].Priority, Semaphore, Schedule);
  assert (R == 0);
  assert (BRS_Schedule_P_Count (Handlers[1].Priority, Semaphore->Schedule) == 1);
  assert (BRS_Schedule_P_Count (Handlers[1].Priority, Schedule) == 2);


  //Ready schedule should have one more handlder.
  //Semaphore schedule should have zero pending handler.
  BRS_Semaphore_Release (Semaphore, Schedule);
  assert (BRS_Schedule_P_Count (Handlers[1].Priority, Schedule) == 3);
  assert (BRS_Schedule_P_Count (Handlers[1].Priority, Semaphore->Schedule) == 0);
  assert (Semaphore->Owner == Handlers[1].Node);

  //No more handler is waiting for the semaphore.
  //That is semaphore should then have no owner.
  BRS_Semaphore_Release (Semaphore, Schedule);
  assert (Semaphore->Owner == NULL);


  getchar ();

}
