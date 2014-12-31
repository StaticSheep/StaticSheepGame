/*****************************************************************
Filename: Vec2D.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  const double PI = 3.14159265358979323846;

  struct Vec2D
  {
    // constructor, defaults to 0 vector
    Vec2D(float _x = 0.0f, float _y = 0.0f)
    {x = _x; y = _y;};

    Vec2D(float* v) : x(*v), y(*(v + 1)) {};
    
    // getters
    float Length();
    float SquareLength();
    Vec2D CalculateNormal();
    
    // setters
    Vec2D& Normalize();
    Vec2D& Rotate(float);
    
    // overloads
    Vec2D operator+(const Vec2D&) const;  // add
    Vec2D& operator+=(const Vec2D&);      // add equals
    Vec2D operator-(const Vec2D&) const;  // subtract
    Vec2D& operator-=(const Vec2D&);      // minus equals
    Vec2D operator-() const;              // negate
    float operator*(const Vec2D&) const;  // dot product
    Vec2D operator*(float) const;         // scale
    float operator^(const Vec2D&) const;  // cross product
    Vec2D& operator=(const Vec2D&);       // equals overload
    
    // data
    union
    {
      float x;
      float x_;
      float X;

      float r;
      float R;
    };
    
    union
    {
      float y;
      float y_;
      float Y;

      float g;
      float G;
    };
  };
  
  Vec2D operator*(float, const Vec2D&);
}
