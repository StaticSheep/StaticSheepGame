/*****************************************************************
Filename: CSaucer.cpp
Project:  Gam250
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CSaucer.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../systems/input/Input.h"
#include "SheepMath.h"

namespace Framework
{
  Saucer::Saucer() : m_crosshairColor(1, 1, 1, 1), m_chargeTime(100), m_beamColor(1, 1, 1, 1), m_isFiring(false)
  {

  }

  Saucer::~Saucer()
  {

  }

  void Saucer::LogicUpdate(float dt)
  {
    GamePad* gp = space->GetHandles().GetAs<GamePad>(m_controller);
    Transform* trans = space->GetHandles().GetAs<Transform>(m_sTransform);
    ParticleSystem* system = space->GetHandles().GetAs<ParticleSystem>(m_particleSystem);
    ParticleBeam* emitter = space->GetHandles().GetAs<ParticleBeam>(m_emitter);
    AOEDamage* aoe = space->GetHandles().GetAs<AOEDamage>(m_AOE);

    Vec3 newTrans = trans->GetTranslation();

    if (!gp->LStick_InDeadZone())
    {
     newTrans += (Vec3(gp->LeftStick_X(), gp->LeftStick_Y(), 0) * 20);
    }

    if (gp->RightTrigger() == 0 && m_isFiring || m_chargeTime <= 0)
    {
      m_isFiring = false;
      emitter->spawning = false;
      aoe->m_damagePerSecond = 0;
      system->DestroyParticles();
    }

    if (gp->RightTrigger() && !m_isFiring && m_chargeTime > 0)
    {
      emitter->spawning = true;    
      emitter->time = 0;
      aoe->m_damagePerSecond = 500;
      m_isFiring = true;
    }

    if (m_crosshairColor.g < 1)
      m_crosshairColor.g += dt / 2;

    if (m_crosshairColor.b < 1)
      m_crosshairColor.b += dt / 2;

    newTrans.x = Clamp(newTrans.x, -Draw::ScreenWidth() / 2, Draw::ScreenWidth() / 2);
    newTrans.y = Clamp(newTrans.y, -Draw::ScreenHeight() / 2, Draw::ScreenHeight() / 2);
    trans->SetTranslation(newTrans);

    if (m_isFiring)
      m_chargeTime -= dt;
  }

  void Saucer::Draw()
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(m_sTransform);

    GRAPHICS->SetPosition(trans->GetTranslation().X,
      trans->GetTranslation().Y, 0);

    GRAPHICS->SetRotation(trans->GetRotation());

    GRAPHICS->SetSize(.5,
      .5);

    GRAPHICS->SetColor(m_crosshairColor);

    GRAPHICS->SetObjectOrigin(0, 0);

    GRAPHICS->SetUV(Vec2(0,0), Vec2(1,1));

    GRAPHICS->SetSpriteFlip(false, false);

    GRAPHICS->SetCamState(0);

    GRAPHICS->DrawBatched(m_crosshairTex);
  }

  void Saucer::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Saucer::LogicUpdate));
    space->hooks.Add("Draw", self, BUILD_FUNCTION(Saucer::Draw));

    m_sTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);

    m_controller = space->GetGameObject(owner)->GetComponentHandle(eGamePad);

    m_emitter = space->GetGameObject(owner)->GetComponentHandle(eParticleBeam);

    m_particleSystem = space->GetGameObject(owner)->GetComponentHandle(eParticleSystem);

    m_AOE = space->GetGameObject(owner)->GetComponentHandle(eAOEDamage);

    m_crosshairTex = GRAPHICS->LoadTexture(std::string("target_clear.png"));
    m_TexDim = GRAPHICS->GetTextureDim(m_crosshairTex);

    InitBeam();

    SetPlayerID(0);
  }


  void Saucer::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("Draw", self);
  }

  void Saucer::InitBeam()
  {
    ParticleSystem* system = space->GetHandles().GetAs<ParticleSystem>(m_particleSystem);
    ParticleBeam* emitter = space->GetHandles().GetAs<ParticleBeam>(m_emitter);
    Transform* trans = space->GetHandles().GetAs<Transform>(m_sTransform);

    emitter->m_rate.m_startMin = emitter->m_rate.m_startMax = 1;
    emitter->spawning = false;
    system->m_useZ = true;
    
    system->direction.m_startMin = Vec3(0, 0, -1.0f);
    system->direction.m_startMax = Vec3(0, 0, -1.0f);
    system->direction.m_endMin = Vec3(0, 0, -1.0f);
    system->direction.m_endMax = Vec3(0, 0, -1.0f);

    system->scale.m_startMin = system->scale.m_startMax = 100;
    system->scale.m_endMin = system->scale.m_endMax = 100;

    system->particleLife.m_startMin = system->particleLife.m_startMin = 1;
    system->speed.m_endMin = system->speed.m_endMax = system->speed.m_startMin = system->speed.m_startMax = 0;
    UpdateBeamColor();
  }

  void Saucer::UpdateBeamColor()
  {
    ParticleSystem* system = space->GetHandles().GetAs<ParticleSystem>(m_particleSystem);

    system->color.m_startMin = system->color.m_startMax = system->color.m_endMin = system->color.m_endMax = m_beamColor;
  }

  void Saucer::SetPlayerID(const unsigned& playerID)
  {
    if (playerID >= 0 && playerID <= 3)
      m_playerID = playerID;

    GamePad* gp = space->GetHandles().GetAs<GamePad>(m_controller);

    gp->SetPad(m_playerID);
  }
}