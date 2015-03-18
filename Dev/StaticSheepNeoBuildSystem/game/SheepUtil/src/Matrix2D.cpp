/*****************************************************************
Filename: Matrix2D.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "src/precompiled.h"
#include "Matrix2D.h"
#include "SheepMath.h"

namespace Framework
{

  // Constructors
  Mat2D::Mat2D()
  {
    _Reset();
  }
  
  // Builds the matrix outright with vectors
  Mat2D::Mat2D(const Vec2D& left, const Vec2D& right)
  {
    _matrix[0][0] = left.x;
    _matrix[1][0] = left.y;
    
    _matrix[1][0] = right.x;
    _matrix[1][1] = right.y;
  }
  
  // Creates a rotation matrix
  Mat2D::Mat2D(float theta)
  {
    float sine = sin(theta);
    float cosine = cos(theta);
    
    _matrix[0][0] = cosine;
    _matrix[0][1] = -sine;
    _matrix[1][0] = sine;
    _matrix[1][1] = cosine;
  }
  
  // Creates a scale matrix
  Mat2D::Mat2D(float x, float y)
  {
    _matrix[0][0] = x;
    _matrix[1][0] = 0.0f;
    
    _matrix[1][0] = 0.0f;
    _matrix[1][1] = y;
  }
  
  // Creates a matrix explicitly with the values. 
  Mat2D::Mat2D(float x0, float y0, float x1, float y1)
  {
    matrix[0] = x0;
    matrix[1] = y0;
    matrix[2] = x1;
    matrix[3] = y1;
  }
  
  // Sets the matrix to rotation given in degrees
  void Mat2D::SetRotationDegrees(float degrees)
  {
    float sine = (float)sin((degrees * PI) / 180.0f);
    float cosine = (float)cos((degrees * PI) / 180.0f);
    
    _matrix[0][0] = cosine;
    _matrix[0][1] = -sine;
    _matrix[1][0] = sine;
    _matrix[1][1] = cosine;
  }
  
  // Sets the matrix to rotation given in radians
  void Mat2D::SetRotationRadians(float radians)
  {
    float sine = (float)sin(radians);
    float cosine = (float)cos(radians);
    
    _matrix[0][0] = cosine;
    _matrix[0][1] = -sine;
    _matrix[1][0] = sine;
    _matrix[1][1] = cosine;
  }
  
  // Sets the matrix to scale, given by a vector
  void Mat2D::SetScale(const Vec2D& scale)
  {
    _matrix[0][0] = scale.x;
    _matrix[1][0] = 0.0f;
    
    _matrix[1][0] = 0.0f;
    _matrix[1][1] = scale.y;
  }
  
  // Sets the matrix to scale, with two floats
  void Mat2D::SetScale(float x, float y)
  {
    _matrix[0][0] = x;
    _matrix[1][0] = 0.0f;
    
    _matrix[1][0] = 0.0f;
    _matrix[1][1] = y;
  }
  
  // Returns a transpose of the matrix
  Mat2D Mat2D::Transpose() const
  {
    return Mat2D(Vec2D(_matrix[0][0], _matrix[0][1]), 
                 Vec2D(_matrix[1][0], _matrix[1][1]));
                  
  }
  
  // Returns the inverse of this matrix
  Mat2D Mat2D::Inverse() const
  {
    float a = _matrix[0][0];
    float b = _matrix[0][1];
    float c = _matrix[1][0];
    float d = _matrix[1][1];
    
    // get the determinant, then invert it
    float inverseDet = 1.0f / (( a * d ) - ( b * c ));
    
    Vec2D x(d, -c);        //  | D -B |
    Vec2D y(-b, a);        //  | -C A |
    
    x = x * inverseDet;     //  1 | D -B |    | A B | ^-1
    y = y * inverseDet;     // det| -C A | =  | C D |
    
    //Vector negTrans(-A.matrix[0][2], -A.matrix[1][2]);
    
    Mat2D linear(x,y);
    
    return linear;
  }

  // Matrix multiplication
  Mat2D Mat2D::operator*(const Mat2D& rhs) const
  {
    int j;
    Mat2D C;
    
    for(int i = 0; i < 2; ++i)
    {
      j = 0;
                       // row stays same  | column changes after each calculation
      C._matrix[i][j] =   (_matrix[i][0] * rhs._matrix[0][j]) + 
                          (_matrix[i][1] * rhs._matrix[1][j]);
                          
      ++j;

      C._matrix[i][j] =   (_matrix[i][0] * rhs._matrix[0][j]) + 
                          (_matrix[i][1] * rhs._matrix[1][j]);
      
    }
    
    return C;
  }
  
  // Matrix multiplication
  Mat2D& Mat2D::operator*=(const Mat2D& rhs)
  {
    *this = *this * rhs;
    return *this;
  }
  
  // Matrix vector multiplication
  Vec2D Mat2D::operator*(const Vec2D& rhs) const
  {
    Vec2D result;
    
    result.x = _matrix[0][0] * rhs.x + _matrix[0][1] * rhs.y;
    result.y = _matrix[1][0] * rhs.x + _matrix[1][1] * rhs.y;
    
    return result;
  }

  // Assignment
  Mat2D& Mat2D::operator=(const Mat2D& rhs)
  {
    for(int i = 0; i < 2; ++i)
    {
      for(int j = 0; j < 2; ++j)
        _matrix[i][j] = rhs._matrix[i][j];
    }
    return *this;
  }

  // Resets the matrix to the identity matrix
  void Mat2D::_Reset()
  {
    for (int i = 0; i < 2; ++i)
    {
      for (int j = 0; j < 2; ++j)
      {
        if (i == j)
          _matrix[i][j] = 1.0f;
        else
          _matrix[i][j] = 0.0f;
      }
    }
  }
  
}
