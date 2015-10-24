#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <time.h>
#include "Timer.h"
#include "SIMD.h"

#define Float_Array_4_Format "%f %f %f %f"
typedef float Float_Array_4[4] __attribute__((aligned(32)));
typedef float Float_Array_16[16] __attribute__((aligned(32)));

Float_Array_16 Matrix_1 =
{
  1.0f, 2.0f, 3.0f, 4.0f,
  0.0f, 1.0f, 2.0f, 3.0f,
  2.0f, 0.0f, 0.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
Float_Array_16 Matrix_2 =
{
  1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f
};
Float_Array_16 Matrix_3;

int main (int argc, char** argv)
{

  #ifdef __MMX__
    printf ("__MMX__\n");
  #endif

  #ifdef __SSE2__
    printf ("__SSE2__\n");
  #endif

  #ifdef __SSE3__
    printf ("__SSE3__\n");
  #endif

  SIMD_Matrix_4_Multiplicate (Matrix_1, Matrix_2, Matrix_3);
  printf ("End2:\n");
  //getchar();
  printf (Float_Array_4_Format"\n", Matrix_3[0], Matrix_3[1], Matrix_3[2], Matrix_3[3]);
  printf (Float_Array_4_Format"\n", Matrix_3[4], Matrix_3[5], Matrix_3[6], Matrix_3[7]);
  printf (Float_Array_4_Format"\n", Matrix_3[8], Matrix_3[9], Matrix_3[10], Matrix_3[11]);
  printf (Float_Array_4_Format"\n", Matrix_3[12], Matrix_3[13], Matrix_3[14], Matrix_3[15]);


  getchar();
  return 0;
}
