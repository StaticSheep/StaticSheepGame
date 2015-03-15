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
    Handle sTransform;
    Handle controller;

    DirectSheep::Handle crosshairTex;

    Vec2                TexDim;

    bool                hasFired;

    Vec4                crosshairColor;
  };
}