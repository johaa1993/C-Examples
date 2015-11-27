#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <time.h>
#include "BRS_Schedule_PT.h"
#include "BRS_Schedule_TT.h"

struct BRS_Task_Handler
{
  int Apple;
  struct BRS_Lists_DL_Node Node_PT;
  float Orange;
  size_t Priority;
  signed volatile long Potato;
  struct BRS_Lists_DL_Node Node_TT;
  size_t Time;
  unsigned volatile long long Banana;
};

#define BRS_Task_Handler_Iterator_Entry_PT(Iterator) BRS_Lists_DL_Node_Entry (Iterator, struct BRS_Task_Handler, Node_PT)
#define BRS_Task_Handler_Iterator_Entry_TT(Iterator) BRS_Lists_DL_Node_Entry (Iterator, struct BRS_Task_Handler, Node_TT)

struct BRS_Task_Handler Handlers [7];

//Setup priority scheduler.
#define Schedule_PT_Lists_Count 10
struct BRS_Schedule_PT Schedule_PT;
struct BRS_Lists_DL_Node Schedule_PT_Lists [Schedule_PT_Lists_Count];

//Setup time scheduler.
struct BRS_Schedule_TT Schedule_TT;
#define BRS_Task_Handler_Time_Offset sizeof (struct BRS_Lists_DL_Node)


int main (int argc, char** argv)
{

  BRS_Schedule_PT_Initialize (Schedule_PT_Lists, Schedule_PT_Lists_Count, &Schedule_PT);
  BRS_Lists_DL_Link_Self (&Schedule_TT.List);

  Handlers[0].Priority = 2;
  Handlers[1].Priority = 4;
  Handlers[2].Priority = 7;
  Handlers[3].Priority = 2;
  Handlers[4].Priority = 0;
  Handlers[5].Priority = 1;
  Handlers[6].Priority = 4;

  Handlers[0].Time = 32;
  Handlers[1].Time = 24;
  Handlers[2].Time = 12;
  Handlers[3].Time = 52;
  Handlers[4].Time = 23;
  Handlers[5].Time = 11;
  Handlers[6].Time = 1;

  BRS_Schedule_PT_Insert (&Handlers[0].Node_PT, Handlers[0].Priority, &Schedule_PT);
  BRS_Schedule_PT_Insert (&Handlers[1].Node_PT, Handlers[1].Priority, &Schedule_PT);
  BRS_Schedule_PT_Insert (&Handlers[2].Node_PT, Handlers[2].Priority, &Schedule_PT);
  BRS_Schedule_PT_Insert (&Handlers[3].Node_PT, Handlers[3].Priority, &Schedule_PT);
  BRS_Schedule_PT_Insert (&Handlers[4].Node_PT, Handlers[4].Priority, &Schedule_PT);
  BRS_Schedule_PT_Insert (&Handlers[5].Node_PT, Handlers[5].Priority, &Schedule_PT);
  BRS_Schedule_PT_Insert (&Handlers[6].Node_PT, Handlers[6].Priority, &Schedule_PT);

  BRS_Schedule_TT_Insert (&Handlers[0].Node_TT, BRS_Task_Handler_Time_Offset, &Schedule_TT);
  BRS_Schedule_TT_Insert (&Handlers[1].Node_TT, BRS_Task_Handler_Time_Offset, &Schedule_TT);
  BRS_Schedule_TT_Insert (&Handlers[2].Node_TT, BRS_Task_Handler_Time_Offset, &Schedule_TT);
  BRS_Schedule_TT_Insert (&Handlers[3].Node_TT, BRS_Task_Handler_Time_Offset, &Schedule_TT);
  BRS_Schedule_TT_Insert (&Handlers[4].Node_TT, BRS_Task_Handler_Time_Offset, &Schedule_TT);
  BRS_Schedule_TT_Insert (&Handlers[6].Node_TT, BRS_Task_Handler_Time_Offset, &Schedule_TT);


  struct BRS_Lists_DL_Node * Node;
  struct BRS_Task_Handler * Handler;

  for (;;)
  {
    Node = BRS_Schedule_PT_Pull (&Schedule_PT);
    if (Node == Schedule_PT.List_Array) break;
    Handler = BRS_Task_Handler_Iterator_Entry_PT (Node);
    printf ("Priority %i\n", Handler->Priority);
    getchar ();
  }

  getchar();

  printf ("Time\n");

  for (;;)
  {
    Node = BRS_Schedule_TT_Pull (BRS_Task_Handler_Time_Offset, 40, &Schedule_TT);
    if (Node == NULL)
    {
      printf ("No tasks is ready");
      break;
    }

    if (Node == &Schedule_TT.List)
    {
      printf ("No task is Timeed");
      break;
    }

    Handler = BRS_Task_Handler_Iterator_Entry_TT (Node);
    printf ("Task entry time %i\n", Handler->Time);
    getchar ();
  }


  return 0;
}
