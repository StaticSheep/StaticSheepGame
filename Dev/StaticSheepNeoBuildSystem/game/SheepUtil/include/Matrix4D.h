/*****************************************************************
Filename: Matrix4D.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "Vec4D.h"
#include <array>

namespace Framework
{

  class Mat4D
  {
  public:
    
    // constructors
    Mat4D();                                                       // Default to identity
    Mat4D(const Vec3D&);                                           // Translation
    Mat4D(const Vec4D&);                                           // Translation
    Mat4D(const Vec4D&, const Vec4D&, const Vec4D&, const Vec4D&); // Construct with vectors
    Mat4D(const Vec4D&, const Vec4D&);                             // Transpose constructor
    Mat4D(float, float, float);                                    // Scale
    Mat4D(float*);
    
    // setters
    void SetTranslation(const Vec3D&);
    void SetTranslation(const Vec4D&);
    void SetTranslation(float x, float y, float z = 1.0f, float w = 1.0f);
    
    void SetScale(const Vec2D&); // 2D scale
    void SetScale(const Vec3D&); // 3D scale, probably won't be used
    
    Mat4D Transpose();
    
    Mat4D& operator*=(const Mat4D& rhs);
    Mat4D& operator=(const Mat4D& rhs);
    Mat4D operator*(const Mat4D& rhs) const;
    
    // for the funsies
    void SetPerspective(Vec4D& plane, Vec4D& position);
    
    // matrix data
    
    union
    {
      float _matrix[4][4];
      float matrix[16];
    };
    
  private:
    // private method for resetting to identity matrix
    void _Reset();
  };
}
