/*****************************************************************
Filename: AnimationController.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "../../sound/CSoundEmitter.h"
#include "../../transform/CTransform.h"
#include "types/weapons/WBase.h"
#include "components/sprites/CSpineSprite.h"

namespace Framework
{
  struct AnimFrames
  {
    AnimFrames() {
      beginFrame = 0; endFrame = 0;
    };
    int beginFrame;
    int endFrame;
  };

  enum AnimationState {IDLE, RUN, JUMP, ATTACK};
  enum AimDirection {UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWN};

	class AnimationController
	{
	public:
    AnimationController();
    AnimationController(int playerNum);
    ~AnimationController();

    void Update(SpineSprite* spine, Vec4& color, float rotation, Vec3& aimDir, bool aiming);
		
		//member variables
    const char *spriteSheet;
    AnimFrames idle;
    AnimFrames run;
    AnimFrames jump;
    AnimFrames attack;
    AnimationState AnimState;
	};
}