/*****************************************************************************/
/*!
\file   Matrix3D.cpp
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
#include "Matrix3D.h"
#include "SheepMath.h"

namespace Framework
{

/*****************************************************************************/
/*!
    \brief
      Default constructor for a 3x3 matrix. Defaults to identity.
*/
/*****************************************************************************/
  Mat3D::Mat3D()
  {
    _Reset();
  }
  
/*****************************************************************************/
/*!
    \brief
      Non-default constructor. Sets the matrix to be translation.
      
    \param trans
      The translation vector
*/
/*****************************************************************************/
  Mat3D::Mat3D(const Vec2D& trans)
  {
    _Reset();
    _matrix[0][2] = trans.x;
    _matrix[1][2] = trans.y;
  }
  
/*****************************************************************************/
/*!
    \brief
      Non-default constructor. Sets the matrix to be rotation.
      
    \param radians
      The angle in radians
*/
/*****************************************************************************/
  Mat3D::Mat3D(float radians) 
  {
    _Reset();
    float sine = sin(radians);
    float cosine = cos(radians);
    
    _matrix[0][0] = cosine;
    _matrix[0][1] = -sine;
    _matrix[1][0] = sine;
    _matrix[1][1] = cosine;
  }
  
/*****************************************************************************/
/*!
    \brief
      Non-default constructor. Sets the matrix to be scale.
      
    \param x
      The x scale
      
    \param y
      The y scale
*/
/*****************************************************************************/
  Mat3D::Mat3D(float x, float y)
  {
    _Reset();
    _matrix[0][0] = x;
    _matrix[1][1] = y;
  }
  
/*****************************************************************************/
/*!
    \brief
      Non-default constructor. Outright constructs the matrix from the 
      passed in vectors.
      
    \param r
      The right vector (x vector)
      
    \param u
      The up vector (y vector)
      
    \param p
      The point/translation vector (point)
*/
/*****************************************************************************/
  Mat3D::Mat3D(const Vec3D& r, const Vec3D& u, const Vec3D& p)
  {
    _matrix[0][0] = r.x;
    _matrix[1][0] = r.y;
    _matrix[2][0] = r.z;
    
    _matrix[0][1] = u.x;
    _matrix[1][1] = u.y;
    _matrix[2][1] = u.z;
    
    _matrix[0][2] = p.x;
    _matrix[1][2] = p.y;
    _matrix[2][2] = p.z;
  }
  
/*****************************************************************************/
/*!
    \brief
      Sets the matrix to be a translation matrix.
      
    \param trans
      The vector to translate by
*/
/*****************************************************************************/
  void Mat3D::SetTranslation(const Vec2D& trans)
  {
    _Reset();
    _matrix[0][2] = trans.x;
    _matrix[1][2] = trans.y;
  }
  
/*****************************************************************************/
/*!
    \brief
      Sets the matrix to be a translation matrix.
      
    \param x
      The x to translate to
      
    \param y
      The y to translate to
      
    \param z
      The z to translate to, defaults to 1.0
*/
/*****************************************************************************/
  void Mat3D::SetTranslation(float x, float y, float z)
  {
    _Reset();
    _matrix[0][2] = x;
    _matrix[1][2] = y;
    _matrix[2][2] = z;
  }
  
/*****************************************************************************/
/*!
    \brief
      Sets the matrix to be a rotation matrix. In degrees.
      
    \param degrees
      The angle in degrees
*/
/*****************************************************************************/
  void Mat3D::SetRotationDegrees(float degrees)
  {
    float sine = sinf((float)(degrees * PI) / 180.0f);
    float cosine = cosf((float)(degrees * PI) / 180.0f);
    
    _Reset();
    
    _matrix[0][0] = cosine;
    _matrix[0][1] = -sine;
    _matrix[1][0] = sine;
    _matrix[1][1] = cosine;
  }
  
