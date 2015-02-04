/*****************************************************************
Filename: SheepMath.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include <math.h>
#include "Vec3D.h"
#include "Vec4D.h"

namespace Framework
{

/******************************************************************************
    Struct Definitions
******************************************************************************/

  struct Fraction
  {
    Fraction()
    {Numerator = 1; Denominator = 1;};
    Fraction(int n,int d)
    {Numerator = n; Denominator = d;};
    
    int Numerator;
    int Denominator;
  };


  // t current time, b start value, c change in value, d duration

  // linear ((c * t) / d) + b

  //template<typename T>
  class Ease
  {
  public:

    template<typename T>
    static T Linear(float currentTime, T start, T end)
    {
      return start * (1.0f - currentTime) + (end * currentTime);
    }

    template<typename T>
    static T QuadraticIn(float t, T start, T end)
    {
      return start * (1.0f - t * t) + end * t * t;
    }

    template<typename T>
    static T QuadraticOut(float t, T start, T end)
    {
      return start * (1.0f - (t * ( 2.0f - t))) + end * (t * ( 2.0f - t));
    }

    template<typename T>
    static T QuadraticInOut(float t, T start, T end)
    {
      if(t < 0.5f)
        return start * (1.0f - 2.0f * t * t) + (end * 2.0f * t * t);
      else
      {
        t -= 0.5f;
        return start * (1.0f - 2.0f * t * (1.0f - t)) + (end * 2.0f * t * (1.0f * t));
      }
    }
  };
  
/******************************************************************************
    Public Functions
******************************************************************************/

  double    POW(float,int);
  double    Factorial(int);
  Fraction  FloatToFraction(float);
  double    SquareRoot(float);
  void      Normalize(Vec3D*);

  int       GetRandom(int min, int max);
  float     GetRandom(float min, float max);
  Vec3D     GetRandom(const Vec3D& min, const Vec3D& max);
  Vec4D     GetRandom(const Vec4D& min, const Vec4D& max);
  Vec3D     GetRandomNormalizedVector(const Vec3D& left, const Vec3D& right);
  float     Minimum(float a, float b);
  float     Maximum(float a, float b);
  float     CatSpline(double, float, float, float, float);
  float     Minimum(float a, float b);
  float     Maximum(float a, float b);
  float     Clamp(float a, float min, float max);
  float     DistanceFromLine(Vec2D ls, Vec2D le, Vec2D point);
  float     DistanceFromSegment(Vec2D s0, Vec2D s1, Vec2D p);
}
