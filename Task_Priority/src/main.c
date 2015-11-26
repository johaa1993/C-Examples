#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <time.h>
#include "BRS_List_CDLL.h"
#include "BRS_Schedule_PT.h"




struct BRS_Task_Handler
{
  int Context_X;
  struct BRS_Lists_DL_Node Node;
  float Context_Y;
  size_t Priority;
  signed volatile long Context_Z;
};

#define BRS_Task_Handler_Iterator_Entry(Iterator) BRS_Lists_DL_Node_Entry (Iterator, struct BRS_Task_Handler, Node)


struct BRS_Schedule_PT    Schedule;
struct BRS_Lists_DL_Node Schedule_Nodes [10];
struct BRS_Task_Handler   Handlers [10];


int main (int argc, char** argv)
{

  BRS_Schedule_PT_Initialize (Schedule_Nodes, 10, &Schedule);

  Handlers[0].Priority = 2;
  Handlers[1].Priority = 4;
  Handlers[2].Priority = 7;
  Handlers[3].Priority = 2;
  Handlers[4].Priority = 0;
  Handlers[5].Priority = 1;
  Handlers[6].Priority = 4;

  BRS_Schedule_PT_Insert (&Handlers[0].Node, Handlers[0].Priority, &Schedule);
  BRS_Schedule_PT_Insert (&Handlers[1].Node, Handlers[1].Priority, &Schedule);
  BRS_Schedule_PT_Insert (&Handlers[2].Node, Handlers[2].Priority, &Schedule);
  BRS_Schedule_PT_Insert (&Handlers[3].Node, Handlers[3].Priority, &Schedule);
  BRS_Schedule_PT_Insert (&Handlers[4].Node, Handlers[4].Priority, &Schedule);
  BRS_Schedule_PT_Insert (&Handlers[5].Node, Handlers[5].Priority, &Schedule);
  BRS_Schedule_PT_Insert (&Handlers[6].Node, Handlers[6].Priority, &Schedule);

  struct BRS_Lists_DL_Node * Node;
  struct BRS_Task_Handler * Handler;

  for (;;)
  {
    Node = BRS_Schedule_PT_Pull (&Schedule);
    if (Node == &Schedule.List_Array [Schedule.Priority]) break;
    Handler = BRS_Task_Handler_Iterator_Entry (Node);
    printf("Priority %i\n", Handler->Priority);
    getchar();
  }


  getchar();
  return 0;
}
