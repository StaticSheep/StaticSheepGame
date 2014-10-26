/******************************************************************************
filename    math.h
project     Go-Ninja, Road!
author      Zakary Wilson
date        01/23/2014

Brief Description:
This file contains math constants, prototypes for functions in math.c,
and structure declarations used in math.c.

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once

#include <math.h>
#include "Vec3D.h"

namespace Framework
{

/******************************************************************************
    Defines
******************************************************************************/

  const double PI = 3.14159265358979323846;
  
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
  float     CatSpline(double, float, float, float, float);
  float     Minimum(float a, float b);
  float     Maximum(float a, float b);
}
