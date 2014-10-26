/******************************************************************************
filename    Matrix3D.h
project     Giga Gravity Games
author      Zakary Wilson
date        10/20/2014

Brief Description:
This header contains the 3x3 matrix class.

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#pragma once

#include "Vec3D.h"

namespace Framework
{
  class Mat3D
  {
  public:
    // constructors
    Mat3D();                                        // Default to identity
    Mat3D(const Vec2D&);                            // Translation
    Mat3D(const Vec3D&, const Vec3D&, const Vec3D&);// Construct with vectors
    Mat3D(float);                                   // Rotate
    Mat3D(float, float);                            // Scale
    
    // setters
    void SetTranslation(const Vec2D&);
    void SetTranslation(float x, float y, float z = 1.0f);
    
    void SetRotationDegrees(float degrees);
    void SetRotationRadians(float radians);
    
    void SetScale(const Vec2D&);
    void SetScale(float, float);
    
    void Inverse();
    
    Mat3D Transpose();
    
    // operator overloads
    Mat3D operator*(const Mat3D& rhs) const;
    Vec3D operator*(const Vec3D& rhs) const;
    Vec3D operator*(const Vec2D& rhs) const;
    Mat3D& operator*=(const Mat3D& rhs);
    Mat3D& operator=(const Mat3D& rhs);
    
    // the matrix data
    union
    {
      float _matrix[3][3];
      float matrix[9];
    };
    
  private:
    // reset to identity private method
    void _Reset();
  };
}
