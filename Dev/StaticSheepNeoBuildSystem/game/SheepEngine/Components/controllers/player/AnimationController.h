#pragma once

#include "components/base/Component.h"
#include "../../sound/CSoundEmitter.h"
#include "../../transform/CTransform.h"
#include "types/weapons/WBase.h"

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

	class AnimationController
	{
	public:
    AnimationController();
    AnimationController(int playerNum);
    ~AnimationController();
		
		//member variables
    const char *spriteSheet;
    AnimFrames idle;
    AnimFrames run;
    AnimFrames jump;
    AnimFrames attack;
    AnimationState AnimState;
	};
}