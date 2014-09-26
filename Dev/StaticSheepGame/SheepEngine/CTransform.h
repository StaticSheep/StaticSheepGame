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
    virtual void Initialize();

    Vec3D GetTranslation();
    Vec2 GetScale();
    float GetRotation();

    void SetTranslation(Vec3D newTr);
    void SetScale(Vec2 newSc);
    void SetRotation(float rot);

  private:

    bool m_hasPhysics;
    unsigned m_pBody;

    Vec3D m_translation;
    Vec2 m_scale;
    float m_rotation;
  };
}