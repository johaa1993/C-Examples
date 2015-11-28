#ifndef BRS_Schedule_TT_h
#define BRS_Schedule_TT_h

#include "BRS_Lists_DL.h"
#include "BRS_Address.h"


struct BRS_Schedule_TT
{
  struct BRS_Lists_DL_Node List;
};


static inline void
BRS_Schedule_TT_Insert (struct BRS_Lists_DL_Node * Item, size_t Offset, struct BRS_Schedule_TT * Schedule)
{
  struct BRS_Lists_DL_Node * Node;
  struct BRS_Lists_DL_Node * List;
  size_t Time;
  size_t Compare;

  List = &Schedule->List;
  Node = List->Next;

  //Locate time data relative to item.
  Time = BRS_Type_Offset (size_t, Item, Offset);

  for (;;)
  {
    if (Node == List) break;
    Compare = BRS_Type_Offset (size_t, Node, Offset);
    if (Time < Compare) break;
    Node = Node->Next;
  }

  BRS_Lists_DL_Insert_After (Item, Node->Prev);
}


static inline struct BRS_Lists_DL_Node *
BRS_Schedule_TT_Pull (size_t Offset, size_t Current_Time, struct BRS_Schedule_TT * Schedule)
{
  struct BRS_Lists_DL_Node * List;
  struct BRS_Lists_DL_Node * Node;
  size_t Time;

  List = &Schedule->List;
  Node = List->Next;

  //List is empty.
  if (Node == List)
  {
    return List;
  }
  
  Time = BRS_Type_Offset (size_t, Node, Offset);
  if (Time < Current_Time)
  {
    BRS_Lists_DL_Remove (Node);
    return Node;
  }

  //List is not empty.
  return NULL;
}


#endif
