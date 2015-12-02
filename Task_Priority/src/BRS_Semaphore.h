#ifndef BRS_Semaphore_h
#define BRS_Semaphore_h

#include "BRS_Schedule_P.h"

struct BRS_Semaphore
{
  struct BRS_Schedule_P Schedule [1];
  struct BRS_CDLL_Node * Owner;
};


static inline void BRS_Semaphore_Initialize
(struct BRS_CDLL_Node * List, size_t Count, struct BRS_Semaphore * Semaphore)
{
  BRS_Schedule_P_Initialize (List, Count, Semaphore->Schedule);
  Semaphore->Owner = NULL;
}


static inline int BRS_Semaphore_Take
(struct BRS_CDLL_Node * Item, size_t Priority, struct BRS_Semaphore * Semaphore, struct BRS_Schedule_P * Schedule)
{
  //You should not take the semaphore if you are already the owner.
  assert (Item != Semaphore->Owner);

  if (Semaphore->Owner == NULL)
  {
    Semaphore->Owner = Item;
    return 1;
  }
  else
  {
    BRS_Schedule_P_Transfer (Item, Priority, Schedule, Semaphore->Schedule);
    return 0;
  }
}


static inline void BRS_Semaphore_Release
(struct BRS_Semaphore * Semaphore, struct BRS_Schedule_P * Schedule)
{
  if (BRS_Schedule_P_Empty (Semaphore->Schedule))
  {
    Semaphore->Owner = NULL;
  }
  else
  {
    //Assign the next owner.
    Semaphore->Owner = BRS_Schedule_P_Current (Semaphore->Schedule);
    Semaphore->Owner = Semaphore->Owner->Next;

    //Empty list not allowed.
    assert (Semaphore->Owner != Semaphore->Owner->Next);

    //Move the inheritor to the <Schedule>
    BRS_Schedule_P_Transfer (Semaphore->Owner, Semaphore->Schedule->Priority, Semaphore->Schedule, Schedule);
  }
}



#endif
