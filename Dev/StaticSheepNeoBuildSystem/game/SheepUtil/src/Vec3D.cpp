/*****************************************************************
Filename: Vec3D.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "src/precompiled.h"
#include "Vec3D.h"
#include "SheepMath.h"

namespace Framework
{

  //--- Getters ---//

  // Gets the length
  float Vec3D::Length()
  {
    return sqrtf(x * x + y * y + z * z);
  }

  // Gets the square length
  float Vec3D::SquareLength()
  {
    return (x * x + y * y + z * z);
  }

  // Calculates the vector normal to this one
  Vec3D Vec3D::CalculateNormal()
  {
    Vec3D normal(y, -x);
    normal.Normalize();

    return normal;
  }

  // Normalizes the vector
  Vec3D& Vec3D::Normalize()
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
  
  // Rotates the vector by radians
  Vec3D& Vec3D::Rotate(float radians)
  {
    Vec3D temp(0,0);
    temp.x = x * cosf(radians) - y * sinf(radians);
    temp.y = x * sinf(radians) + y * cosf(radians);
    x = temp.x;
    y = temp.y;
    return *this;
  }
  
  // Computes the dot product
  float Vec3D::DotProduct(const Vec3D& rhs)
  {
    return (rhs.x * x + rhs.y * y + rhs.z * z);
  }
  
  // Computes the cross product
  Vec3D Vec3D::CrossProduct(Vec3D& rhs)
  {
    return Vec3D(y * rhs.z - z * rhs.y, 
                 z * rhs.x - x * rhs.z, 
                 x * rhs.y - y * rhs.x);
  }
  
  //--- Overloads ---//

  Vec3D Vec3D::operator+(const Vec3D& rhs) const
  {
    return Vec3D(rhs.x + x, rhs.y + y, rhs.z + z);
  }

  Vec3D& Vec3D::operator+=(const Vec3D& rhs)
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
  }
  
  Vec3D Vec3D::operator-(const Vec3D& rhs) const
  {
    return Vec3D(x - rhs.x, y - rhs.y, z - rhs.z);
  }
  
  Vec3D& Vec3D::operator-=(const Vec3D& rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
  }
  
  Vec3D Vec3D::operator-() const
  {
    return Vec3D(-x,-y,-z);
  }
  
  float Vec3D::operator*(const Vec3D& rhs) const
  {
    return (rhs.x * x + rhs.y * y + rhs.z * z);
  }
  
  Vec3D Vec3D::operator/(float scalar) const
  {
    return Vec3D(x / scalar, y / scalar, z / scalar);
  }
  
  Vec3D& Vec3D::operator/=(float scalar)
  {
    x /= scalar;
    y /= scalar;
    z /= scalar;

    return *this;
  }
  
  Vec3D Vec3D::operator*(float scalar) const
  {
    return Vec3D(x * scalar, y * scalar, z * scalar);
  }
  
  Vec3D& Vec3D::operator*=(float scalar)
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;

    return *this;
  }
  
  Vec3D Vec3D::operator^(const Vec3D& rhs) const
  {
    return Vec3D(y * rhs.z - z * rhs.y, 
                 z * rhs.x - x * rhs.z, 
                 x * rhs.y - y * rhs.x);
  }
  
  Vec3D& Vec3D::operator=(const Vec3D& rhs)
  {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
  }

  bool Vec3D::operator!=(const Vec3D& rhs)
  {
    if( this->x != rhs.x )
      return true;

    if( this->y != rhs.y )
      return true;

    if( this->z != rhs.z )
      return true;

    return false;
  }
  
  Vec3D operator*(float scalar, const Vec3D& rhs)
  {
    return Vec3D(rhs.x * scalar, rhs.y * scalar, rhs.z);
  }

}
