#ifndef BRS_Schedule_P_h
#define BRS_Schedule_P_h

#include <assert.h>

#include "BRS_CDLL.h"

struct BRS_Schedule_P
{
  struct BRS_CDLL_Node * List;
  size_t Priority;
};



static inline void BRS_Schedule_P_Initialize
(struct BRS_CDLL_Node * List, size_t Count, struct BRS_Schedule_P * Schedule)
{
  BRS_CDLL_Link_Self_Vector (List, Count);
  Schedule->Priority = 0;
  Schedule->List = List;
}



//Insert node <Item> into <Schedule>.
//Updates the highest priority.
static inline void BRS_Schedule_P_Insert
(struct BRS_CDLL_Node * Item, size_t Priority, struct BRS_Schedule_P * Schedule)
{
  assert (Item != Item->Next);
  if (Priority > Schedule->Priority)
  {
    Schedule->Priority = Priority;
  }
  BRS_CDLL_Insert_After (Item, Schedule->List + Priority);
}




static inline struct BRS_CDLL_Node * BRS_Schedule_P_Current
(struct BRS_Schedule_P * Schedule)
{
  return Schedule->List + Schedule->Priority;
}




//Updates the highest priority.
static inline void BRS_Schedule_P_Update
(struct BRS_Schedule_P * Schedule)
{
  struct BRS_CDLL_Node const * Node;
  for (;;)
  {
    Node = BRS_Schedule_P_Current (Schedule);
    Node = Node->Next;
    if ((Node != BRS_Schedule_P_Current (Schedule)) || (Schedule->Priority == 0)) break;
    Schedule->Priority--;
  }
}



//Remove.
//Updates the highest priority.
static inline void BRS_Schedule_P_Remove
(struct BRS_CDLL_Node * Node, struct BRS_Schedule_P * Schedule)
{
  BRS_CDLL_Remove (Node);
  BRS_Schedule_P_Update (Schedule);
}



//Empty 0 or 1
static inline int BRS_Schedule_P_Empty
(struct BRS_Schedule_P * Schedule)
{
  struct BRS_CDLL_Node * List;
  List = BRS_Schedule_P_Current (Schedule);
  if (List == List->Next)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}



static inline struct BRS_CDLL_Node * BRS_Schedule_P_Current_Next_Node
(struct BRS_CDLL_Node * Node, struct BRS_Schedule_P * Schedule)
{
  Node = Node->Next;
  if (Node == BRS_Schedule_P_Current (Schedule))
  {
    Node = Node->Next;
  }
  return Node;
}



//Get the number of element-node in the priority <Priority> list-node.
static inline size_t BRS_Schedule_P_Count
(size_t Priority, struct BRS_Schedule_P * Schedule)
{
  return BRS_CDLL_Count (Schedule->List + Priority);
}


//Get the number of element-node in the highest priority list-node.
static inline size_t BRS_Schedule_P_Current_Count
(struct BRS_Schedule_P * Schedule)
{
  return BRS_Schedule_P_Count (Schedule->Priority, Schedule);
}



//Transfer element-node <Item> with priority <Priority> from schedule <A> to <B>.
static inline void BRS_Schedule_P_Transfer
(struct BRS_CDLL_Node * Item, size_t Priority, struct BRS_Schedule_P * A, struct BRS_Schedule_P * B)
{
  //No empty list allowed.
  assert (Item != Item->Next);
  BRS_Schedule_P_Remove (Item, A);
  BRS_Schedule_P_Insert (Item, Priority, B);
}


//Transfer current highest priority element-node <Item> from schedule <A> to <B>.
static inline void BRS_Schedule_P_Current_Transfer
(struct BRS_Schedule_P * A, struct BRS_Schedule_P * B)
{
  struct BRS_CDLL_Node * Item;
  //Get the current list-node.
  Item = BRS_Schedule_P_Current (A);
  //Goto the the first list-element.
  Item = Item->Next;
  //No empty list allowed.
  assert (Item != Item->Next);
  BRS_Schedule_P_Transfer (Item, A->Priority, A, B);
}



//Change item priority.
static inline void BRS_Schedule_P_Promote
(struct BRS_CDLL_Node * Item, size_t Priority, struct BRS_Schedule_P * Schedule)
{
  //No empty list allowed.
  assert (Item != Item->Next);
  //Transfer Item to it's own schedule but with a other priority.
  BRS_Schedule_P_Transfer (Item, Priority, Schedule, Schedule);
}





#endif
