#ifndef BRS_Schedule_T_h
#define BRS_Schedule_T_h

#include "BRS_CDLL.h"
#include "BRS_Address.h"


struct BRS_Schedule_T
{
  struct BRS_CDLL_Node List [1];
};

struct BRS_Schedule_T_Node
{
  struct BRS_CDLL_Node Node [1];
  size_t Time;
};

int BRS_Schedule_T_Compare (struct BRS_CDLL_Node * Insertion, struct BRS_CDLL_Node * Iterator)
{
  int Value_Insertion;
  int Value_Iterator;
  Value_Insertion = BRS_Type_Offset (size_t, Insertion, sizeof (struct BRS_CDLL_Node));
  Value_Iterator = BRS_Type_Offset (size_t, Iterator, sizeof (struct BRS_CDLL_Node));
  return Value_Insertion < Value_Iterator;
}


static inline void BRS_Schedule_T_Insert
(struct BRS_Schedule_T_Node * Item, struct BRS_Schedule_T * Schedule)
{
  BRS_CDLL_Insert_Sorted (BRS_Schedule_T_Compare, Item->Node, Schedule->List);
}


static inline size_t BRS_Schedule_T_First
(struct BRS_Schedule_T * Schedule)
{
  if (Schedule->List == Schedule->List->Next)
  {
    return 0;
  }
  else
  {
    return BRS_Type_Offset (size_t, Schedule->List->Next, sizeof (struct BRS_CDLL_Node));
  }
}


#endif
