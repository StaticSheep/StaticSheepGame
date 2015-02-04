/*****************************************************************
Filename: SheepMath.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "src/precompiled.h"
#include "SheepMath.h" /* math functions */
#include "boost/random.hpp"
#include <cstdlib>

// these are used for CatSpline
const float M11 =  0.0f;
const float M12 =  1.0f;
const float M13 =  0.0f;
const float M14 =  0.0f;
const float M21 = -0.5f;
const float M22 =  0.0f;
const float M23 =  0.5f;
const float M24 =  0.0f;
const float M31 =  1.0f;
const float M32 = -2.5f;
const float M33 =  2.0f;
const float M34 = -0.5f;
const float M41 = -0.5f;
const float M42 =  1.5f;
const float M43 = -1.5f;
const float M44 =  0.5f;

const double e = 0.0000000001; // epsilon precision value
const float Floating_Precision = 1000000.0f; //used in float to fractions f()

namespace Framework
{

  /* ----- Private Function Prototypes ----- */
  double    _POW(float,int);
  double    _Factorial(int);
  int       _GDC(int,int);
  Fraction  _FloatToFraction(float);
  double    _SquareRoot(float);

  // power function. Raises the number by the exponent
  double POW(float number, int exponent)
  {
    if(exponent == 1)
      return (double)number;

    if(exponent == 0)
      return 1.0;

    if(exponent < 0)
      return ( 1.0 / _POW(number, -exponent) );

    return _POW(number, exponent);
  }

  // Calculates the factorial of the number. Returns double because it could
  // be very large
  double Factorial(int number)
  {
    if(number < 0)
      return 0;

    if(number == 0)
      return 1;

    return _Factorial(number);
  }

  // Converts a float into a fraction, which would be represented by integers
  Fraction FloatToFraction(float number)
  {
    Fraction Zero(0,0);
    if(number)
      return _FloatToFraction(number);
    else
      return Zero;
  }

  // Calculates the square root of the number
  double SquareRoot(float Number)
  {
    Fraction fract(0,0);
    int x, y;

    if( Number <= 0.0f )
      return Number;

    if( Number < 1.0f ) // between 0 and 1 is a special case, need to calculate
    {                   // calculate a fraction
      fract = _FloatToFraction(Number);
      x = (int)_SquareRoot((float)fract.Numerator);
      y = (int)_SquareRoot((float)fract.Denominator);
      return ( x / y );
    }

    return _SquareRoot(Number);
  }

  // Normalizes a 2D Vector
  void Normalize(Vec2D* Vector)
  {
    double length = SquareRoot( ( Vector->x * Vector->x ) + 
                                ( Vector->y * Vector->y ) );
    
    /*unit length = X / |X| where |X| is the magnitude(length) of the Vector*/
    Vector->x /= (float)length;
    Vector->y /= (float)length;

    return;
  }

  // Normalizes a 3D Vector
  void Normalize(Vec3D* Vector)
  {
    float length;
    
    /*length is sqrt((x^2) + (y^2) + (z^2))*/
    length = (float)SquareRoot( ( Vector->x * Vector->x ) + 
                                ( Vector->y * Vector->y ) );
    
    /*unit length = X / |X| where |X| is the magnitude(length) of the Vector*/
    Vector->x /= (float)length;
    Vector->y /= (float)length;
    
    return;
  }
  
/*
  This calculates a curve between 4 control points. This first parameter is the
  time where the value should be calculated on the curve. The curve is only valid
  between the two middle control points.
*/
  float CatSpline(double x, float v0, float v1, float v2, float v3)
  {
    float c1,c2,c3,c4;

    c1 =            M12 * v1;
    c2 = M21 * v0            + M23 * v2;
    c3 = M31 * v0 + M32 * v1 + M33 * v2 + M34 * v3;
    c4 = M41 * v0 + M42 * v1 + M43 * v2 + M44 * v3;
    
    return (float)(((c4 * x + c3) * x + c2) * x + c1);
  }
  
  static boost::random::mt19937 rng((uint32_t)std::time(0));

  int GetRandom(int min, int max)
  {
    boost::random::uniform_int_distribution<> range(min, max);
    return range(rng);
  }

  float GetRandom(float min, float max)
  {
    if(min == max)
      return min;

    if(min > max)
    {
      float temp = min;
      min = max;
      max = temp;
    }

    boost::random::uniform_real_distribution<> range(min, max);
    return (float)range(rng);
  }

  Vec3D GetRandomNormalizedVector(const Vec3D& left, const Vec3D& right)
  {
    int scalar = GetRandom(0, 100);

    if(scalar % 2)
    {
      Vec3D result(left * ( 50.0f / (float)scalar) + right);
      return result.Normalize();
    }
    else
    {
      return Vec3D(left + ( 50.0f / (float)scalar) * right).Normalize();
    }
  }

