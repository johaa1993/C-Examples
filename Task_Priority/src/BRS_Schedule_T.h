#ifndef BRS_Schedule_T_h
#define BRS_Schedule_T_h

#include "BRS_CDLL.h"
#include "BRS_Address.h"


struct BRS_Schedule_T
{
  struct BRS_CDLL_Node List;
};


static inline void BRS_Schedule_T_Insert
(struct BRS_CDLL_Node * Item, size_t Offset, struct BRS_Schedule_T * Schedule)
{
  struct BRS_CDLL_Node * Node;
  struct BRS_CDLL_Node * List;
  size_t Time;
  size_t Compare;

  List = &Schedule->List;
  Node = List->Next;
  //Locate time data relative to item.
  Time = BRS_Type_Offset (size_t, Item, Offset);
  for (;;)
  {
    //Break if list is empty.
    if (Node == List) break;
    //List is not empty extract comparison value.
    Compare = BRS_Type_Offset (size_t, Node, Offset);
    //Node is the location where new item is inserted.
    //If we break node is the position to insert.
    if (Time < Compare) break;
    //Iterate through list.
    Node = Node->Next;
  }
  BRS_CDLL_Insert_After (Item, Node->Prev);
}


static inline struct BRS_CDLL_Node * BRS_Schedule_T_Pull
(size_t Offset, size_t Time, struct BRS_Schedule_T * Schedule)
{
  struct BRS_CDLL_Node * List;
  struct BRS_CDLL_Node * Node;
  size_t Time;

  List = &Schedule->List;
  Node = List->Next;

  //List is empty.
  if (Node == List)
  {
    return List;
  }

  //Extract comparison value.
  Compare = BRS_Type_Offset (size_t, Node, Offset);

  if (Compare < Time)
  {
    BRS_CDLL_Remove (Node);
    return Node;
  }

  //List is not empty.
  return NULL;
}


#endif
