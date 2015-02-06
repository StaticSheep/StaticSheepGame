/*****************************************************************
Filename: Vec4D.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "Vec3D.h"

namespace Framework
{
  struct Vec4D : public Vec3D
  {
    // constructor, defaults to homogenous 3D 0 vector
    Vec4D(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, 
         float _w = 1.0f) : Vec3D(_x, _y, _z)
    {w = _w;};
    
    // conversion constructor. Assume vector
    Vec4D(const Vec3D& copy){ Vec4D(copy.x, copy.y, copy.z, 0.0f);};
    
    // getters. This overwrites the Vec2 ones

    float Length();
    float LengthSquare();
    
    // setters. This overwrites the Vec2 ones
    Vec4D& Normalize();
    
    // overloads
    Vec4D operator+(const Vec4D&) const;  // add
    Vec4D& operator+=(const Vec4D&);      // plus equals
    Vec4D operator-(const Vec4D&) const;  // subtract
    Vec4D& operator-=(const Vec4D&);      // minus equals
    Vec4D operator-() const;              // negate
    float operator*(const Vec4D&) const;  // dot product
    Vec4D Vec4D::operator/(float scalar) const;   // scalar
    Vec4D& Vec4D::operator/=(float scalar);       // scalar
    Vec4D Vec4D::operator*(float scalar) const;   // scalar
    Vec4D& Vec4D::operator*=(float scalar);       // scalar
    Vec4D& operator=(const Vec4D&);       // assignment
    
    // data
    union
    {
      float w;
      float w_;
      float W;

      float a;
      float a_;
      float A;
    };
  };
  
  Vec4D operator*(float, const Vec4D&);
}