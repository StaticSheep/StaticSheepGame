/*****************************************************************
Filename: CTransform.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "Vec3.h"

namespace Framework
{



  class Transform : public GameComponent
  {
  public:
    Transform();
    ~Transform();

    Vec3 GetTranslation(bool world = true) const;
    Vec3 GetScale(bool world = true) const;
    float GetRotation(bool world = true) const;

    void SetTranslation(Vec3 newTr);
    void SetScale(Vec3 newSc);
    void SetRotation(float rot);

    void SetPhysicsBody(unsigned body);

    bool operator==(const Transform& rhs);
    Transform& operator=(const Transform& rhs);

    static void Serialize(File& file, Variable var);
    static void Deserialize(File& file, Variable var);
    static void ToTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label);

    float test;


    void TweakTranslationSet(const void* value);
    void TweakRotationSet(const void* value);
    void TweakScaleSet(const void* value);

    void TweakTranslationGet(void* value);
    void TweakRotationGet(void* value);
    void TweakScaleGet(void* value);

    static void RegisterMembers()
    {
      TYPE_ADD_MEMBER( Transform, m_translation );
      TYPE_ADD_MEMBER( Transform, m_scale );
      TYPE_ADD_MEMBER( Transform, m_rotation );
    }

  private:

    bool m_hasPhysics;
    unsigned m_pBody;

    Vec3 m_translation;
    Vec3 m_scale;
    float m_rotation;
  };
}