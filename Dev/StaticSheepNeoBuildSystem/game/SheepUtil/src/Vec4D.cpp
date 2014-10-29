/*****************************************************************************/
/*!
\file   Vec4D.cpp
\author Zakary Wilson
\par    Course: GAM200
\par    Giga Gravity Games
\date   10/20/2014<BR>
\brief  
    This file contains functions to for 4D homogenous vector math.<BR>

\par    All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/*****************************************************************************/

#include "src/precompiled.h"
#include "../include/Vec4D.h"
#include "../include/Math.h"

namespace Framework
{

/*****************************************************************************/
/*!
    \brief
      Gets the length of the vector in 3D space.
*/
/*****************************************************************************/
  float Vec4D::Length()
  {
    return sqrtf( x * x + y * y + z * z);
  }
  
/*****************************************************************************/
/*!
    \brief
      Gets the squared length of the vector in 3D space.
*/
/*****************************************************************************/
  float Vec4D::LengthSquare()
  {
    return (x * x + y * y + z * z);
  }
  
/*****************************************************************************/
/*!
    \brief
      Normalizes the vector in 3D space.
*/
/*****************************************************************************/
  Vec4D& Vec4D::Normalize()
  {
    float length = Length();
    //check to see if length is zero
    if (length == 0)
      return *this;

    x /= length;
    y /= length;
    z /= length;
    
    return *this;
  }
/*****************************************************************************/
/*!
    \brief
      Operator overload for adding 4D vectors.
*/
/*****************************************************************************/
  Vec4D Vec4D::operator+(const Vec4D& rhs) const
  {
    return Vec4D(rhs.x + x, rhs.y + y, rhs.z + z, rhs.w + w);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for adding 4D vectors.
*/
/*****************************************************************************/
  Vec4D& Vec4D::operator+=(const Vec4D& rhs)
  {
    *this = Vec4D(rhs.x + x, rhs.y + y, rhs.z + z, rhs.w + w);
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for subtracting 4D vectors.
*/
/*****************************************************************************/
  Vec4D Vec4D::operator-(const Vec4D& rhs) const
  {
    return Vec4D(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for subtracting 4D vectors.
*/
/*****************************************************************************/
  Vec4D& Vec4D::operator-=(const Vec4D& rhs)
  {
    *this = Vec4D(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for negating 4D vectors.
*/
/*****************************************************************************/
  Vec4D Vec4D::operator-() const
  {
    return Vec4D(-x,-y,-z,-w);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for 4D dotproduct.
*/
/*****************************************************************************/
  float Vec4D::operator*(const Vec4D& rhs) const
  {
    return (rhs.x * x + rhs.y * y + rhs.z * z + rhs.w * w);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for assignment.
*/
/*****************************************************************************/
  Vec4D& Vec4D::operator=(const Vec4D& rhs)
  {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload multiplying floats with a 4D vector.
*/
/*****************************************************************************/
  Vec4D operator*(float scalar, const Vec4D& rhs)
  {
    return Vec4D(rhs.x * scalar, rhs.y * scalar, rhs.z * scalar, rhs.w);
  }
  
}