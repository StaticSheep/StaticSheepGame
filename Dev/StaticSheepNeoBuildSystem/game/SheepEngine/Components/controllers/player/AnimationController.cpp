#include "pch/precompiled.h"
#include "types/space/Space.h"
#include "components/colliders/CBoxCollider.h"
#include "types/vectors/Vec3.h"
#include "AnimationController.h"

namespace Framework
{
  
  AnimationController::AnimationController() //1
	{
		//set defaults
    spriteSheet = "default.png";
	}

  AnimationController::AnimationController(int playerNum)
  {
    switch (playerNum)
    {
    case 0:
      spriteSheet = "ninja_sprite_sheet.png";
      idle.beginFrame = 0;
      idle.endFrame = 0;
      run.beginFrame = 12;
      run.endFrame = 23;
      jump.beginFrame = 24;
      jump.endFrame = 24;
      attack.beginFrame = 36;
      attack.endFrame = 36;
      break;
    case 1:
      spriteSheet = "ruiser_sprite_sheet.png";
      idle.beginFrame = 0;
      idle.endFrame = 0;
      run.beginFrame = 12;
      run.endFrame = 12;
      jump.beginFrame = 24;
      jump.endFrame = 24;
      attack.beginFrame = 36;
      attack.endFrame = 36;
      break;
    case 2:
      spriteSheet = "sw_sprite_sheet.png";
      idle.beginFrame = 0;
      idle.endFrame = 0;
      run.beginFrame = 12;
      run.endFrame = 12;
      jump.beginFrame = 24;
      jump.endFrame = 24;
      attack.beginFrame = 36;
      attack.endFrame = 36;
      break;
    case 3:
      spriteSheet = "ninja_sprite_sheet.png";
      idle.beginFrame = 0;
      idle.endFrame = 0;
      run.beginFrame = 12;
      run.endFrame = 12;
      jump.beginFrame = 24;
      jump.endFrame = 24;
      attack.beginFrame = 36;
      attack.endFrame = 36;
      break;
    }
  }

  AnimationController::~AnimationController() //4
	{

	}


}