/*****************************************************************
Filename: CStarController.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content � 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../../sound/CSoundEmitter.h"
#include "../../transform/CTransform.h"

namespace Framework
{
  class StarController : public GameComponent
  {
  public:
    StarController();
    ~StarController();
    void Initialize();
    void LogicUpdate(float dt);
    void Remove();

    void GivePlayerStar(int player, int stars);
    void DrawStars();

    Handle LevelLogic;
    int playerStars[4];
  };
}