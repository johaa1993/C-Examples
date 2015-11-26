#ifndef BRS_List_CDLL_h
#define BRS_List_CDLL_h

/*
#define list_entry(ptr, type, member) \
((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))
*/

#define BRS_List_CDLL_Node_Entry(ptr, type, member) ((type *)((char *)(ptr) - __builtin_offsetof(type, member)))


struct BRS_List_CDLL_Node
{
  struct BRS_List_CDLL_Node * Next;
  struct BRS_List_CDLL_Node * Prev;
};



/*
Link the node <Item> to it self.
*/
static inline void BRS_List_CDLL_Initialize (struct BRS_List_CDLL_Node * Item)
{
  Item->Next = Item;
  Item->Prev = Item;
}

/*
Insert the node <Item> between nodes <Prev> and <Next>
*/
static inline void BRS_List_CDLL_Insert (struct BRS_List_CDLL_Node * Item, struct BRS_List_CDLL_Node * Prev, struct BRS_List_CDLL_Node * Next)
{
  Item->Next = Next;
  Item->Prev = Prev;
  Next->Prev = Item;
  Prev->Next = Item;
}


/*
Insert the node <Item> after the node <Item_0>
*/
static inline void BRS_List_CDLL_Insert_After (struct BRS_List_CDLL_Node * Item, struct BRS_List_CDLL_Node * Item_0)
{
  BRS_List_CDLL_Insert (Item, Item_0, Item_0->Next);
}


/*
Insert the node <Item> before the node <Item_0>
*/
static inline void BRS_List_CDLL_Insert_Before (struct BRS_List_CDLL_Node * Item, struct BRS_List_CDLL_Node * Item_0)
{
  BRS_List_CDLL_Insert (Item, Item_0->Prev, Item_0);
}


/*
Remove the node between <Prev> and <Next>.
*/
static inline void BRS_List_CDLL_Remove_Between (struct BRS_List_CDLL_Node * Prev, struct BRS_List_CDLL_Node * Next)
{
  Next->Prev = Prev;
  Prev->Next = Next;
}


/*
Remove the node <Item>.
*/
static inline void BRS_List_CDLL_Remove (struct BRS_List_CDLL_Node * Item)
{
  BRS_List_CDLL_Remove_Between (Item->Prev, Item->Next);
}




#endif
