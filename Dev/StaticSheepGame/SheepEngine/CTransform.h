/*****************************************************************
Filename: CTransform.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{

  class Transform : public GameComponent
  {
  public:
    Transform();
    ~Transform();

    Vec3D GetTranslation(void) const;
    Vec2 GetScale(void) const;
    float GetRotation(void) const;

    void SetTranslation(Vec3D newTr);
    void SetScale(Vec2 newSc);
    void SetRotation(float rot);

    void SetPhysicsBody(unsigned body);

    Transform& operator=(const Transform& rhs);

    float test;

  private:

    bool m_hasPhysics;
    unsigned m_pBody;

    Vec3D m_translation;
    Vec2 m_scale;
    float m_rotation;
  };
}