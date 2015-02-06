/*****************************************************************
Filename: Vec3D.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "Vec2D.h"

namespace Framework
{
  struct Vec3D : public Vec2D
  {
    // constructor, defaults to 0 vector
    Vec3D(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : Vec2D( _x, _y)
    {z = _z; };

    Vec3D(float* v) : Vec2D(*v, *(v + 1)), z(*(v + 2)) {};
    
    // conversion constructor, assume vector
    Vec3D(const Vec2D& copy) { x = copy.x; y = copy.y; z = 0.0f;};
    
	  //copy constructor
	  Vec3D(const Vec3D& copy) { x = copy.x; y = copy.y; z = copy.z; };

    // getters
    float Length();
    float SquareLength();
    Vec3D CalculateNormal();

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

    bool operator!=(const Vec3D&);
    
    // data
    union
    {
      float z;
      float z_;
      float Z;

      float b;
      float B;
    };
  };
  
  Vec3D operator*(float scalar, const Vec3D& rhs);
}
