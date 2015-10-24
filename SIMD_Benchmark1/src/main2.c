#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <time.h>
#include <pmmintrin.h>
#include <smmintrin.h>
#include "Timer.h"


typedef float Float_Array_4[4] __attribute__((aligned(32)));

void Float_Array_Set (int Count, float X, float * R)
{
  for (int I = 0; I < Count; I = I + 1)
  {
    R[I] = X;
  }
}


__m128 SIMD_m128_Dot (const __m128 A, const __m128 B)
{
  __m128 R;
  R = _mm_mul_ps (A, B);
  R = _mm_hadd_ps (R, R);
  R = _mm_hadd_ps (R, R);
  return R;
}

float SIMD_Float_4x32_Dot (float * A, float * B)
{
  __m128 R4;
  float R;
  //R4 = SIMD_m128_Dot (_mm_load_ps (A), _mm_load_ps (B));
  //R4 = _mm_mul_ps (_mm_load_ps (A), _mm_load_ps (B));
  //R4 = _mm_hadd_ps (R4, R4);
  //R4 = _mm_hadd_ps (R4, R4);
  R4 =  _mm_dp_ps (_mm_load_ps (A), _mm_load_ps (B), 0b11111111);
  R = _mm_cvtss_f32 (R4);
  return R;
}

float Conv_Float_4x32_Dot (float * A, float * B)
{
  float R;
  R = (A[0]*B[0]) + (A[1]*B[1]) + (A[2]*B[2]) + (A[3]*B[3]);
  return R;
}


Float_Array_4 Vector_1 = {1.0f, 2.0f, 3.0f, 4.0f};
Float_Array_4 Vector_2 = {1.0f, 2.0f, 3.0f, 4.0f};
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
    R = SIMD_Float_4x32_Dot (Vector_1, Vector_2);
  }
  Wall_Time_1 = Get_Wall_Time ();
  CPU_Time_1 = Get_CPU_Time ();
  printf ("SIMD Wall-Time %lf\n", (Wall_Time_1 - Wall_Time_0));
  printf ("SIMD CPU Time %lf\n", (CPU_Time_1 - CPU_Time_0));
  printf ("R %f\n", R);
}

void Test_Conv ()
{
  Wall_Time_0 = Get_Wall_Time ();
  CPU_Time_0 = Get_CPU_Time ();
  float R;
  for (long long i = 0; i < Test_Count; i++)
  {
    R = Conv_Float_4x32_Dot (Vector_1, Vector_2);
  }
  Wall_Time_1 = Get_Wall_Time ();
  CPU_Time_1 = Get_CPU_Time ();
  printf ("Conv Wall-Time %lf\n", (Wall_Time_1 - Wall_Time_0));
  printf ("Conv CPU Time %lf\n", (CPU_Time_1 - CPU_Time_0));
  printf ("R %f\n", R);
}



int main (int argc, char** argv)
{

  #ifdef __SSE2__
    printf ("__SSE2__\n");
  #endif

  #ifdef __SSE3__
    printf ("__SSE3__\n");
  #endif

  Test_Conv ();
  //Test_SIMD ();


  getchar();
  return 0;
}

/*
SIMD Wall-Time 5.999117
SIMD CPU Time 5.984375

Conv Wall-Time 5.077319
Conv CPU Time 5.078125

*/
