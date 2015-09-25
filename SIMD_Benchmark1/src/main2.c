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


#define COUNT 10000
void Test_SIMD ()
{
  float data[COUNT] __attribute__((aligned(16))) = {0.0f};
  const __m128 a = _mm_set1_ps(10.0f);
  __m128 temp;
  for (int i = 0; i < COUNT; i = i + 4)
  {
    temp = _mm_load_ps (data + i);
    temp = _mm_add_ps (temp, a);
    _mm_store_ps (data + i, temp);
  }
  /*
  for (int i = 0; i < COUNT; i = i + 1)
  {
    printf ("%f\n", data[i]);
  }
  */
}

void Test_Conventional ()
{
  float data[COUNT] = {0.0f};
  for (int i = 0; i < COUNT; i = i + 1)
  {
    data[i] = data[i] + 10.0f;
  }
  /*
  for (int i = 0; i < COUNT; i = i + 1)
  {
    printf ("%f\n", data[i]);
  }
  */
}
#undef COUNT


struct timespec start;
struct timespec stop;
struct timespec delta;

void Run_Test_Conventional()
{
  clock_gettime (CLOCK_MONOTONIC, &start);
  Test_Conventional ();
  clock_gettime (CLOCK_MONOTONIC, &stop);
  timespec_subtract (&start, &stop, &delta);
  printf ("Conventional Time elapsed: ");
  timespec_print (&delta);
  printf ("s\n\n");
}

void Run_Test_SIMD()
{
  clock_gettime (CLOCK_MONOTONIC, &start);
  Test_SIMD ();
  clock_gettime (CLOCK_MONOTONIC, &stop);
  timespec_subtract (&start, &stop, &delta);
  printf ("SIMD Time elapsed: ");
  timespec_print (&delta);
  printf ("s\n\n");
}



int main (int argc, char** argv)
{

  Run_Test_SIMD();
  Run_Test_Conventional();
  Run_Test_SIMD();
  Run_Test_Conventional();
  Run_Test_SIMD();
  Run_Test_Conventional();
  Run_Test_SIMD();
  Run_Test_Conventional();
  return 0;
}
