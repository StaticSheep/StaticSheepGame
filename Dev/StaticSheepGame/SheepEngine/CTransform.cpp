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

  Transform& Transform::operator=(const Transform& rhs)
  {
    m_translation = rhs.GetTranslation();
    m_rotation = rhs.GetRotation();
    m_scale = rhs.GetScale();

    return *this;
  }


  // ==================== SETTERS ============================

  void Transform::SetTranslation(Vec3D newTr)
  {
    if (m_hasPhysics)
    {
      reinterpret_cast<SheepFizz::PhysicsSpace*>(space->m_pSpace)->SetBodyPos(m_pBody, newTr);
    }
    else
    {
      m_translation = newTr;
    }
  }

  void Transform::SetScale(Vec2 newSc)
  {
    m_scale = newSc;

    if (m_hasPhysics)
    {

    }
  }

  void Transform::SetRotation(float rot)
  {
    if (m_hasPhysics)
    {
      reinterpret_cast<SheepFizz::PhysicsSpace*>(space->m_pSpace)->SetBodyRot(m_pBody, rot);
    }
    else
      m_rotation = rot;
  }

  void Transform::SetPhysicsBody(unsigned body)
  {
    m_pBody = body;
    m_hasPhysics = true;
  }


  // ==================== GETTERS ============================


  Vec3D Transform::GetTranslation() const
  {
    if (m_hasPhysics)
    {
      return reinterpret_cast<SheepFizz::PhysicsSpace*>(space->m_pSpace)->GetBodyPos(m_pBody);
    }
    else
      return m_translation;
  }

  Vec2 Transform::GetScale() const
  {
    if (m_hasPhysics)
    {
      return m_scale;
    }
    else
      return m_scale;
  }

  float Transform::GetRotation() const
  {
    if (m_hasPhysics)
    {
      return reinterpret_cast<SheepFizz::PhysicsSpace*>(space->m_pSpace)->GetBodyRot(m_pBody);
    }
    else
      return m_rotation;
  }

}