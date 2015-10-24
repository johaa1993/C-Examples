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
  for (int I = 0; I < Count; I = I + 4)
  {
    temp = _mm_load_ps (A + I);
    temp = _mm_add_ps (temp, B4);
    _mm_store_ps (Result + I, temp);
  }
}

void Conventional_Add_Vector_Scalar (int Count, float * A, float B, float * Result)
{
  for (int I = 0; I < Count; I = I + 1)
  {
    Result[I] = A[I] + B;
  }
}



float Conventional_Sum_Vector (int Count, float * X)
{
  float Sum = 0;
  for (int I = 0; I < Count; I = I + 1)
  {
    Sum = Sum + X[I];
  }
  return Sum;
}


void Float_Array_Set (int Count, float X, float * R)
{
  for (int I = 0; I < Count; I = I + 1)
  {
    R[I] = X;
  }
}









#define Array_Count 1000
#define Test_Count 10000000


float Data[Array_Count] __attribute__((aligned(16)));
float Result[Array_Count] __attribute__((aligned(16)));
double Wall_Time_0;
double Wall_Time_1;
double CPU_Time_0;
double CPU_Time_1;

void Test_SIMD ()
{
  Wall_Time_0 = Get_Wall_Time ();
  CPU_Time_0 = Get_CPU_Time ();
  for (int i = 0; i < Test_Count; i++)
  {
    SIMD_Add_Vector_Scalar (Array_Count, Data, 5.0f, Result);
  }
  Wall_Time_1 = Get_Wall_Time ();
  CPU_Time_1 = Get_CPU_Time ();
  printf ("SIMD Wall-Time %lf\n", (Wall_Time_1 - Wall_Time_0));
  printf ("SIMD CPU Time %lf\n", (CPU_Time_1 - CPU_Time_0));



}

void Test_Conv ()
{
  Wall_Time_0 = Get_Wall_Time ();
  CPU_Time_0 = Get_CPU_Time ();
  for (int i = 0; i < Test_Count; i++)
  {
    Conventional_Add_Vector_Scalar (Array_Count, Data, 5.0f, Result);
  }
  Wall_Time_1 = Get_Wall_Time ();
  CPU_Time_1 = Get_CPU_Time ();
  printf ("Conv Wall-Time %lf\n", (Wall_Time_1 - Wall_Time_0));
  printf ("Conv CPU Time %lf\n", (CPU_Time_1 - CPU_Time_0));
}




int main (int argc, char** argv)
{
  Float_Array_Set (Array_Count, 1.0f, Data);

  //Test_Conv ();
  Test_SIMD ();

  getchar();
  return 0;
}
