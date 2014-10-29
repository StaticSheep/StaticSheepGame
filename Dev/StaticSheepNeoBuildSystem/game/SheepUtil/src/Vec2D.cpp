/*****************************************************************************/
/*!
\file   Vec2D.cpp
\author Zakary Wilson
\par    Course: GAM200
\par    Giga Gravity Games
\date   10/20/2014<BR>
\brief  
    This file contains functions to for 2D vector math.<BR>

\par    All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/*****************************************************************************/

#include "src/precompiled.h"
#include "Vec2D.h"
#include "Math.h"

namespace Framework
{

/*****************************************************************************/
/*!
    \brief
      Gets the length of the vector in 2D space.
*/
/*****************************************************************************/
  float Vec2D::Length()
  {
    return sqrtf( x * x + y * y );
  }
  
/*****************************************************************************/
/*!
    \brief
      Gets the square length of the vector in 2D space.
*/
/*****************************************************************************/
  float Vec2D::SquareLength()
  {
    return (x * x + y * y );
  }
  
/*****************************************************************************/
/*!
    \brief
      Calculates the normal of this vector and returns it. Non-modifying.
*/
/*****************************************************************************/
  Vec2D Vec2D::CalculateNormal()
  {
	Vec2D normal(y, -x);
	normal.Normalize();
    
	return normal;
  }
  
/*****************************************************************************/
/*!
    \brief
      Normalizes the vector in 2D space.
*/
/*****************************************************************************/
  Vec2D& Vec2D::Normalize()
  {
    float length = Length();
    
    //check to see if length is zero
    if (length == 0)
      return *this;

    x /= length;
    y /= length;
    
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Rotates the vector by radians.
*/
/*****************************************************************************/
  Vec2D& Vec2D::Rotate(float radians)
  {
    x = x * cos(radians) - y * sin(radians);
    y = x * sin(radians) + y * cos(radians);
    return *this;
  }

/*****************************************************************************/
/*!
    \brief
      Operator overload for adding 2D vectors together.
*/
/*****************************************************************************/
  Vec2D Vec2D::operator+(const Vec2D& rhs) const
  {
    return Vec2D(rhs.x + x, rhs.y + y);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for adding 2D vectors together.
*/
/*****************************************************************************/
  Vec2D& Vec2D::operator+=(const Vec2D& rhs)
  {
    *this = Vec2D(x + rhs.x, y + rhs.y);
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for subtracting 2D vectors together.
*/
/*****************************************************************************/
  Vec2D Vec2D::operator-(const Vec2D& rhs) const
  {
    return Vec2D(x - rhs.x, y - rhs.y);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for subtracting 2D vectors together.
*/
/*****************************************************************************/
  Vec2D& Vec2D::operator-=(const Vec2D& rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for negating a 2D vector.
*/
/*****************************************************************************/
  Vec2D Vec2D::operator-() const
  {
    return Vec2D(-x,-y);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for getting a dot product between two vectors.
*/
/*****************************************************************************/
  float Vec2D::operator*(const Vec2D& rhs) const
  {
    return (rhs.x * x + rhs.y * y);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for scaling a 2D vector.
*/
/*****************************************************************************/
  Vec2D Vec2D::operator*(float scale) const
  {
    return Vec2D(x * scale, y * scale);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for finding the 2D cross product.
*/
/*****************************************************************************/
  float Vec2D::operator^(const Vec2D& rhs) const
  {
    return (x * rhs.y - y * rhs.x);
  }
  
/*****************************************************************************/
/*!
    \brief
      Assignment operator overload.
*/
/*****************************************************************************/
  Vec2D& Vec2D::operator=(const Vec2D& rhs)
  {
    x = rhs.x;
    y = rhs.y;
    
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Non-member operator overload multiplying floats with a 2D vector.
*/
/*****************************************************************************/
  Vec2D operator*(float scalar, const Vec2D& rhs)
  {
    return Vec2D(rhs.x * scalar, rhs.y * scalar);
  }
} // end namespace
