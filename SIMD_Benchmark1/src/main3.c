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

void Float_Array_Set (int Count, float X, float * R)
{
  for (int I = 0; I < Count; I = I + 1)
  {
    R[I] = X;
  }
}



void SIMD_Float_4x32_Dot (float * A, float * B, float * R)
{
  __m128 B1 = _mm_set1_ps (B[0]);
  __m128 B2 = _mm_set1_ps (B[1]);
  __m128 B3 = _mm_set1_ps (B[2]);
  __m128 B4 = _mm_set1_ps (B[3]);
  __m128 A0 = _mm_load_ps (A + 0);
  __m128 A4 = _mm_load_ps (A + 4);
  __m128 A8 = _mm_load_ps (A + 8);
  __m128 A12 = _mm_load_ps (A + 12);
  __m128 Result = _mm_add_ps( _mm_add_ps(_mm_mul_ps(A0, B1),_mm_mul_ps(A4, B2)),_mm_add_ps(_mm_mul_ps(A8, B3),_mm_mul_ps(A12, B4)));
  _mm_store_ps (R, Result);
}








/*
void SIMD_Float_4x32_Dot (float * A, float * B, float * R)
{
  __m128 M[4];
  __m128 X;
  M[0] = _mm_load_ps (A + 0);
  M[1] = _mm_load_ps (A + 4);
  M[2] = _mm_load_ps (A + 8);
  M[3] = _mm_load_ps (A + 12);
  X = _mm_load_ps (B);
  M[0] = _mm_dp_ps (M[0], X, 0b11110001);
  M[1] = _mm_dp_ps (M[1], X, 0b11110001);
  M[2] = _mm_dp_ps (M[2], X, 0b11110001);
  M[3] = _mm_dp_ps (M[3], X, 0b11110001);
  R[0] = _mm_cvtss_f32 (M[0]);
  R[1] = _mm_cvtss_f32 (M[1]);
  R[2] = _mm_cvtss_f32 (M[2]);
  R[3] = _mm_cvtss_f32 (M[3]);
}
*/
void Conv_Float_4x32_Dot (float * A, float * B, float * R)
{
  R[0] = (A[0]*B[0]) + (A[1]*B[1]) + (A[2]*B[2]) + (A[3]*B[3]);
  R[1] = (A[4]*B[0]) + (A[5]*B[1]) + (A[6]*B[2]) + (A[7]*B[3]);
  R[2] = (A[8]*B[0]) + (A[9]*B[1]) + (A[10]*B[2]) + (A[11]*B[3]);
  R[3] = (A[12]*B[0]) + (A[13]*B[1]) + (A[14]*B[2]) + (A[15]*B[3]);
}

Float_Array_4 Vector_1 = {1.0f, 2.0f, 3.0f, 4.0f};
Float_Array_16 Matrix_1 =
{
  1.0f, 2.0f, 3.0f, 4.0f,
  0.0f, 1.0f, 2.0f, 3.0f,
  2.0f, 0.0f, 0.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
Float_Array_4 Vector_2;
double Wall_Time_0;
double Wall_Time_1;
double CPU_Time_0;
double CPU_Time_1;
#define Test_Count 100000000

void Test_SIMD ()
{
  Wall_Time_0 = Get_Wall_Time ();
  CPU_Time_0 = Get_CPU_Time ();
  float R;
  for (long long i = 0; i < Test_Count; i++)
  {
    SIMD_Float_4x32_Dot (Matrix_1, Vector_1, Vector_2);
  }
  Wall_Time_1 = Get_Wall_Time ();
  CPU_Time_1 = Get_CPU_Time ();
  printf ("SIMD Wall-Time %lf\n", (Wall_Time_1 - Wall_Time_0));
  printf ("SIMD CPU Time %lf\n", (CPU_Time_1 - CPU_Time_0));
  printf ("R1 %f\n", Vector_2[0]);
  printf ("R2 %f\n", Vector_2[1]);
  printf ("R3 %f\n", Vector_2[2]);
  printf ("R4 %f\n", Vector_2[3]);
}

void Test_Conv ()
{
  Wall_Time_0 = Get_Wall_Time ();
  CPU_Time_0 = Get_CPU_Time ();
  float R;
  for (long long i = 0; i < Test_Count; i++)
  {
    Conv_Float_4x32_Dot (Matrix_1, Vector_1, Vector_2);
  }
  Wall_Time_1 = Get_Wall_Time ();
  CPU_Time_1 = Get_CPU_Time ();
  printf ("Conv Wall-Time %lf\n", (Wall_Time_1 - Wall_Time_0));
  printf ("Conv CPU Time %lf\n", (CPU_Time_1 - CPU_Time_0));
  printf ("R1 %f\n", Vector_2[0]);
  printf ("R2 %f\n", Vector_2[1]);
  printf ("R3 %f\n", Vector_2[2]);
  printf ("R4 %f\n", Vector_2[3]);
}

int main (int argc, char** argv)
{

  #ifdef __SSE2__
    printf ("__SSE2__\n");
  #endif

  #ifdef __SSE3__
    printf ("__SSE3__\n");
  #endif

/*
  {
    __m128 brod1 = _mm_set_ps (1.0f, 2.0f, 3.0f, 4.0f);
    __m128 brod2 = _mm_set_ps (1.0f, 2.0f, 3.0f, 4.0f);
    __m128 brod3 = _mm_set_ps (1.0f, 2.0f, 3.0f, 4.0f);
    __m128 brod4 = _mm_set_ps (1.0f, 2.0f, 3.0f, 4.0f);
    __m128 r = _mm_add_ps(_mm_add_ps(a, b), _mm_add_ps(c, d));

    _mm_add_ps( _mm_add_ps(_mm_mul_ps(brod1, row1),_mm_mul_ps(brod2, row2)),_mm_add_ps(_mm_mul_ps(brod3, row3),_mm_mul_ps(brod4, row4)));

    Float_Array_4 R;
    _mm_store_ps (R, r);
    printf (Float_Array_4_Format"\n", R[0], R[1], R[2], R[3]);
  }

*/
/*
{
  __m128 brod1 = _mm_set1_ps (0.0f);
  __m128 brod2 = _mm_set1_ps (1.0f);
  __m128 brod3 = _mm_set1_ps (1.0f);
  __m128 brod4 = _mm_set1_ps (2.0f);
  __m128 row1 = _mm_set_ps (1.0f, 2.0f, 3.0f, 4.0f);
  __m128 row2 = _mm_set_ps (5.0f, 6.0f, 7.0f, 8.0f);
  __m128 row3 = _mm_set_ps (1.0f, 2.0f, 3.0f, 4.0f);
  __m128 row4 = _mm_set_ps (1.0f, 2.0f, 3.0f, 4.0f);

  __m128 r = _mm_add_ps( _mm_add_ps(_mm_mul_ps(brod1, row1),_mm_mul_ps(brod2, row2)),_mm_add_ps(_mm_mul_ps(brod3, row3),_mm_mul_ps(brod4, row4)));

  Float_Array_4 R;
  _mm_store_ps (R, r);
  printf (Float_Array_4_Format"\n", R[0], R[1], R[2], R[3]);
}
/*

  //Test_Conv ();
  //Test_SIMD ();

  getchar();
  return 0;
}

/*


*/
