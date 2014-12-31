/*****************************************************************
Filename: Matrix4D.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "src/precompiled.h"
#include "Matrix4D.h"
#include "SheepMath.h"

namespace Framework
{

  // Constructors
  Mat4D::Mat4D()
  {
    _Reset();
  }
  
  // Sets the matrix to translate by the vector
  Mat4D::Mat4D(const Vec3D& vec)
  {
    _Reset();
    _matrix[0][3] = vec.x;
    _matrix[1][3] = vec.y;
    _matrix[2][3] = vec.z;
  }
  
  // Sets the matrix to translate by the vector
  Mat4D::Mat4D(const Vec4D& vec)
  {
    _Reset();
    _matrix[0][3] = vec.x;
    _matrix[1][3] = vec.y;
    _matrix[2][3] = vec.z;
    _matrix[3][3] = vec.w; // ummmm....
  }
  
  // Construct the matrix outright with the vectors passed in
  Mat4D::Mat4D(const Vec4D& r, const Vec4D& p, const Vec4D& y, const Vec4D& vec)
  {
    // roll
    _matrix[0][0] = r.x;
    _matrix[1][0] = r.y;
    _matrix[2][0] = r.z;
    _matrix[3][0] = r.w;
    
    // pitch
    _matrix[0][1] = p.x;
    _matrix[1][1] = p.y;
    _matrix[2][1] = p.z;
    _matrix[3][1] = p.w;
    
    // yaw
    _matrix[0][2] = y.x;
    _matrix[1][2] = y.y;
    _matrix[2][2] = y.z;
    _matrix[3][2] = y.w;
    
    // position
    _matrix[0][3] = vec.x;
    _matrix[1][3] = vec.y;
    _matrix[2][3] = vec.z;
    _matrix[3][3] = vec.w;
  }
  
  // Constructs the matrix by transposing to Vector4s
  Mat4D::Mat4D(const Vec4D& VectorA, const Vec4D& VectorB)
  {
    _matrix[0][0] = VectorA.x * VectorB.x;
    _matrix[0][1] = VectorA.x * VectorB.y;
    _matrix[0][2] = VectorA.x * VectorB.z;
    _matrix[0][3] = VectorA.x * VectorB.w;

    _matrix[1][0] = VectorA.y * VectorB.x;
    _matrix[1][1] = VectorA.y * VectorB.y;
    _matrix[1][2] = VectorA.y * VectorB.z;
    _matrix[1][3] = VectorA.y * VectorB.w;

    _matrix[2][0] = VectorA.z * VectorB.x;
    _matrix[2][1] = VectorA.z * VectorB.y;
    _matrix[2][2] = VectorA.z * VectorB.z;
    _matrix[2][3] = VectorA.z * VectorB.w;

    _matrix[3][0] = VectorA.w * VectorB.x;
    _matrix[3][1] = VectorA.w * VectorB.y;
    _matrix[3][2] = VectorA.w * VectorB.z;
    _matrix[3][3] = VectorA.w * VectorB.w;
  }
  
  // Scale matrix
  Mat4D::Mat4D(float x, float y, float z)
  {
    _Reset();
    _matrix[0][0] = x;
    _matrix[1][1] = y;
    _matrix[2][2] = z;
  }

  Mat4D::Mat4D(float* m)
  {
    _Reset();
    memcpy(_matrix, m, sizeof(float)* 16);
  }
  
  // Setters
  void Mat4D::SetTranslation(const Vec3D& vec)
  {
    _Reset();
    _matrix[0][3] = vec.x;
    _matrix[1][3] = vec.y;
    _matrix[2][3] = vec.z;
  }
  
  void Mat4D::SetTranslation(const Vec4D& vec)
  {
    _Reset();
    _matrix[0][3] = vec.x;
    _matrix[1][3] = vec.y;
    _matrix[2][3] = vec.z;
    _matrix[3][3] = vec.w; // I don't know anymore...
  }
  
  void Mat4D::SetTranslation(float x, float y, float z, float w)
  {
    _Reset();
    _matrix[0][3] = x;
    _matrix[1][3] = y;
    _matrix[2][3] = z;
    _matrix[3][3] = w;
  }
  
  void Mat4D::SetScale(const Vec2D& vec)
  {
    _Reset();
    _matrix[0][0] = vec.x;
    _matrix[1][1] = vec.y;
  }
  
  void Mat4D::SetScale(const Vec3D& vec)
  {
    _Reset();
    _matrix[0][0] = vec.x;
    _matrix[1][1] = vec.y;
    _matrix[2][2] = vec.z;
  }
  
  // Returns a transpost of the matrix
  Mat4D Mat4D::Transpose()
  {
    return Mat4D
    (Vec4D(_matrix[0][0], _matrix[0][1], _matrix[0][2], _matrix[0][3]),
     Vec4D(_matrix[1][0], _matrix[1][1], _matrix[1][2], _matrix[1][3]),
     Vec4D(_matrix[2][0], _matrix[2][1], _matrix[2][2], _matrix[2][3]),
     Vec4D(_matrix[3][0], _matrix[3][1], _matrix[3][2], _matrix[3][3]));
  }
  
  // Operator overloads
  Mat4D& Mat4D::operator*=(const Mat4D& rhs)
  {
    *this = *this * rhs;
    return *this;
  }
  
  Mat4D& Mat4D::operator=(const Mat4D& rhs)
  {
    for(int i = 0; i < 4; ++i)
    {
      for(int j = 0; j < 4; ++j)
        _matrix[i][j] = rhs._matrix[i][j];
    }
    
    return *this;
  }

  Mat4D Mat4D::operator*(const Mat4D& rhs) const
  {
    int i,j;
    Mat4D C;
    
    // every pass, complete one row of the result matrix.
    for(i = 0; i < 4; ++i)
    {
      j = 0;
                   // row stays same  | column changes after each calculation
      C._matrix[i][j] =   (_matrix[i][0] * rhs._matrix[0][j]) + 
                          (_matrix[i][1] * rhs._matrix[1][j]) + 
                          (_matrix[i][2] * rhs._matrix[2][j]) + 
                          (_matrix[i][3] * rhs._matrix[3][j]);
                          
      ++j;

      C._matrix[i][j] =   (_matrix[i][0] * rhs._matrix[0][j]) + 
                          (_matrix[i][1] * rhs._matrix[1][j]) + 
                          (_matrix[i][2] * rhs._matrix[2][j]) +
                          (_matrix[i][3] * rhs._matrix[3][j]);
                          
      ++j;

      C._matrix[i][j] =   (_matrix[i][0] * rhs._matrix[0][j]) + 
                          (_matrix[i][1] * rhs._matrix[1][j]) + 
                          (_matrix[i][2] * rhs._matrix[2][j]) +
                          (_matrix[i][3] * rhs._matrix[3][j]);
                          
      ++j;

      C._matrix[i][j] =   (_matrix[i][0] * rhs._matrix[0][j]) + 
                          (_matrix[i][1] * rhs._matrix[1][j]) + 
                          (_matrix[i][2] * rhs._matrix[2][j]) +
                          (_matrix[i][3] * rhs._matrix[3][j]);
    }
    
    return C;
  }
  
/*****************************************************************************/
/*!
    \brief
      Constructs a stereographic projection matrix. 
      P = ( q~ * n~ ^t ) - ( q~ dot n~) * I
      Where q~ is the 3D homogenous coordinate of the point to project to
      and n~ is the normal to the plane to paint to. [[a][b][c][-d]]
      
    \param Result
      Pointer to the matrix to put the result into

    \param Position
      Pointer to the point coordinates

    \param Plane
      Pointer to the plane coordinates
*/
/*****************************************************************************/
  void Mat4D::SetPerspective(Vec4D& plane, Vec4D& position)
  {
    *this = Mat4D(plane, position); // transpose constructor
    float dotProduct = position * plane;

    for(int i = 0; i < 4; ++i)
    {
      for(int j = 0; j < 4; ++j)
      {
        if( i == j)
          _matrix[i][j] -= dotProduct;
      }
    }
  }
  
  // Resets the matrix to the identity
  void Mat4D::_Reset()
  {
    for(int i = 0; i < 4; ++i)
    {
      for(int j = 0; j < 4; ++j)
      {
        if(i == j)
          _matrix[i][j] = 1.0f;
        else
          _matrix[i][j] = 0.0f;
      }
    }
  }
  
} // end namespace
