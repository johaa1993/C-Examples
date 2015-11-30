#include "Test_BRS_Semaphore.h"

struct Task_Handler
{
  int Dummy;
  struct BRS_Lists_DL_Node Node [1];
  size_t Priority;
};

#define Task_Handler_Entry(Iterator) \
BRS_Lists_DL_Node_Entry (Iterator, struct Task_Handler, Node)


void Test_BRS_Semaphore_1 ()
{

  struct Task_Handler Handlers [10];

  struct BRS_Schedule_P Schedule [1];
  struct BRS_Lists_DL_Node Schedule_Lists [100];

  struct BRS_Semaphore Semaphore [1];
  struct BRS_Lists_DL_Node Semaphore_Lists [100];

  BRS_Semaphore_Initialize (Semaphore_Lists, 100, Semaphore);
  BRS_Schedule_P_Initialize (Schedule_Lists, 100, Schedule);

  Handlers[0].Priority = 2;
  Handlers[1].Priority = 4;
  Handlers[2].Priority = 7;
  Handlers[3].Priority = 2;
  Handlers[4].Priority = 0;
  Handlers[5].Priority = 7;
  Handlers[6].Priority = 4;
  Handlers[7].Priority = 4;

  BRS_Schedule_P_Insert (Handlers[0].Node, Handlers[0].Priority, Schedule);
  BRS_Schedule_P_Insert (Handlers[1].Node, Handlers[1].Priority, Schedule);
  BRS_Schedule_P_Insert (Handlers[2].Node, Handlers[2].Priority, Schedule);
  BRS_Schedule_P_Insert (Handlers[3].Node, Handlers[3].Priority, Schedule);
  BRS_Schedule_P_Insert (Handlers[4].Node, Handlers[4].Priority, Schedule);
  BRS_Schedule_P_Insert (Handlers[5].Node, Handlers[5].Priority, Schedule);
  BRS_Schedule_P_Insert (Handlers[6].Node, Handlers[6].Priority, Schedule);
  BRS_Schedule_P_Insert (Handlers[7].Node, Handlers[7].Priority, Schedule);

  //Check if we have three of priority 4 tasks.
  assert (BRS_Lists_DL_Count (Schedule->List + 4) == 3);

  int R;

  R = BRS_Semaphore_Take (Handlers[7].Node, Handlers[7].Priority, Semaphore, Schedule);
  assert (R == 1);
  assert (BRS_Lists_DL_Count (Schedule->List + 4) == 3);

  R = BRS_Semaphore_Take (Handlers[1].Node, Handlers[1].Priority, Semaphore, Schedule);
  assert (R == 0);
  assert (BRS_Schedule_P_Current_Count (Semaphore->Schedule) == 1);
  assert (BRS_Lists_DL_Count (Schedule->List + 4) == 2);

  R = BRS_Semaphore_Release (Handlers[1].Node, Handlers[1].Priority, Semaphore, Schedule);
  assert (R == 0);

  R = BRS_Semaphore_Release (Handlers[7].Node, Handlers[7].Priority, Semaphore, Schedule);
  assert (R == 1);
  assert (BRS_Lists_DL_Count (Schedule->List + 4) == 3);



  getchar ();

}
