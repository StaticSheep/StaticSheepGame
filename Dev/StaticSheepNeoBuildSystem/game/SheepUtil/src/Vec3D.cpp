/*****************************************************************************/
/*!
\file   Vec3D.cpp
\author Zakary Wilson
\par    Course: GAM200
\par    Giga Gravity Games
\date   10/20/2014<BR>
\brief  
    This file contains functions to for 2D homogenous vector math.<BR>

\par    All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/*****************************************************************************/

#include "src/precompiled.h"
#include "Vec3D.h"
#include "SheepMath.h"

namespace Framework
{

  float Vec3D::Length()
  {
    return sqrtf(x * x + y * y + z * z);
  }

  float Vec3D::SquareLength()
  {
    return (x * x + y * y + z * z);
  }

  Vec3D Vec3D::CalculateNormal()
  {
    Vec3D normal(*this);
    normal.Normalize();

    return normal;
  }

/*****************************************************************************/
/*!
    \brief
      Normalizes the vector and returns a reference to it. 2D homogenous
      coordinate system. Doesn't do anything with z.
*/
/*****************************************************************************/
  Vec3D& Vec3D::Normalize()
  {
    float length = Length();
    
    x /= length;
    y /= length;
    z /= length;
    
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Rotates the vector by radians. 2D homogenous coordinate system. Doesn't
      change z.
*/
/*****************************************************************************/
  Vec3D& Vec3D::Rotate(float radians)
  {
    x = x * cos(radians) - y * sin(radians);
    y = x * sin(radians) + y * cos(radians);
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Computes the dotproduct between two vectors.
*/
/*****************************************************************************/
  float Vec3D::DotProduct(const Vec3D& rhs)
  {
    return (rhs.x * x + rhs.y * y + rhs.z * z);
  }
  
/*****************************************************************************/
/*!
    \brief
      Computes the cross product between two vectors.
*/
/*****************************************************************************/
  Vec3D Vec3D::CrossProduct(Vec3D& rhs)
  {
    return Vec3D(y * rhs.z - z * rhs.y, 
                 z * rhs.x - x * rhs.z, 
                 x * rhs.y - y * rhs.x);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for adding two 3D vectors.
*/
/*****************************************************************************/
  Vec3D Vec3D::operator+(const Vec3D& rhs) const
  {
    return Vec3D(rhs.x + x, rhs.y + y, rhs.z + z);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for adding two 3D vectors.
*/
/*****************************************************************************/
  Vec3D& Vec3D::operator+=(const Vec3D& rhs)
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for subtracting two 3D vectors.
*/
/*****************************************************************************/
  Vec3D Vec3D::operator-(const Vec3D& rhs) const
  {
    return Vec3D(x - rhs.x, y - rhs.y, z - rhs.z);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for subtracting two 3D vectors.
*/
/*****************************************************************************/
  Vec3D& Vec3D::operator-=(const Vec3D& rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for negating 3D vectors.
*/
/*****************************************************************************/
  Vec3D Vec3D::operator-() const
  {
    return Vec3D(-x,-y,-z);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for the dotproduct between two 3D vectors.
*/
/*****************************************************************************/
  float Vec3D::operator*(const Vec3D& rhs) const
  {
    return (rhs.x * x + rhs.y * y + rhs.z * z);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for scaling a 3D vector.
*/
/*****************************************************************************/
  Vec3D Vec3D::operator/(float scalar) const
  {
    return Vec3D(x / scalar, y / scalar, z / scalar);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for scaling a 3D vector.
*/
/*****************************************************************************/
  Vec3D& Vec3D::operator/=(float scalar)
  {
    x /= scalar;
    y /= scalar;
    z /= scalar;

    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for scaling a 3D vector.
*/
/*****************************************************************************/
  Vec3D Vec3D::operator*(float scalar) const
  {
    return Vec3D(x * scalar, y * scalar, z * scalar);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for scaling a 3D vector.
*/
/*****************************************************************************/
  Vec3D& Vec3D::operator*=(float scalar)
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;

    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for the cross product.
*/
/*****************************************************************************/
  Vec3D Vec3D::operator^(const Vec3D& rhs) const
  {
    return Vec3D(y * rhs.z - z * rhs.y, 
                 z * rhs.x - x * rhs.z, 
                 x * rhs.y - y * rhs.x);
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload for assignment.
*/
/*****************************************************************************/
  Vec3D& Vec3D::operator=(const Vec3D& rhs)
  {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
  }
  
/*****************************************************************************/
/*!
    \brief
      Operator overload multiplying floats with a 3D vector.
*/
/*****************************************************************************/
  Vec3D operator*(float scalar, const Vec3D& rhs)
  {
    return Vec3D(rhs.x * scalar, rhs.y * scalar, rhs.z);
  }

}