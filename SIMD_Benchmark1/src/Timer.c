#ifdef _WIN32
#include <Windows.h>

double Get_Wall_Time ()
{
  LARGE_INTEGER time;
  LARGE_INTEGER freq;
  if (!QueryPerformanceFrequency (&freq))
  {
    //Handle error
    return 0;
  }
  if (!QueryPerformanceCounter (&time))
  {
    //Handle error
    return 0;
  }
  return (double)time.QuadPart / freq.QuadPart;
}


double Get_CPU_Time()
{
  FILETIME a,b,c,d;
  if (GetProcessTimes (GetCurrentProcess (), &a, &b, &c, &d) != 0)
  {
    double T;
    T = (double)(d.dwLowDateTime | ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
    return T;
  }
  else
  {
    return 0;
  }
}


#elif __linux

#include <time.h>
#include <sys/time.h>

double Get_Wall_Time()
{
  struct timeval time;
  if (gettimeofday(&time,NULL))
  {
    return 0;
  }
  return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

double Get_CPU_Time()
{
  return (double)clock() / CLOCKS_PER_SEC;
}

#endif