  Vec3D GetRandom(const Vec3D& min, const Vec3D& max)
  {
    float x = GetRandom(min.x, max.x);
    float y = GetRandom(min.y, max.y);
    float z = GetRandom(min.z, max.z);

    return Vec3D(x, y, z);
  }

  Vec4D GetRandom(const Vec4D& min, const Vec4D& max)
  {
    Vec3D rand = GetRandom((Vec3D)min, (Vec3D)max);
    float w = GetRandom(min.w, max.w);

    return Vec4D(rand.x, rand.y, rand.z, w);
  }

/*!
    \brief
      Gets the smallest of the two numbers
*/
  float Minimum(float a, float b)
  {
    return ((a < b) ? a : b);
  }
  
/*!
    \brief
      Gets the largest of the two numbers
*/
  float Maximum(float a, float b)
  {
    return ((a > b) ? a : b);
  }

/*!
    \brief
      Clamps a number between two values
*/
  float Clamp(float a, float min, float max)
  {
    if(a < min)
      return min;

    if(a > max)
      return max;

    return a;
  }

/******************************************************************************
    Private Functions
******************************************************************************/

/*!
    \brief
      Calculates the greatest common denominator
*/
  int _GCD(int a, int b)
  {
    int m = 0;
    int n = 0;
    int r = 0;
    
    //edge case detection
      if(a == 0)
        return b;
      
      if(b == 0)
        return a;

      if(a < 0)
        a = -a;
      
      if(b < 0)
        b = -b;
    
    //set up the values of m and n correctly
      if(a > b)
      {
        m = a;
        n = b;
      }
      if(a < b)
      {
        n = a;
        m = b;
      }
      if(a == b)
      {
        return 0; //return if they are the same number
      }

    r = m % n;
    //standard gcd calculation loop
      while (r != 0)
      {
        m = n;
        n = r;
        r = m % n; //iterates until the gcd is found
      }

    return n; //then returns it
  }

/*!
    \brief
      Simple power function with integral exponents
*/
  double _POW(float number, int exponent)
  {
    int i;
    double result = (double)number;

    /* main calculation. Multiply the number by itself exponent amount of times*/
    for(i = 1; i < exponent; ++i)
    {
      result *= number;
    }

    if(exponent < 0)/* this may need to change later */
      return ( 1.0 / result);

    return result;
  }

/*!
    \brief
      Calculates the factorial of a number.
*/
  double _Factorial(int number)
  {
    int i;
    /*set result to 1 for easier calculations... */
    double result = 1.0;

    /* start at 1, then * 2, * 3, * 4... * N amount of times */
    for(i = 2; i <= number; ++i)
    {
      result *= i;
    }

    return result;
  }

/*!
    \brief
      Turns a float into a fraction value of integers
*/
  Fraction _FloatToFraction(float number)
  {
    //get rid of the decimal value.
    int numerator = (int)(number * Floating_Precision);
    int denominator = (int)Floating_Precision;

    //declare a fraction struct. Has numerator and denominator
    Fraction fract; 

    //find the greatest common denominator between the two.
    int gcd = _GCD(numerator, denominator);

    //divide the numerator and denominator by the gcd
    fract.Numerator = numerator / gcd;
    fract.Denominator = denominator / gcd;

    //this returns a struct that has the correct numerator and denominator
    return fract;

  }

/*!
    \brief
      Approximates the square root of a number.
*/
  double _SquareRoot(float Number)
  {
    double x = (double)Number; /* need to keep Number intact */
    double y = 1.0;            /* can only calculate numbers larger than 1 */
    
    while(x - y > e) /* otherwise, do the following until we reach e precision */
    {
      x = (x + y) / 2;
      y = (double)Number / x;
    }
    
    return x; /* then return what we found */
  }

  float DistanceFromLine(Vec2D ls, Vec2D le, Vec2D point)
  {
    return std::abs((le.y - ls.y) * point.x - (le.x - ls.x) * point.y + le.x *
      ls.y - le.y * ls.x) / std::sqrtf((le.y - ls.y) * (le.y - ls.y) +
      (le.x - ls.x) * (le.x - ls.x));
  }

  float DistanceFromSegment(Vec2D s0, Vec2D s1, Vec2D p)
  {
    Vec2D v = s1 - s0;
    Vec2D w = p - s0;

    float c1 = w * v;
    if (c1 <= 0)
      return (p - s0).Length();

    float c2 = v * v;
    if (c2 <= c1)
      return (p - s1).Length();

    float b = c1 / c2;
    Vec2D pb = s0 + b * v;
    return (p - pb).Length();
  }
} // end namespace
