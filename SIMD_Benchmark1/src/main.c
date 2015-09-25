#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <time.h>
#include <tmmintrin.h>

#define TIMESPEC_PRINT_FORMAT "%ld.%06ld"

void timespec_print (struct timespec * a)
{
  printf (TIMESPEC_PRINT_FORMAT, (long int) a->tv_sec, (long int) a->tv_nsec);
}

void timespec_subtract (struct timespec * start, struct timespec * end, struct timespec * res)
{
  if ((end->tv_nsec - start->tv_nsec) < 0)
  {
    res->tv_sec = end->tv_sec - start->tv_sec - 1;
    res->tv_nsec = 1000000000 + end->tv_nsec - start->tv_nsec;
  }
  else
  {
    res->tv_sec = end->tv_sec - start->tv_sec;
    res->tv_nsec = end->tv_nsec - start->tv_nsec;
  }
}



void Test_SIMD (float c, float * x, float s, float * b, int count)
{

  const __m128 c4 = _mm_set1_ps(c);
  const __m128 s4 = _mm_set1_ps(s);
  __m128 b4s4;
  __m128 x4;
  __m128 b4;

  //float factor[4] __attribute__((aligned(16))) = {1.0, 2.0, 2.0, 3.0};
  //__m128 a = _mm_loadu_ps (factor);

  for (int i = 0; i < count; i += 4)
  {
    x4 = _mm_load_ps (x + i);
    x4 = _mm_mul_ps (x4, c4);
    b4 = _mm_load_ps (b + i);
    b4s4 = _mm_mul_ps (b4, s4);
    x4 = _mm_add_ps (x4, b4s4);
    _mm_store_ps (x + i, x4);
  }
}


void Test_Conventional (float c, float * x, float s, float * b, int count)
{
  for(int i = 0; i < count; i++)
  {
    x[i] = c * x[i] + s * b[i];
  }
}

#define Vector_Count 2000 * 4

float x [Vector_Count] __attribute__((aligned(16)));
float b [Vector_Count] __attribute__((aligned(16)));
float c = 3.0f;
float s = 2.0f;

int main (int argc, char** argv)
{

  struct timespec start;
  struct timespec stop;
  struct timespec delta;

  clock_gettime (CLOCK_MONOTONIC, &start);
  Test_SIMD (c, x, s, b, Vector_Count);
  clock_gettime (CLOCK_MONOTONIC, &stop);
  timespec_subtract (&start, &stop, &delta);
  printf ("SIMD Time elapsed: ");
  timespec_print (&delta);
  printf ("s\n\n");

  clock_gettime (CLOCK_MONOTONIC, &start);
  Test_Conventional (c, x, s, b, Vector_Count);
  clock_gettime (CLOCK_MONOTONIC, &stop);
  timespec_subtract (&start, &stop, &delta);
  printf ("Conventional Time elapsed: ");
  timespec_print (&delta);
  printf ("s\n\n");


  return 0;
}
