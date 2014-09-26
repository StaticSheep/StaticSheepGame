/*****************************************************************
Filename: CTransform.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "CRigidBody.h"
#include "SheepPhysics.h"

namespace Framework
{
  Transform::Transform()
    :m_translation(0, 0, 0), m_rotation(0), m_scale(1,1), m_hasPhysics(false), m_pBody(Handle::null)
  {
  }

  Transform::~Transform()
  {
  }

  void Transform::Initialize()
  {

  }

  // ==================== SETTERS ============================

  void Transform::SetTranslation(Vec3D newTr)
  {
    if (m_hasPhysics)
    {
      
    }
    else
    {
      m_translation = newTr;
    }
  }

  void Transform::SetScale(Vec2 newSc)
  {
    if (m_hasPhysics)
    {

    }
    else
      m_scale = newSc;
  }

  void Transform::SetRotation(float rot)
  {
    if (m_hasPhysics)
    {

    }
    else
      m_rotation = rot;
  }


  // ==================== GETTERS ============================


  Vec3D Transform::GetTranslation()
  {
    if (m_hasPhysics)
    {

    }
    else
      return m_translation;
  }

  Vec2 Transform::GetScale()
  {
    if (m_hasPhysics)
    {

    }
    else
      return m_scale;
  }

  float Transform::GetRotation()
  {
    if (m_hasPhysics)
    {

    }
    else
      return m_rotation;
  }

}