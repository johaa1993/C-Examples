#ifndef Test_BRS_CDLL_h
#define Test_BRS_CDLL_h

#include <unistd.h>
#include <stdio.h>
#include "BRS_CDLL.h"

#define Test_BRS_CDLL_1_Model_Position_Node_Offset sizeof (struct BRS_CDLL_Node)

static int Test_BRS_CDLL_1_Compare (struct BRS_CDLL_Node * Insertion, struct BRS_CDLL_Node * Iterator)
{
  int Insert_Value;
  int Iterator_Value;
  Insert_Value = BRS_Type_Offset (int, Insertion, Test_BRS_CDLL_1_Model_Position_Node_Offset);
  Iterator_Value = BRS_Type_Offset (int, Iterator, Test_BRS_CDLL_1_Model_Position_Node_Offset);
  return Insert_Value < Iterator_Value;
}

static void Test_BRS_CDLL_1 ()
{

  struct Model
  {
    long Itude;
    struct BRS_CDLL_Node Node [1];
    int Position;
    char Coal;
  };



  struct BRS_CDLL_Node List [1];
  struct Model Object [1000];

  BRS_CDLL_Link_Self (List);

  Object[0].Position = 2;
  Object[1].Position = 1;
  Object[2].Position = 99;
  Object[3].Position = 0;
  Object[4].Position = 3;

  BRS_CDLL_Insert_Sorted (Test_BRS_CDLL_1_Compare, Object[0].Node, List);
  BRS_CDLL_Insert_Sorted (Test_BRS_CDLL_1_Compare, Object[1].Node, List);
  BRS_CDLL_Insert_Sorted (Test_BRS_CDLL_1_Compare, Object[2].Node, List);
  BRS_CDLL_Insert_Sorted (Test_BRS_CDLL_1_Compare, Object[3].Node, List);
  BRS_CDLL_Insert_Sorted (Test_BRS_CDLL_1_Compare, Object[4].Node, List);

  struct BRS_CDLL_Node * Iterator;
  Iterator = List->Next;

  for (;;)
  {
    if (Iterator == List) break;
    printf("Position %i\n", BRS_Type_Offset (int, Iterator, Test_BRS_CDLL_1_Model_Position_Node_Offset));
    Iterator = Iterator->Next;
  }

  getchar ();

}






#endif
