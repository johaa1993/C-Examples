#ifndef Test_BRS_CDLL_h
#define Test_BRS_CDLL_h

#include <unistd.h>
#include <stdio.h>
#include "BRS_CDLL.h"


static void Test_BRS_CDLL_1 ()
{

  struct Model
  {
    long Banana;
    struct BRS_CDLL_Node Node [1];
    int Position;
    char Coal;
  };

  #define Model_Position_Node_Offset sizeof (struct BRS_CDLL_Node)

  struct BRS_CDLL_Node List [1];
  struct Model Object [1000];

  BRS_CDLL_Link_Self (List);

  Object[0].Position = 2;
  Object[1].Position = 1;
  Object[2].Position = 99;
  Object[3].Position = 0;
  Object[4].Position = 3;

  BRS_CDLL_Insert_Sorted (Object[0].Node, Model_Position_Node_Offset, List);
  BRS_CDLL_Insert_Sorted (Object[1].Node, Model_Position_Node_Offset, List);
  BRS_CDLL_Insert_Sorted (Object[2].Node, Model_Position_Node_Offset, List);
  BRS_CDLL_Insert_Sorted (Object[3].Node, Model_Position_Node_Offset, List);
  BRS_CDLL_Insert_Sorted (Object[4].Node, Model_Position_Node_Offset, List);

  struct BRS_CDLL_Node * Iterator;
  Iterator = List->Next;

  for (;;)
  {
    if (Iterator == List) break;
    printf("Position %i\n", BRS_Type_Offset (int, Iterator, Model_Position_Node_Offset));
    Iterator = Iterator->Next;
  }

  getchar ();

}

#endif
