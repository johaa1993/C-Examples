#include "SIMD.h"
#include <pmmintrin.h>
#include <smmintrin.h>
#include <stdio.h>


__m128 SIMD_128_Sum_4 (__m128 X[4])
{
  return _mm_add_ps (_mm_add_ps(X[0], X[1]), _mm_add_ps(X[2], X[3]));
}

void SIMD_128_Array_Multiplcate (int Count, __m128 * A, __m128 * B, __m128 * R)
{
  for (int I = 0; I < Count; I = I + 1)
  {
    R[I] = _mm_mul_ps (A[I], B[I]);
  }
}

void SIMD_128_Array_Load (int Count, float * A, __m128 * R)
{
  for (int I = 0; I < Count; I = I + 1)
  {
    R[I] = _mm_load_ps (A + (I * 4));
  }
}

void SIMD_128_Array_Set_All (int Count, float * A, __m128 * R)
{
  for (int I = 0; I < Count; I = I + 1)
  {
    R[I] = _mm_set1_ps (A[I]);
  }
}

void SIMD_Matrix_4_Multiplicate (float * A, float * B, float * R)
{
  __m128 AA[4];
  __m128 BB[4];
  #define Result BB[0]
  SIMD_128_Array_Load (4, B, BB);
  for (int I = 0; I < 16; I = I + 4)
  {
    SIMD_128_Array_Set_All (4, A + I, AA);
    SIMD_128_Array_Multiplcate (4, AA, BB, AA);
    Result = SIMD_128_Sum_4 (AA);
    _mm_store_ps (&R[I], Result);
  }
  printf ("End:\n");
  getchar();
}

/*
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
*/
