#include <stdio.h>
#include <assert.h>
#include "BRS_CDLL.h"

struct Test_Model
{
  long Itude;
  struct BRS_CDLL_Node Node [1];
  int Position;
  char Coal;
};

int main (int argc, char** argv)
{

  struct BRS_CDLL_Node List [1];
  struct Test_Model Model [100];
  struct BRS_CDLL_Node * Iterator;

  BRS_CDLL_Link_Self (List);

  BRS_CDLL_Insert_After (Model[0].Node, List);
  BRS_CDLL_Insert_After (Model[1].Node, List);
  BRS_CDLL_Insert_After (Model[2].Node, List);
  BRS_CDLL_Insert_After (Model[3].Node, List);
  BRS_CDLL_Insert_After (Model[4].Node, List);

  printf("Count %i\n", BRS_CDLL_Count (List));
  assert (BRS_CDLL_Count (List) == 5);

  getchar ();

  return 0;
}
