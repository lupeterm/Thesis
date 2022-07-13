#pragma once

#include "math.h"
#include "Vector4d.h"
#include <iostream>

#define EPS 1.E-3
#define PI 3.1415926535897932384626433832795
#define PI2 6.283185307179586476925286766559
#define DEGREES_TO_RADIANS 0.0174532925199432957692369076848
#define RADIANS_TO_DEGREES 57.295779513082320876798154814105

/*inline int round(double x)
{
   return int(x > 0.0 ? x + 0.5 : x - 0.5);
}*/

inline int factorial(int n)
{
   if (n < 0 || n > 12)
   {
      std::cout << "made a booboo :(" << std::endl;
      exit(1);
   }
   int fact[13] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};
   return fact[n];
}

inline int combination(int n, int s)
{
   return factorial(n) / (factorial(s) * factorial(n - s));
}

inline int permutation(int n, int r)
{
   return factorial(n) / factorial(n - r);
}

inline double distance2plane(const Vector4d &point_plane, const Vector4d &normal_plane, const Vector4d &point)
{
   return abs((point - point_plane) & normal_plane.Normalized());
}

inline void sort(double &a, double &b, double &c)
{
   if (a < b)
   {
      if (a < c)
      {
         if (b > c)
         {
            std::swap(b, c);
         }
      }
      else
      {
         std::swap(a, b);
         std::swap(a, c);
      }
   }
   else
   {
      if (b < c)
      {
         if (a < c)
         {
            std::swap(a, b);
         }
         else
         {
            std::swap(a, b);
            std::swap(b, c);
         }
      }
      else
      {
         std::swap(a, c);
      }
   }
}
