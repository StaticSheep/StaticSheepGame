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
    void Draw();
    void Fire();
    void Initialize();
    void Remove();

    //member variables
    Handle m_sTransform;
    Handle m_controller;

    DirectSheep::Handle m_crosshairTex;

    Vec2                m_TexDim;

    bool                m_hasFired;

    Vec4                m_crosshairColor;

    int                 m_shotsLeft;
  };
}