#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <time.h>
#include <tmmintrin.h>
#include "Timer.h"



void SIMD_Add_Vector_Scalar (int Count, float * A, float B, float * Result)
{
  const __m128 B4 = _mm_set1_ps (B);
  __m128 temp;
  for (int i = 0; i < Count; i += 4)
  {
    temp = _mm_load_ps (A + i);
    temp = _mm_add_ps (temp, B4);
    _mm_store_ps (Result + i, temp);
  }
}

void Conventional_Add_Vector_Scalar (int Count, float * A, float B, float * Result)
{
  for (int i = 0; i < Count; i += 1)
  {
    Result[i] = A[i] + B;
  }
}



#define Vector_Count 10000
#define Test_Count 10000

int main (int argc, char** argv)
{

  float Data[Vector_Count] __attribute__((aligned(16))) = {0.0f};
  float Result[Vector_Count] __attribute__((aligned(16))) = {0.0f};

  double Wall_Time_0;
  double Wall_Time_1;
  double CPU_Time_0;
  double CPU_Time_1;

  Wall_Time_0 = Get_Wall_Time ();
  CPU_Time_0 = Get_CPU_Time ();
  for (int i = 0; i < Test_Count; i++)
  {
    //SIMD_Add_Vector_Scalar (COUNT, Data, 10.0f, Result);
    Conventional_Add_Vector_Scalar (Vector_Count, Data, 10.0f, Result);
  }
  Wall_Time_1 = Get_Wall_Time ();
  CPU_Time_1 = Get_CPU_Time ();

  printf ("Wall-Time %lf\n", (Wall_Time_1 - Wall_Time_0));
  printf ("CPU Time %lf\n", (CPU_Time_1 - CPU_Time_0));

  return 0;
}
