#ifndef BRS_Semaphore_h
#define BRS_Semaphore_h

#include "BRS_Schedule_P.h"

struct BRS_Semaphore
{
  struct BRS_Schedule_P Schedule [1];
  struct BRS_Lists_DL_Node * Owner;
};


static inline void BRS_Semaphore_Initialize
(struct BRS_Lists_DL_Node * List, size_t Count, struct BRS_Semaphore * Semaphore)
{
  BRS_Schedule_P_Initialize (List, Count, Semaphore->Schedule);
  Semaphore->Owner = NULL;
}



static inline int BRS_Semaphore_Take
(struct BRS_Lists_DL_Node * Item, size_t Priority, struct BRS_Semaphore * Semaphore, struct BRS_Schedule_P * Schedule)
{
  //If semaphore is free.
  if (Semaphore->Owner == NULL)
  {
    Semaphore->Owner = Item;
    return 1;
  }
  //If semaphore is taken.
  else
  {
    BRS_Schedule_P_Remove (Item, Schedule);
    BRS_Schedule_P_Insert (Item, Priority, Semaphore->Schedule);
    return 0;
  }
}


static inline int BRS_Semaphore_Release
(struct BRS_Lists_DL_Node * Item, size_t Priority, struct BRS_Semaphore * Semaphore, struct BRS_Schedule_P * Schedule)
{
  if (Item == Semaphore->Owner)
  {
    if (BRS_Schedule_P_Empty (Semaphore->Schedule))
    {
      Semaphore->Owner = NULL;
    }
    //Assign the next owner.
    else
    {
      Semaphore->Owner = BRS_Schedule_P_Current (Semaphore->Schedule);
      BRS_Schedule_P_Remove (Semaphore->Owner, Semaphore->Schedule);
      BRS_Schedule_P_Insert (Semaphore->Owner, Priority, Schedule);
    }
    return 1;
  }
  else
  {
    return 0;
  }
}



#endif
