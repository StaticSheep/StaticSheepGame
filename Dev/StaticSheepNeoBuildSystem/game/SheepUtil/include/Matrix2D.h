/******************************************************************************
filename    Matrix2D.h
project     Giga Gravity Games
author      Zakary Wilson
date        10/20/2014

Brief Description:
This header contains the 2x2 matrix class.

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#pragma once

#include "Vec2D.h"

namespace Framework
{

  class Mat2D
  {
  public:
    // constructors
    Mat2D();                            // Default to identity
    Mat2D(const Vec2D&, const Vec2D&);  // Construct with vectors
    Mat2D(float);                       // Rotate
    Mat2D(float, float);                // Scale
    Mat2D(float, float, float, float);  // Construct with floats
    
    // setters
    void SetRotationDegrees(float degrees);
    void SetRotationRadians(float radians);
    
    void SetScale(const Vec2D&);
    void SetScale(float, float);
    
    
    Mat2D Transpose() const;
    Mat2D Inverse() const;
    
    // operator overloads
    Mat2D operator*(const Mat2D& rhs) const;
    Mat2D& operator*=(const Mat2D& rhs);
    Mat2D& operator=(const Mat2D& rhs);
    Vec2D operator*(const Vec2D& rhs) const;

    void _Reset();
    
    // the actual matrix
    union
    {
      float _matrix[2][2];
      float matrix[4];
    };
  };
  
}