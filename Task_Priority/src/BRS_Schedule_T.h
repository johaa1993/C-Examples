#ifndef BRS_Schedule_T_h
#define BRS_Schedule_T_h

#include "BRS_CDLL.h"
#include "BRS_Address.h"


struct BRS_Schedule_T
{
  struct BRS_CDLL_Node List [1];
};

//Internal function.
int BRS_Schedule_T_Compare (struct BRS_CDLL_Node * Insertion, struct BRS_CDLL_Node * Iterator)
{
  int Value_Insertion;
  int Value_Iterator;
  Value_Insertion = BRS_Type_Offset (size_t, Insertion, sizeof (struct BRS_CDLL_Node));
  Value_Iterator = BRS_Type_Offset (size_t, Iterator, sizeof (struct BRS_CDLL_Node));
  return Value_Insertion < Value_Iterator;
}


static inline void BRS_Schedule_T_Insert
(struct BRS_CDLL_Node * Item, struct BRS_Schedule_T * Schedule)
{
  BRS_CDLL_Insert_Sorted (BRS_Schedule_T_Compare, Item, Schedule->List);
}




static inline struct BRS_CDLL_Node * BRS_Schedule_T_Pull_Temporal
(struct BRS_Schedule_T * Schedule, size_t Time)
{
  struct BRS_CDLL_Node * Node;
  size_t Wake;

  //Get first element-node.
  Node = Schedule->List;
  Node = Node->Next;

  //Check if list is empty.
  if (Node == Node->Next)
  {
    return NULL;
  }

  //Get passed wake time node
  Wake = BRS_Type_Offset (size_t, Node, sizeof (struct BRS_CDLL_Node));

  if (Time > Wake)
  {
    BRS_CDLL_Remove (Node);
    return Node;
  }

  //No node has passed wake time.
  return NULL;
}



#endif
