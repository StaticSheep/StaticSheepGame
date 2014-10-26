/*****************************************************************************/
/*!
\file   math.cpp
\author Zakary Wilson
\par    Course: GAM200
\par    Giga Gravity Games
\date   10/20/2014<BR>
\brief  
    This file contains math functions to calculate square roots, normalize
    vectors and other such goodies.<BR>

\par    All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/*****************************************************************************/

/******************************************************************************
    Includes
******************************************************************************/

#include "src/precompiled.h"
#include "SheepMath.h" /* math functions */

/******************************************************************************
    Defines
******************************************************************************/

#define M11  0.0f
#define M12  1.0f
#define M13  0.0f
#define M14  0.0f
#define M21 -0.5f
#define M22  0.0f
#define M23  0.5f
#define M24  0.0f
#define M31  1.0f
#define M32 -2.5f
#define M33  2.0f
#define M34 -0.5f
#define M41 -0.5f
#define M42  1.5f
#define M43 -1.5f
#define M44  0.5f

/******************************************************************************
    Globals
******************************************************************************/

float Floating_Precision = 1000000.0f; //used in float to fractions f()

namespace Framework
{

/******************************************************************************
Private Functions
******************************************************************************/

  double    _POW(float,int);
  double    _Factorial(int);
  int       _GDC(int,int);
  Fraction  _FloatToFraction(float);
  double    _SquareRoot(float);


/*****************************************************************************/
/*!
    \brief
      Raises a float to the power of the passed int exponent.
    
    \param number
      Number to be raised to the power of exponent.
    
    \param exponent
      The power to raise number to.
      
    \return result
      Return a double, since the number could be potentially very large.
*/
/*****************************************************************************/
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

/*****************************************************************************/
/*!
    \brief
      Calculates the factorial of an integer
    
    \param number
      The number to find the factorial of. Positive only
      
    \return result
      Return a double, since the number could be potentially very large.
*/
/*****************************************************************************/
  double Factorial(int number)
  {
    if(number <= 0)
      return 0;

    return _Factorial(number);
  }

/*****************************************************************************/
/*!
    \brief
      Changes a float into a fraction(struct) of integers.
    
    \param number
      The number to find the integer fraction of.
      
    \return fract
      Return a struct that contains the numerator and denominator.
*/
/*****************************************************************************/
  Fraction FloatToFraction(float number)
  {
    Fraction Zero(0,0);
    if(number)
      return _FloatToFraction(number);
    else
      return Zero;
  }

/*****************************************************************************/
/*!
    \brief
      Approximates accurately the square root of a positive float.
    
    \param number
      The number to find the square root of.
      
    \return x
      Return the square root in double form, for maximum accuracy.
*/
/*****************************************************************************/
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

/*****************************************************************************/
/*!
    \brief
      Sets a Vec2 to unit length.
    
    \param Vector
      Pointer to the Vec2 to be normalized.
*/
/*****************************************************************************/
  void Normalize(Vec2D* Vector)
  {
    double length;

    /*length is  sqrt((x^2) + (y^2))*/
    length = SquareRoot( ( Vector->x * Vector->x ) + ( Vector->y * Vector->y ) );
    
    /*unit length = X / |X| where |X| is the magnitude(length) of the Vector*/
    Vector->x /= (float)length;
    Vector->y /= (float)length;

    return;
  }

/*****************************************************************************/
/*!
    \brief
      Sets a Vec3 to unit length.
    
    \param Vector
      Pointer to the Vec3 to be normalized.
*/
/*****************************************************************************/
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
  
/*****************************************************************************/
/*!
    \brief
      This approximates a curve with 4 given control points. It will make the
      curve between the two middle points. The curve will start and end on
      the second and third points.
    
    \param x
      The time, or position on the curve
      
    \param v0
      The first control point
      
    \param v1
      The second control point
    
    \param v2
      The third control point
      
    \param v3
      The fourth control point
*/
/*****************************************************************************/
  float CatSpline(double x, float v0, float v1, float v2, float v3)
  {
    float c1,c2,c3,c4;

    c1 =            M12 * v1;
    c2 = M21 * v0            + M23 * v2;
    c3 = M31 * v0 + M32 * v1 + M33 * v2 + M34 * v3;
    c4 = M41 * v0 + M42 * v1 + M43 * v2 + M44 * v3;
    
    return (float)(((c4 * x + c3) * x + c2) * x + c1);
  }
  
  float Minimum(float a, float b)
  {
    return ((a < b) ? a : b);
  }
  
  float Maximum(float a, float b)
  {
    return ((a > b) ? a : b);
  }

/******************************************************************************
    Private Functions
******************************************************************************/

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

/*****************************************************************************/

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

/*****************************************************************************/

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

/*****************************************************************************/

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

/*****************************************************************************/

  double _SquareRoot(float Number)
  {
    double x = (double)Number; /*need to keep Number intact*/
    double y = 1.0;            /*can only calculate numbers larger than 1*/
    double e = 0.0000000001;   /*precision value*/
    
    while(x - y > e) /*otherwise, do the following until we reach e precision*/
    {
      x = (x + y) / 2;
      y = (double)Number / x;
    }
    
    return x; /*then return what we found*/
  }
  
/*****************************************************************************/
  
} // end namespace Math


