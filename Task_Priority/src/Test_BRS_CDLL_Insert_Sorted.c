#include <stdio.h>

#include "BRS_Address.h"
#include "BRS_CDLL.h"


struct Test_Model
{
  long Itude;
  struct BRS_CDLL_Node Node [1];
  int Position;
  char Coal;
};

#define Position_Offset sizeof (struct BRS_CDLL_Node)

int Compare (struct BRS_CDLL_Node * Insertion, struct BRS_CDLL_Node * Iterator)
{
  int Position_Insertion;
  int Position_Iterator;
  Position_Insertion = BRS_Type_Offset (int, Insertion, Position_Offset);
  Position_Iterator = BRS_Type_Offset (int, Iterator, Position_Offset);
  return Position_Insertion < Position_Iterator;
}


int main (int argc, char** argv)
{

  struct BRS_CDLL_Node List [1];
  struct Test_Model Model [100];
  struct BRS_CDLL_Node * Iterator;

  BRS_CDLL_Link_Self (List);

  Model[0].Position = 2;
  Model[1].Position = 1;
  Model[2].Position = 99;
  Model[3].Position = 0;
  Model[4].Position = 3;

  for (size_t I = 0; I < 5; I = I + 1)
  {
    printf ("Insert ");
    printf ("I %04i ", I);
    printf ("P %04i ", Model[I].Position);
    printf ("\n");
    BRS_CDLL_Insert_Sorted (Compare, Model[I].Node, List);
  }

  Iterator = List->Next;

  for (;;)
  {
    if (Iterator == List) break;
    printf ("P %04i\n", BRS_Type_Offset (int, Iterator, Position_Offset));
    Iterator = Iterator->Next;
  }

  getchar ();

  return 0;
}
