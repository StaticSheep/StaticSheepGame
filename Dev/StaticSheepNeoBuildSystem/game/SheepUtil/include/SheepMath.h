/*****************************************************************
Filename: SheepMath.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include <math.h>
#include "Vec3D.h"

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
  
/******************************************************************************
    Public Functions
******************************************************************************/

  double    POW(float,int);
  double    Factorial(int);
  Fraction  FloatToFraction(float);
  double    SquareRoot(float);
  void      Normalize(Vec3D*);
  int       GetRandom(int min, int max);
  float     Minimum(float a, float b);
  float     Maximum(float a, float b);
  float     CatSpline(double, float, float, float, float);
  float     Minimum(float a, float b);
  float     Maximum(float a, float b);
  float     Clamp(float a, float min, float max);
}
