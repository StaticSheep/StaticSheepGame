/*****************************************************************
Filename: CTransform.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CTransform.h"
#include "components/rigidbody/CRigidBody.h"
#include "systems/physics/SheepPhysics.h"

#include "systems/anttweak/AntTweakModule.h"
#include "systems/anttweak/TweakHelper.h"


namespace Framework
{
  Transform::Transform()
    :m_translation(0, 0, 0), m_rotation(0), m_scale(1, 1), m_hasPhysics(false), m_pBody(Handle::null), m_useDirtyHook(false)
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

  bool Transform::operator==(const Transform& rhs)
  {
    return false;
  }


  // ==================== SETTERS ============================

  void Transform::SetTranslation(Vec3 newTr)
  {

    if (m_hasPhysics)
    {
      reinterpret_cast<SheepFizz::PhysicsSpace*>(space->m_pSpace)->SetBodyPos(m_pBody, newTr);
    }
    else
    {
      m_translation = newTr;
    }
    if (m_useDirtyHook)
      this->GetOwner()->hooks.Call("TransformDirty");
  }

  void Transform::SetScale(Vec3 newSc)
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

    if (m_useDirtyHook)
      this->GetOwner()->hooks.Call("TransformDirty");
  }

  void Transform::SetPhysicsBody(unsigned body)
  {
    if (body == Handle::null)
    {
      m_pBody = Handle::null;
      m_hasPhysics = false;
    }
    else
    {
      m_pBody = body;
      m_hasPhysics = true;
    }
    
  }


  // ==================== GETTERS ============================


  Vec3 Transform::GetTranslation(bool world) const
  {
    if (m_hasPhysics)
    {
      return (reinterpret_cast<SheepFizz::PhysicsSpace*>(space->m_pSpace)->GetBodyPos(m_pBody));
    }
    else
      return m_translation;
  }

  Vec3 Transform::GetScale(bool world) const
  {
    if (m_hasPhysics)
    {
      return m_scale;
    }
    else
      return m_scale;
  }

  float Transform::GetRotation(bool world) const
  {
    if (m_hasPhysics)
    {
      return reinterpret_cast<SheepFizz::PhysicsSpace*>(space->m_pSpace)->GetBodyRot(m_pBody);
    }
    else
      return m_rotation;
  }



  void Transform::Serialize(File& file, Variable var)
  {
    Transform* tr = var.GetValue<Transform*>();

    // Cache our local Translation, Rotation, and Scale inside for the component itself
    tr->m_translation = tr->GetTranslation(false);
    tr->m_rotation = tr->GetRotation(false);
    tr->m_scale = tr->GetScale(false);

    // Get a pointer to the serializers
    Serializer* s = Serializer::Get();
    // Store a pointer to the typeinfo for later use
    const TypeInfo* info = var.GetTypeInfo();

    // Variable for members (translation and stuff)
    Variable mem;
    
    // Write to the file the name of the type
    file.Write("%s\n", info->Name());

    // Increase padding
    s->Padding(file, s->GetPadLevel()++);

    // Make a bracket and newline
    file.Write("{\n");

    // Translation
    s->Padding( file, s->GetPadLevel() );
    file.Write("%s ", "Translation");
    mem = Variable(tr->m_translation);
    mem.Serialize(file);

    // Rotation
    s->Padding( file, s->GetPadLevel() );
    file.Write("%s ", "Rotation");
    mem = Variable(tr->m_rotation);
    mem.Serialize(file);

    // Scale
    s->Padding( file, s->GetPadLevel() );
    file.Write("%s ", "Scale");
    mem = Variable(tr->m_scale);
    mem.Serialize(file);


    // Decrease padding level
    s->Padding(file, --s->GetPadLevel());
    // Write a bracket
    file.Write("}\n");
  }

  void Transform::Deserialize(File& file, Variable var)
  {
    Transform* tr = var.GetValue<Transform*>();
    Serializer* s = Serializer::Get();

    // Our peek function was nice enough to figure out the starting level for us
    int startLevel = s->GetPadLevel();

    std::string line = file.GetLine('{');
    char buf[32];
    
    // Read past Translation
    file.Read("%s", &buf);

    Vec3 trans;
    GET_TYPE(Vec3)->Deserialize(file, trans);

    // Read past Rotation
    file.Read("%s", &buf);

    float rot;
    GET_TYPE(float)->Deserialize(file, rot);

    // Read past Scale
    file.Read("%s", &buf);

    Vec3 scale;
    GET_TYPE(Vec3)->Deserialize(file, scale);

    tr->m_translation = trans;
    tr->m_rotation = rot;
    tr->m_scale = scale;

    file.GetLine('\n');
    file.GetLine('\n');
  }

  void Transform::TweakTranslationSet(const void* value)
  {
    SetTranslation(*(Vec3*)value);
  }

  void Transform::TweakTranslationGet(void* value)
  {
    *(Vec3*)value = GetTranslation(false);
  }

  void Transform::TweakScaleSet(const void* value)
  {
    SetScale(*(Vec3*)value);
  }

  void Transform::TweakScaleGet(void* value)
  {
    *(Vec3*)value = GetScale(false);
  }

  void Transform::TweakRotationSet(const void* value)
  {
    SetRotation(*(float*)value);
  }

  void Transform::TweakRotationGet(void* value)
  {
    *(float*)value = GetRotation(false);
  }

  void Transform::ToTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label)
  {
    Transform* tr = var.GetValue<Transform*>();

    bar->DefineGroup("Transform");
    bar->AddGenericVarCB("Translation", AntTweak::TW_TYPE_DIR3F, nullptr, 0, tr, BUILD_FUNCTION(Transform::TweakTranslationSet), BUILD_FUNCTION(Transform::TweakTranslationGet));

    bar->DefineGroup("Transform");
    bar->AddGenericVarCB("Scale", AntTweak::TW_TYPE_DIR3F, nullptr, 0, tr, BUILD_FUNCTION(Transform::TweakScaleSet), BUILD_FUNCTION(Transform::TweakScaleGet));

    bar->DefineGroup("Transform");
    bar->AddGenericVarCB("Rotation", AntTweak::TW_TYPE_FLOAT, nullptr, 0, tr, BUILD_FUNCTION(Transform::TweakRotationSet), BUILD_FUNCTION(Transform::TweakRotationGet));
  }

}