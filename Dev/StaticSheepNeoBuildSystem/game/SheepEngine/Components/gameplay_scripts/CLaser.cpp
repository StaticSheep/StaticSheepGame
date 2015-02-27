/*****************************************************************
Filename: CLaser.cpp
Project:  Giga Gravity Games
Author(s): Jon Sourbeer (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CLaser.h"
#include "systems/graphics/SheepGraphics.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CCircleCollider.h"

namespace Framework
{
  Laser::Laser() : 
    startDelay(0), duration(1), 
    damage(1), width(1), 
    arcRotation(0), arcDelay(0), 
    arcPerSec(0),
    m_bodyScale(1, 1), m_bodyColor(1,1,1,1),
    m_beamColor(1,1,1,1)
  {
    
  }

  Laser::~Laser()
  {

  }

  void Laser::Initialize()
  {

    if (m_bodyTexName.length() == 0)
      m_bodyTexName = "Default.png";

    SetBodyTexture(m_bodyTexName.c_str());

    if (m_beamTexName.length() == 0)
      m_beamTexName = "Default.png";

    SetBeamTexture(m_beamTexName.c_str());


    //TODO not sure if we have a GetOwner()->has working
    space->hooks.Add("Draw", self, BUILD_FUNCTION(Laser::DrawLaser));

    //logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Laser::LogicUpdate));

    lTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    lCollider = space->GetGameObject(owner)->GetComponentHandle(eCircleCollider);

    //lEmitter = FACTORY->

    CircleCollider *lc = space->GetHandles().GetAs <CircleCollider>(lCollider);
    Vec3D direction = lc->GetBodyRotationAsVector();

    if (duration > arcDelay)
      arcPerSec = arcRotation / (duration - arcDelay);
    else
      arcPerSec = 0;

    if (type)
      Caster = &Framework::Laser::ComplexCaster;
    else
      Caster = &Framework::Laser::SimpleCaster;

    if (width > 4)
    {
      Vec3D offsetDir = direction.CalculateNormal();
      numberOfRays = width / 4;

      for (int i = 1; i < numberOfRays + 1; ++i)
      {
        positionOffsets.push_back((offsetDir * 2 * i) + lc->GetBodyPosition());
        positionOffsets.push_back((-offsetDir * 2 * i) + lc->GetBodyPosition());
      }

      if (width % 4 != 0)
      {
        float valueOffset = ((float)width) / 2.0f;
        positionOffsets.push_back(offsetDir * valueOffset + lc->GetBodyPosition());
        positionOffsets.push_back(-offsetDir * valueOffset + lc->GetBodyPosition());
      }
    }
  }

  void Laser::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("Draw", self);
  }

  void Laser::LogicUpdate(float dt)
  {
    CircleCollider *lc = space->GetHandles().GetAs <CircleCollider>(lCollider);

    if (startDelay > 0)
    {
      startDelay -= dt;
      return;
    }

      //duration begins after start delay over
    duration -= dt;

    if (duration < 0)
      space->GetGameObject(owner)->Destroy();

    arcDelay -= dt;

    if (arcDelay <= 0)
      (this->*Caster)(lc);

    float curRotation = lc->GetBodyRotation();
    curRotation += arcPerSec;
    lc->SetBodyRotation(curRotation);
  }

  void Laser::DrawLaser()
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(lTransfrom);

    GRAPHICS->SetUV(Vec2(0,0), Vec2(1,1));

    GRAPHICS->SetPosition(trans->GetTranslation().X,
      trans->GetTranslation().Y, trans->GetTranslation().Z);

    GRAPHICS->SetRotation(trans->GetRotation());

    GRAPHICS->SetSize(trans->GetScale().X * m_bodyScale.x,
      trans->GetScale().Y * m_bodyScale.Y);

    GRAPHICS->SetColor(m_bodyColor);

    GRAPHICS->SetCamState(0);

    GRAPHICS->DrawBatched(m_bodyTex);

    GRAPHICS->SetSize((trans->GetScale().X * 500) / m_beamTexDim.x,
      (trans->GetScale().Y * width) / m_beamTexDim.y);

    GRAPHICS->SetColor(m_beamColor);

    GRAPHICS->SetObjectOrigin(((m_bodyTexDim.x * m_bodyScale.x * trans->GetScale().X) / 2.0f) + ((trans->GetScale().X * 500) / 2.0f), 0);

    GRAPHICS->DrawBatched(m_beamTex);

    GRAPHICS->SetObjectOrigin(0, 0);
  }

  void Laser::SimpleCaster(CircleCollider *lc)
  {
    Vec3D direction = lc->GetBodyRotationAsVector();
    lc->SetRayCast(lc->GetBodyPosition(), direction, "Collide");
    lc->SimpleRayCast();
    Vec3D offset;

    for (int i = 0; i < positionOffsets.size(); ++i)
    {
      lc->SetRayCast(positionOffsets[i], direction, "Collide");
      lc->SimpleRayCast();

      //check return results
    }
  }

  void Laser::ComplexCaster(CircleCollider *lc)
  {
    Vec3D direction = lc->GetBodyRotationAsVector();
    lc->SetRayCast(lc->GetBodyPosition(), direction, "Collide");
    lc->ComplexRayCast();
    Vec3D offset;

    for (int i = 0; i < positionOffsets.size(); ++i)
    {
      lc->SetRayCast(positionOffsets[i], direction, "Collide");
      lc->ComplexRayCast();
    }
  }

  void Laser::SetBodyTexture(const char * Texture)
  {
    m_bodyTexName = Texture;
    m_bodyTex = GRAPHICS->LoadTexture(Texture);
    m_bodyTexDim = GRAPHICS->GetTextureDim(m_bodyTex);
  }

  DirectSheep::Handle& Laser::GetBodyTexture()
  {
    return m_bodyTex;
  }

  void Laser::SetBeamTexture(const char * Texture)
  {
    m_beamTexName = Texture;
    m_beamTex = GRAPHICS->LoadTexture(Texture);
    m_beamTexDim = GRAPHICS->GetTextureDim(m_beamTex);
  }

  DirectSheep::Handle& Laser::GetBeamTexture()
  {
    return m_beamTex;
  }

  void Laser::TweakSetBodyTexture(const void * Texture)
  {
    SetBodyTexture(((std::string *)Texture)->c_str());
  }

  void Laser::TweakSetBeamTexture(const void * Texture)
  {
    SetBeamTexture(((std::string *)Texture)->c_str());
  }
}