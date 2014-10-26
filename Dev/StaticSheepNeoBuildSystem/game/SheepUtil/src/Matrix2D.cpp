/*****************************************************************************/
/*!
\file   Matrix2D.cpp
\author Zakary Wilson
\par    Course: GAM200
\par    Giga Gravity Games
\date   10/20/2014<BR>
\brief  
    This file contains functions to construct and perform matrix 
    multiplication<BR>

\par    All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/*****************************************************************************/

#include "src/precompiled.h"
#include "Matrix2D.h"
#include "SheepMath.h"

namespace Framework
{

/*****************************************************************************/
/*!
    \brief
      Default constructor for 2x2 matrix. Defaults to identity2x2
*/
/*****************************************************************************/
  Mat2D::Mat2D()
  {
    _Reset();
  }
  
/*****************************************************************************/
/*!
    \brief
      Non-default constructor, outright sets the 2x2 matrix to the x and y
      vectors passed in.
      
    \param left
      the x vector
      
    \param right
      the y vector
*/
/*****************************************************************************/
  Mat2D::Mat2D(const Vec2D& left, const Vec2D& right)
  {
    _matrix[0][0] = left.x;
    _matrix[1][0] = left.y;
    
    _matrix[1][0] = right.x;
    _matrix[1][1] = right.y;
  }
  
/*****************************************************************************/
/*!
    \brief
      Non-default constructor, sets it to a rotation matrix
      
    \param theta
      The degree to do the 2D rotation
*/
/*****************************************************************************/
  Mat2D::Mat2D(float theta)
  {
    float sine = sin(theta);
    float cosine = cos(theta);
    
    _matrix[0][0] = cosine;
    _matrix[0][1] = -sine;
    _matrix[1][0] = sine;
    _matrix[1][1] = cosine;
  }
  
/*****************************************************************************/
/*!
    \brief
      Non-default constructor for scale. 
      
    \param x
      the x scale
      
    \param y
      the y scale
*/
/*****************************************************************************/
  Mat2D::Mat2D(float x, float y)
  {
    _matrix[0][0] = x;
    _matrix[1][0] = 0.0f;
    
    _matrix[1][0] = 0.0f;
    _matrix[1][1] = y;
  }
  
/*****************************************************************************/
/*!
    \brief
      Non-default constructor for outrighting setting the values
      
    \param x0
      00 element
      
    \param y0
      01 element
      
    \param x1
      10 element
      
    \param y1
      11 element
*/
/*****************************************************************************/
  Mat2D::Mat2D(float x0, float y0, float x1, float y1)
  {
    matrix[0] = x0;
    matrix[1] = y0;
    matrix[2] = x1;
    matrix[3] = y1;
  }
  
/*****************************************************************************/
/*!
    \brief
      Sets the matrix to be a rotation matrix. In degrees.
      
    \param degrees
      The angle in degrees
*/
/*****************************************************************************/
  void Mat2D::SetRotationDegrees(float degrees)
  {
    float sine = (float)sin((degrees * PI) / 180.0f);
    float cosine = (float)cos((degrees * PI) / 180.0f);
    
    _matrix[0][0] = cosine;
    _matrix[0][1] = -sine;
    _matrix[1][0] = sine;
    _matrix[1][1] = cosine;
  }
  
/*****************************************************************************/
/*!
    \brief
      Sets the matrix to be a rotation matrix. In radians.
      
    \param degrees
      The angle in radians
*/
/*****************************************************************************/
  void Mat2D::SetRotationRadians(float radians)
  {
    float sine = (float)sin(radians);
    float cosine = (float)cos(radians);
    
    _matrix[0][0] = cosine;
    _matrix[0][1] = -sine;
    _matrix[1][0] = sine;
    _matrix[1][1] = cosine;
  }
  
/*****************************************************************************/
/*!
    \brief
      Sets the matrix to be a scale matrix.
      
    \param scale
      The vector to scale with. 
*/
/*****************************************************************************/
  void Mat2D::SetScale(const Vec2D& scale)
  {
    _matrix[0][0] = scale.x;
    _matrix[1][0] = 0.0f;
    
    _matrix[1][0] = 0.0f;
    _matrix[1][1] = scale.y;
  }
  
/*****************************************************************************/
/*!
    \brief
      Sets the matrix to be a scale matrix.
      
    \param x
      The x scale
      
    \param y
      The y scale
*/
/*****************************************************************************/
  void Mat2D::SetScale(float x, float y)
  {
    _matrix[0][0] = x;
    _matrix[1][0] = 0.0f;
    
    _matrix[1][0] = 0.0f;
    _matrix[1][1] = y;
  }
  
/*****************************************************************************/
/*!
    \brief
      Transposes the passed in matrix and spits out the transpose of it.
*/
/*****************************************************************************/
  Mat2D Mat2D::Transpose() const
  {
    return Mat2D(Vec2D(_matrix[0][0], _matrix[0][1]), 
                 Vec2D(_matrix[1][0], _matrix[1][1]));
                  
  }
  
/*****************************************************************************/
/*!
    \brief
      Inverses the current matrix.
*/
/*****************************************************************************/
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

/*****************************************************************************/
/*!
    \brief
      Operator overload for matrix multiplication.
      
    \param rhs
      The matrix on the right hand side.
*/
/*****************************************************************************/
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
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for matrix multiplication.
      
    \param rhs
      The matrix on the right hand side.
*/
/*****************************************************************************/
  Mat2D& Mat2D::operator*=(const Mat2D& rhs)
  {
    *this = *this * rhs;
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for matrix to vector multiplication.
      
    \param rhs
      The vector on the right hand side
*/
/*****************************************************************************/
  Vec2D Mat2D::operator*(const Vec2D& rhs) const
  {
    Vec2D result;
    
    result.x = _matrix[0][0] * rhs.x + _matrix[0][1] * rhs.y;
    result.x = _matrix[1][0] * rhs.x + _matrix[1][1] * rhs.y;
    
    return result;
  }

/*****************************************************************************/
/*!
    \brief
      Operator overload for matrix assignment.
      
    \param rhs
      The vector on the right hand side
*/
/*****************************************************************************/
  Mat2D& Mat2D::operator=(const Mat2D& rhs)
  {
    for(int i = 0; i < 2; ++i)
    {
      for(int j = 0; j < 2; ++j)
        _matrix[i][j] = rhs._matrix[i][j];
    }
    return *this;
  }

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
