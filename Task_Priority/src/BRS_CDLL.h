#ifndef BRS_CDLL_h
#define BRS_CDLL_h

#include <assert.h>

#include "BRS_Address.h"


#define BRS_CDLL_Node_Entry(Pointer, Type, Member) \
((Type *)((char *)(Pointer) - __builtin_offsetof(Type, Member)))

struct BRS_CDLL_Node
{
  struct BRS_CDLL_Node * Next;
  struct BRS_CDLL_Node * Prev;
};



//Link the node <Item> to it self.
static inline void BRS_CDLL_Link_Self
(struct BRS_CDLL_Node * Item)
{
  Item->Next = Item;
  Item->Prev = Item;
}


//Link the every node item in <Items> to it self.
static inline void BRS_CDLL_Link_Self_Vector
(struct BRS_CDLL_Node * Items, size_t Count)
{
  for (size_t I = 0; I < Count; I = I + 1)
  {
    BRS_CDLL_Link_Self (Items + I);
  }
}


//Insert the node <Item> between nodes <Prev> and <Next>.
static inline void BRS_CDLL_Insert
(struct BRS_CDLL_Node * Item, struct BRS_CDLL_Node * Prev, struct BRS_CDLL_Node * Next)
{
  Item->Next = Next;
  Item->Prev = Prev;
  Next->Prev = Item;
  Prev->Next = Item;
}


//Insert the node <Item> after the node <Item_0>.
static inline void BRS_CDLL_Insert_After
(struct BRS_CDLL_Node * Item, struct BRS_CDLL_Node * Item_0)
{
  BRS_CDLL_Insert (Item, Item_0, Item_0->Next);
}


//Insert the node <Item> before the node <Item_0>.
static inline void BRS_CDLL_Insert_Before
(struct BRS_CDLL_Node * Item, struct BRS_CDLL_Node * Item_0)
{
  BRS_CDLL_Insert (Item, Item_0->Prev, Item_0);
}


//Remove the node between <Prev> and <Next>.
//Link <Prev> and <Next> to each other.
static inline void BRS_CDLL_Remove_Between
(struct BRS_CDLL_Node * Prev, struct BRS_CDLL_Node * Next)
{
  Next->Prev = Prev;
  Prev->Next = Next;
}


//Remove the node <Item>.
//Link nodes beside node <Item> to each other.
static inline void BRS_CDLL_Remove
(struct BRS_CDLL_Node * Item)
{
  BRS_CDLL_Remove_Between (Item->Prev, Item->Next);
}



static inline size_t BRS_CDLL_Count
(struct BRS_CDLL_Node * Item)
{
  struct BRS_CDLL_Node * Node;
  size_t Count;

  Count = 0;
  Node = Item;

  for (;;)
  {
    Node = Node->Next;
    if (Node == Item) break;
    Count = Count + 1;
  }

  return Count;
}




static inline size_t BRS_CDLL_Insert_Sorted
(struct BRS_CDLL_Node * Item, size_t Offset, struct BRS_CDLL_Node * List)
{
  size_t Compare;
  size_t Time;
  struct BRS_CDLL_Node * Iterator;
  assert (Item != Item->Next);
  Time = BRS_Type_Offset (size_t, Item, Offset);
  Iterator = List->Next;
  for (;;)
  {
    if (Iterator == List) break;
    Compare = BRS_Type_Offset (size_t, Iterator, Offset);
    if (Time < Compare) break;
    Iterator = Iterator->Next;
  }
  BRS_CDLL_Insert_After (Item, Iterator->Prev);
}








/*
static inline void BRS_CDLL_Looped_Right
(struct BRS_CDLL_Node * Item)
{
  struct BRS_CDLL_Node * Iterator;
  Iterator = Item;
  for (;;)
  {
    Iterator = Iterator->Next;
    if (Iterator == Item) break;
  }
}
*/

#endif
