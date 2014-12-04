/*****************************************************************
Filename: Vec2D.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "src/precompiled.h"
#include "Vec2D.h"
#include "Math.h"

namespace Framework
{

  //--- Getters ---//

  // Gets the length
  float Vec2D::Length()
  {
    return sqrtf( x * x + y * y );
  }
  
  // Gets the square length
  float Vec2D::SquareLength()
  {
    return (x * x + y * y );
  }
  
  // Calculates a normal to this vector
  Vec2D Vec2D::CalculateNormal()
  {
	Vec2D normal(y, -x);
	normal.Normalize();
    
	return normal;
  }
  
  // Normalizes the vector
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
  
  // Rotates the vector by radians
  Vec2D& Vec2D::Rotate(float radians)
  {
    x = x * cos(radians) - y * sin(radians);
    y = x * sin(radians) + y * cos(radians);
    return *this;
  }

  //--- Overloads ---//

  Vec2D Vec2D::operator+(const Vec2D& rhs) const
  {
    return Vec2D(rhs.x + x, rhs.y + y);
  }
  
  Vec2D& Vec2D::operator+=(const Vec2D& rhs)
  {
    *this = Vec2D(x + rhs.x, y + rhs.y);
    return *this;
  }
  
  Vec2D Vec2D::operator-(const Vec2D& rhs) const
  {
    return Vec2D(x - rhs.x, y - rhs.y);
  }
  
  Vec2D& Vec2D::operator-=(const Vec2D& rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    
    return *this;
  }
  
  Vec2D Vec2D::operator-() const
  {
    return Vec2D(-x,-y);
  }
  
  float Vec2D::operator*(const Vec2D& rhs) const
  {
    return (rhs.x * x + rhs.y * y);
  }
  
  Vec2D Vec2D::operator*(float scale) const
  {
    return Vec2D(x * scale, y * scale);
  }
  
  float Vec2D::operator^(const Vec2D& rhs) const
  {
    return (x * rhs.y - y * rhs.x);
  }
  
  Vec2D& Vec2D::operator=(const Vec2D& rhs)
  {
    x = rhs.x;
    y = rhs.y;
    
    return *this;
  }
  
  Vec2D operator*(float scalar, const Vec2D& rhs)
  {
    return Vec2D(rhs.x * scalar, rhs.y * scalar);
  }
} // end namespace