/*****************************************************************************/
/*!
    \brief
      Sets the matrix to be a rotation matrix. In radians.
      
    \param radians
      The angle in radians
*/
/*****************************************************************************/
  void Mat3D::SetRotationRadians(float radians)
  {
    float sine = sin(radians);
    float cosine = cos(radians);
    
    _Reset();
    
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
      The vector to scale by
*/
/*****************************************************************************/
  void Mat3D::SetScale(const Vec2D& scale)
  {
    _Reset();
    
    _matrix[0][0] = scale.x;
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
  void Mat3D::SetScale(float x, float y)
  {
    _Reset();
    
    _matrix[0][0] = x;
    _matrix[1][1] = y;
  }
  
/*****************************************************************************/
/*!
    \brief
      Makes a transpose of the passed in matrix.
*/
/*****************************************************************************/
  Mat3D Mat3D::Transpose()
  {
    return Mat3D(Vec3D(_matrix[0][0], _matrix[0][1], _matrix[0][2]),
                 Vec3D(_matrix[1][0], _matrix[1][1], _matrix[1][2]),
                 Vec3D(_matrix[2][0], _matrix[2][1], _matrix[2][2]));
  }
  
/*****************************************************************************/
/*!
    \brief
      Inverts the current matrix.
*/
/*****************************************************************************/
  void Mat3D::Inverse()
  {
    float a = _matrix[0][0];
    float b = _matrix[0][1];
    float c = _matrix[1][0];
    float d = _matrix[1][1];
    
    // get the determinant, then invert it
    float inverseDet = 1.0f / (( a * d ) - ( b * c ));
    
    Vec3D x(d, -c);        //  | D -B |
    Vec3D y(-b, a);        //  | -C A |
    Vec3D p(-_matrix[0][2], -_matrix[1][2], 1.0f);
    Vec3D zero(0.0f,0.0f,1.0f);
    
    x = x * inverseDet;     //  1 | D -B |    | A B | ^-1
    y = y * inverseDet;     // det| -C A | =  | C D |
    
    //Vector negTrans(-A.matrix[0][2], -A.matrix[1][2]);
    
    Mat3D linear(x,y,zero);
    Mat3D translate(p);
    
    // A^-1 = ([Translation]*[Linear])^ 1 = [Linear]^-1 * [Translation]^-1
    
    linear = linear * translate;
    
    *this = linear;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for matrix multiplication.
      
    \param rhs
      The matrix on the right hand side
*/
/*****************************************************************************/
  Mat3D Mat3D::operator*(const Mat3D& rhs) const
  {
    int i,j;
    Mat3D C;
    
    // every pass, complete one row of the result matrix.
    for(i = 0; i < 3; ++i)
    {
      j = 0;
                   // row stays same  | column changes after each calculation
      C._matrix[i][j] =   (_matrix[i][0] * rhs._matrix[0][j]) + 
                          (_matrix[i][1] * rhs._matrix[1][j]) + 
                          (_matrix[i][2] * rhs._matrix[2][j]);
                          
      ++j;

      C._matrix[i][j] =   (_matrix[i][0] * rhs._matrix[0][j]) + 
                          (_matrix[i][1] * rhs._matrix[1][j]) + 
                          (_matrix[i][2] * rhs._matrix[2][j]);
                          
      ++j;

      C._matrix[i][j] =   (_matrix[i][0] * rhs._matrix[0][j]) + 
                          (_matrix[i][1] * rhs._matrix[1][j]) + 
                          (_matrix[i][2] * rhs._matrix[2][j]);

      
    }
    
    return C;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for matrix to vector multiplication.
      
    \param rhs
      The vector on the right hand side
*/
/*****************************************************************************/
  Vec3D Mat3D::operator*(const Vec3D& v) const
  {
    float X = _matrix[0][0] * v.x +
              _matrix[0][1] * v.y +
              _matrix[0][2] * v.z;

    float Y = _matrix[1][0] * v.x +
              _matrix[1][1] * v.y +
              _matrix[1][2] * v.z;

    float Z = _matrix[2][0] * v.x +
              _matrix[2][1] * v.y +
              _matrix[2][2] * v.z;

    Vec3D result(X, Y, Z);

    return result;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for matrix multiplication.
      
    \param rhs
      The matrix on the right hand side ( it's vec2, assumes vector not point)
*/
/*****************************************************************************/
  Vec3D Mat3D::operator*(const Vec2D& v) const // assuming vector, not point
  {
    float X = _matrix[0][0] * v.x +
              _matrix[0][1] * v.y;

    float Y = _matrix[1][0] * v.x +
              _matrix[1][1] * v.y;

    float Z = _matrix[2][0] * v.x +
              _matrix[2][1] * v.y;

    Vec3D result(X, Y, Z);

    return result;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for matrix multiplication.
*/
/*****************************************************************************/
  Mat3D& Mat3D::operator*=(const Mat3D& rhs)
  {
    *this = *this * rhs;
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for matrix assignment.
*/
/*****************************************************************************/
  Mat3D& Mat3D::operator=(const Mat3D& rhs)
  {
    for(int i = 0; i < 3; ++i)
    {
      for(int j = 0; j < 3; ++j)
        _matrix[i][j] = rhs._matrix[i][j];
    }
    return *this;
  }

/*****************************************************************************/
/*!
    \brief
      Resets the matrix to the identity matrix.
*/
/*****************************************************************************/
  void Mat3D::_Reset()
  {
    for(int i = 0; i < 3; ++i)
    {
      for(int j = 0; j < 3; ++j)
      {
        if(i == j)
          _matrix[i][j] = 1.0f;
        else
          _matrix[i][j] = 0.0f;
      }
    }
  }
  
} // end name space
