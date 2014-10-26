
#include "src/precompiled.h"
#include "Matrix4D.h"
#include "SheepMath.h"

namespace Framework
{

/*****************************************************************************/
/*!
    \brief
      Default constructor for 4x4 matrix. Defaults to identity matrix.
*/
/*****************************************************************************/
  Mat4D::Mat4D()
  {
    _Reset();
  }
  
/*****************************************************************************/
/*!
    \brief
      Non-default constructor. Sets the matrix to a 3D translation.
      
    \param vec
      The vector to translate by
*/
/*****************************************************************************/
  Mat4D::Mat4D(const Vec3D& vec)
  {
    _Reset();
    _matrix[0][3] = vec.x;
    _matrix[1][3] = vec.y;
    _matrix[2][3] = vec.z;
  }
  
/*****************************************************************************/
/*!
    \brief
      Non-default constructor. Sets the matrix to a 3D translation.
      
    \param vec
      The vector to translate by. Shouldn't use, because 4th dimension stuff
*/
/*****************************************************************************/
  Mat4D::Mat4D(const Vec4D& vec)
  {
    _Reset();
    _matrix[0][3] = vec.x;
    _matrix[1][3] = vec.y;
    _matrix[2][3] = vec.z;
    _matrix[3][3] = vec.w; // ummmm....
  }
  
/*****************************************************************************/
/*!
    \brief
      Non-default constructor. Outright constructs it from the vectors passed
      in.
      
    \param r
      The roll vector (x vector)
      
    \param p
      The pitch vector (y vector)
      
    \param y
      The yaw vector (z vector)
      
    \param vec
      The position (w vector)
*/
/*****************************************************************************/
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
  
/*****************************************************************************/
/*!
    \brief
      Takes two Vec4s and constructs a 4x4 matrix by multiplaying VectorA by 
      the transpose of VectorB.
    
    \param VectorA
      Pointer to the first Vec4.

    \param VectorB
      Pointer to the second Vec4..
*/
/*****************************************************************************/
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
  
/*****************************************************************************/
/*!
    \brief
      Non-default constructor. Creates a scale matrix.
    
    \param x
      The x scale

    \param y
      The y scale

    \param z
      The z scale
*/
/*****************************************************************************/
  Mat4D::Mat4D(float x, float y, float z)
  {
    _Reset();
    _matrix[0][0] = x;
    _matrix[1][1] = y;
    _matrix[2][2] = z;
  }
  
/*****************************************************************************/
/*!
    \brief
      Sets the matrix to be a translation matrix.
    
    \param vec
      The vector to translate to
*/
/*****************************************************************************/
  void Mat4D::SetTranslation(const Vec3D& vec)
  {
    _Reset();
    _matrix[0][3] = vec.x;
    _matrix[1][3] = vec.y;
    _matrix[2][3] = vec.z;
  }
  
/*****************************************************************************/
/*!
    \brief
      Sets the matrix to be a translation matrix.
    
    \param vec
      The vector to translate to... 4th dimension craziness...?
*/
/*****************************************************************************/
  void Mat4D::SetTranslation(const Vec4D& vec)
  {
    _Reset();
    _matrix[0][3] = vec.x;
    _matrix[1][3] = vec.y;
    _matrix[2][3] = vec.z;
    _matrix[3][3] = vec.w; // I don't know anymore...
  }
  
/*****************************************************************************/
/*!
    \brief
      Sets the matrix to be a translation matrix.
    
    \param x
      The x translation
      
    \param y
      The y translation
      
    \param z
      The z translation
      
    \param w
      The w translation, defaults to 1.0f
*/
/*****************************************************************************/
  void Mat4D::SetTranslation(float x, float y, float z, float w)
  {
    _Reset();
    _matrix[0][3] = x;
    _matrix[1][3] = y;
    _matrix[2][3] = z;
    _matrix[3][3] = w;
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
  void Mat4D::SetScale(const Vec2D& vec)
  {
    _Reset();
    _matrix[0][0] = vec.x;
    _matrix[1][1] = vec.y;
  }
  
/*****************************************************************************/
/*!
    \brief
      Sets the matrix to be a scale matrix.
    
    \param vec
      The vector to scale by
*/
/*****************************************************************************/
  void Mat4D::SetScale(const Vec3D& vec)
  {
    _Reset();
    _matrix[0][0] = vec.x;
    _matrix[1][1] = vec.y;
    _matrix[2][2] = vec.z;
  }
  
/*****************************************************************************/
/*!
    \brief
      Spits out a transpose of the current matrix.
*/
/*****************************************************************************/
  Mat4D Mat4D::Transpose()
  {
    return Mat4D
    (Vec4D(_matrix[0][0], _matrix[0][1], _matrix[0][2], _matrix[0][3]),
     Vec4D(_matrix[1][0], _matrix[1][1], _matrix[1][2], _matrix[1][3]),
     Vec4D(_matrix[2][0], _matrix[2][1], _matrix[2][2], _matrix[2][3]),
     Vec4D(_matrix[3][0], _matrix[3][1], _matrix[3][2], _matrix[3][3]));
  }
  
/*****************************************************************************/
/*!
    \brief
      Matrix multiplication for 4x4 matrices.
      
    \param rhs
      The matrix on the right
*/
/*****************************************************************************/
  Mat4D& Mat4D::operator*=(const Mat4D& rhs)
  {
    *this = *this * rhs;
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for assignment of 4x4 matrices.
*/
/*****************************************************************************/
  Mat4D& Mat4D::operator=(const Mat4D& rhs)
  {
    for(int i = 0; i < 4; ++i)
    {
      for(int j = 0; j < 4; ++j)
        _matrix[i][j] = rhs._matrix[i][j];
    }
    
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for matrix multiplication.
*/
/*****************************************************************************/
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
  
/*****************************************************************************/
/*!
    \brief
      Resets the matrix to the identity matrix.
*/
/*****************************************************************************/
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
