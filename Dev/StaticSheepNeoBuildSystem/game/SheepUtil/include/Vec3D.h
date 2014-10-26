/******************************************************************************
filename    Vec3D.h
project     Giga Gravity Games
author      Zakary Wilson
date        10/20/2014

Brief Description:
This header contains the 2D homogenous vector struct.

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#pragma once

#include "Vec2D.h"

namespace Framework
{
  struct Vec3D : public Vec2D
  {
    // constructor, defaults to 0 vector
    Vec3D(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : Vec2D( _x, _y)
    {z = _z; };
    
    // conversion constructor, assume vector
    Vec3D(const Vec2D& copy) { x = copy.x; y = copy.y; z = 0.0f;};
    
    // setters
    Vec3D& Normalize();
    Vec3D& Rotate(float);
    
    float DotProduct(const Vec3D& rhs);
    Vec3D CrossProduct(Vec3D& rhs);
    
    
    // overloads
    Vec3D operator+(const Vec3D&) const;          // add
    Vec3D& operator+=(const Vec3D&);              // add equals
    Vec3D operator-(const Vec3D&) const;          // subtract
    Vec3D& operator-=(const Vec3D&);              // minus equals
    Vec3D operator-() const;                      // negate
    float operator*(const Vec3D&) const;          // dot product
    Vec3D Vec3D::operator/(float scalar) const;   // scalar
    Vec3D& Vec3D::operator/=(float scalar);       // scalar
    Vec3D Vec3D::operator*(float scalar) const;   // scalar
    Vec3D& Vec3D::operator*=(float scalar);       // scalar
    Vec3D operator^(const Vec3D&) const;          // cross product
    Vec3D& operator=(const Vec3D&);               // assignment
    
    // data
    union
    {
      float z;
      float z_;
      float Z;
    };
  };
  
  Vec3D operator*(float scalar, const Vec3D& rhs);
}
