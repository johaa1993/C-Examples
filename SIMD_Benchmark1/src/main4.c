#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <time.h>
#include <pmmintrin.h>
#include <smmintrin.h>
#include "Timer.h"

#define Float_Array_4_Format "%f %f %f %f"
typedef float Float_Array_4[4] __attribute__((aligned(32)));
typedef float Float_Array_16[16] __attribute__((aligned(32)));

void SIMD_Matrix_4_Mul (float * A, float * B, float * R)
{
  __m128 B0 = _mm_load_ps (&B[0]);
  __m128 B4 = _mm_load_ps (&B[4]);
  __m128 B8 = _mm_load_ps (&B[8]);
  __m128 B12 = _mm_load_ps (&B[12]);
  for (int I = 0; I < 16; I = I + 4)
  {
    __m128 A0 = _mm_set1_ps(A[I + 0]);
    __m128 A1 = _mm_set1_ps(A[I + 1]);
    __m128 A2 = _mm_set1_ps(A[I + 2]);
    __m128 A3 = _mm_set1_ps(A[I + 3]);
    __m128 Result = _mm_add_ps
      (
        _mm_add_ps(_mm_mul_ps(A0, B0),_mm_mul_ps(A1, B4)),
        _mm_add_ps (_mm_mul_ps(A2, B8),_mm_mul_ps(A3, B12))
      );
    _mm_store_ps (&R[I], Result);
  }
}

void Conv_Matrix_4_Mul (float * A, float * B, float * R)
{
  for (int I = 0; I < 4; I = I + 1)
  {
    for (int J = 0; J < 4; J = J + 1)
    {
      R[I + 4 * J] = 0;
      for (int K = 0; K < 4; K = K + 1)
      {
        R[I + 4 * J] = R[I + 4 * J] + A[I + 4 * K] * B[K + 4 * J];
      }
    }
  }
}


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

double Wall_Time_0;
double Wall_Time_1;
double CPU_Time_0;
double CPU_Time_1;
#define Test_Count 1000000000

void Test_SIMD ()
{
  Wall_Time_0 = Get_Wall_Time ();
  CPU_Time_0 = Get_CPU_Time ();
  float R;
  for (long long i = 0; i < Test_Count; i++)
  {
    SIMD_Matrix_4_Mul (Matrix_1, Matrix_2, Matrix_3);
  }
  Wall_Time_1 = Get_Wall_Time ();
  CPU_Time_1 = Get_CPU_Time ();
  printf ("SIMD Wall-Time %lf\n", (Wall_Time_1 - Wall_Time_0));
  printf ("SIMD CPU Time %lf\n", (CPU_Time_1 - CPU_Time_0));
  printf (Float_Array_4_Format"\n", Matrix_3[0], Matrix_3[1], Matrix_3[2], Matrix_3[3]);
  printf (Float_Array_4_Format"\n", Matrix_3[4], Matrix_3[5], Matrix_3[6], Matrix_3[7]);
  printf (Float_Array_4_Format"\n", Matrix_3[8], Matrix_3[9], Matrix_3[10], Matrix_3[11]);
  printf (Float_Array_4_Format"\n", Matrix_3[12], Matrix_3[13], Matrix_3[14], Matrix_3[15]);
}


void Test_Conv ()
{
  Wall_Time_0 = Get_Wall_Time ();
  CPU_Time_0 = Get_CPU_Time ();
  float R;
  for (long long i = 0; i < Test_Count; i++)
  {
    Conv_Matrix_4_Mul (Matrix_2, Matrix_1, Matrix_3);
  }
  Wall_Time_1 = Get_Wall_Time ();
  CPU_Time_1 = Get_CPU_Time ();
  printf ("Conv Wall-Time %lf\n", (Wall_Time_1 - Wall_Time_0));
  printf ("Conv CPU Time %lf\n", (CPU_Time_1 - CPU_Time_0));
  printf (Float_Array_4_Format"\n", Matrix_3[0], Matrix_3[1], Matrix_3[2], Matrix_3[3]);
  printf (Float_Array_4_Format"\n", Matrix_3[4], Matrix_3[5], Matrix_3[6], Matrix_3[7]);
  printf (Float_Array_4_Format"\n", Matrix_3[8], Matrix_3[9], Matrix_3[10], Matrix_3[11]);
  printf (Float_Array_4_Format"\n", Matrix_3[12], Matrix_3[13], Matrix_3[14], Matrix_3[15]);
}

int main (int argc, char** argv)
{

  //Test_Conv ();
  Test_SIMD ();

  getchar();
  return 0;
}

/*
#define Test_Count 100000000
SIMD Wall-Time 5.842827
SIMD CPU Time 5.828125
1.000000 3.000000 3.000000 4.000000
0.000000 1.000000 2.000000 3.000000
2.000000 2.000000 0.000000 1.000000
1.000000 2.000000 1.000000 1.000000


Conv Wall-Time 26.537392
Conv CPU Time 26.500000
1.000000 3.000000 3.000000 4.000000
0.000000 1.000000 2.000000 3.000000
2.000000 2.000000 0.000000 1.000000
1.000000 2.000000 1.000000 1.000000


#define Test_Count 1000000000

Conv Wall-Time 268.542875
Conv CPU Time 266.687500
1.000000 3.000000 3.000000 4.000000
0.000000 1.000000 2.000000 3.000000
2.000000 2.000000 0.000000 1.000000
1.000000 2.000000 1.000000 1.000000


SIMD Wall-Time 57.865591
SIMD CPU Time 57.687500
1.000000 3.000000 3.000000 4.000000
0.000000 1.000000 2.000000 3.000000
2.000000 2.000000 0.000000 1.000000
1.000000 2.000000 1.000000 1.000000

*/
