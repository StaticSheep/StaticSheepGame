/*****************************************************************
Filename: CSaucer.h
Project:  Gam250
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "components/gamepad/CGamePad.h"
#include "components/particles/CParticleCircleEmitter.h"
#include"components/gameplay_scripts/FX_Scripts/CAOEDamage.h"
#include "components/particles/CParticleSystem.h"
#include "../../transform/CTransform.h"
#include "../systems/graphics/SheepGraphics.h"

namespace Framework
{
  class Saucer : public GameComponent
  {
  public:
    Saucer();
    ~Saucer();
    void LogicUpdate(float dt);
    void InitBeam();
    void Draw();
    void Initialize();
    void Remove();

    //member variables
    Handle m_sTransform;
    Handle m_controller;
    Handle m_emitter;
    Handle m_particleSystem;
    Handle m_AOE;

    DirectSheep::Handle m_crosshairTex;

    Vec2                m_TexDim;

    Vec4                m_crosshairColor;

    float                 m_chargeTime;

    Vec4                m_beamColor;

    bool                m_isFiring;

  private:
    void UpdateBeamColor();
  };
}