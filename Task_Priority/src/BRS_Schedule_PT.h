#ifndef BRS_Schedule_PT_h
#define BRS_Schedule_PT_h

#include "BRS_Lists_DL.h"


//Priority Sorted Array of Priority Grouped Circular Doubly Linked List,
//Highest Priority Tracked.
struct BRS_Schedule_PT
{
  struct BRS_Lists_DL_Node * List_Array;
  size_t Priority;
};

static inline void BRS_Schedule_PT_Initialize (struct BRS_Lists_DL_Node * List_Array, size_t Count, struct BRS_Schedule_PT * Schedule)
{
  Schedule->List_Array = List_Array;
  BRS_Lists_DL_Link_Self_Vector (Schedule->List_Array, Count);
}




//Insert task handler <Item> into <Schedule>.
//Calulates new highest priority if nesseracy.
static inline void BRS_Schedule_PT_Insert
(
  struct BRS_Lists_DL_Node * Item,
  size_t Priority,
  struct BRS_Schedule_PT * Schedule
)
{
  //Calulates new highest priority if nesseracy.
  if (Priority > Schedule->Priority)
  {
    Schedule->Priority = Priority;
  }

  BRS_Lists_DL_Insert_After (Item, Schedule->List_Array + Priority);
}







//<Return> the highest priority task handler node.
//Calulates new highest priority if nesseracy.
static inline struct BRS_Lists_DL_Node * BRS_Schedule_PT_Pull (struct BRS_Schedule_PT * Schedule)
{
  struct BRS_Lists_DL_Node * Node;

  //Find the highest priority non empty list.
  for (;;)
  {
    Node = Schedule->List_Array + Schedule->Priority;
    if ((Node != Node->Next) || (Schedule->Priority == 0)) break;
    Schedule->Priority--;
  }

  //Get the first node of the list.
  //The list may be empty here.
  Node = Node->Next;

  //Remove the node from the schedule list.
  BRS_Lists_DL_Remove (Node);
  return Node;
}


#endif
