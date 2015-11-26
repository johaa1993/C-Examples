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

  //Insert the new node item at the respective priority slot.
  BRS_Lists_DL_Insert_After (Item, &Schedule->List_Array [Priority]);
}







//<Return> the highest priority task handler node.
//Calulates new highest priority if nesseracy.
static inline struct BRS_Lists_DL_Node * BRS_Schedule_PT_Pull (struct BRS_Schedule_PT * Schedule)
{
  //Node can be anything.
  //It's just a location for the memory of interest we want to find.
  struct BRS_Lists_DL_Node * Node;

  //Get the first node by priority tracker.
  Node = Schedule->List_Array [Schedule->Priority].Next;

  //Calulates new highest priority if nesseracy.
  //If node is list then its empty then decrament the priority tracker.
  //NOTE: Calculation is is not O(1).
  //TODO: Find a solution to make it O(1).
  while ((Schedule->Priority > 0) && (Node == &Schedule->List_Array [Schedule->Priority]))
  {
    Schedule->Priority--;
    Node = Schedule->List_Array [Schedule->Priority].Next;
  }

  //Remove the node from the schedule list.
  //If node is list then its empty. This can be checked outside this function.
  //Remove from empty list has no side effect.
  BRS_Lists_DL_Remove (Node);
  return Node;
}


#endif
