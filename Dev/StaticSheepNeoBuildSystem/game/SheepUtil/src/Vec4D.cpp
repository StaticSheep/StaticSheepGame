/*****************************************************************
Filename: Vec4D.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "src/precompiled.h"
#include "../include/Vec4D.h"
#include "../include/Math.h"

namespace Framework
{

  //--- Getters ---//

  // Gets the length of the vector
  float Vec4D::Length()
  {
    return sqrtf( x * x + y * y + z * z);
  }
  
  // Gets the squared length
  float Vec4D::LengthSquare()
  {
    return (x * x + y * y + z * z);
  }
  
  // Normalizes the vector
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

  //--- Overloads ---//

  Vec4D Vec4D::operator+(const Vec4D& rhs) const
  {
    return Vec4D(rhs.x + x, rhs.y + y, rhs.z + z, rhs.w + w);
  }
  
  Vec4D& Vec4D::operator+=(const Vec4D& rhs)
  {
    *this = Vec4D(rhs.x + x, rhs.y + y, rhs.z + z, rhs.w + w);
    return *this;
  }
  
  Vec4D Vec4D::operator-(const Vec4D& rhs) const
  {
    return Vec4D(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
  }
  
  Vec4D& Vec4D::operator-=(const Vec4D& rhs)
  {
    *this = Vec4D(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    return *this;
  }
  
  Vec4D Vec4D::operator-() const
  {
    return Vec4D(-x,-y,-z,-w);
  }
  
  float Vec4D::operator*(const Vec4D& rhs) const
  {
    return (rhs.x * x + rhs.y * y + rhs.z * z + rhs.w * w);
  }

  Vec4D Vec4D::operator/(float scalar) const
  {
    return Vec4D(x / scalar, y / scalar, z / scalar, w / scalar);
  }
  
  Vec4D& Vec4D::operator/=(float scalar)
  {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;

    return *this;
  }
  
  Vec4D Vec4D::operator*(float scalar) const
  {
    return Vec4D(x * scalar, y * scalar, z * scalar, w * scalar);
  }
  
  Vec4D& Vec4D::operator*=(float scalar)
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;

    return *this;
  }
  
  Vec4D& Vec4D::operator=(const Vec4D& rhs)
  {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
    return *this;
  }
  
  Vec4D operator*(float scalar, const Vec4D& rhs)
  {
    return Vec4D(rhs.x * scalar, rhs.y * scalar, rhs.z * scalar, rhs.w);
  }
  
}
