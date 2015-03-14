/*****************************************************************
Filename: AnimationController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "types/space/Space.h"
#include "components/colliders/CBoxCollider.h"
#include "types/vectors/Vec3.h"
#include "../SheepUtil/include/Matrix3D.h"
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
      spriteSheet = "us_sprite_sheet.png";
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

  //enum AnimationState {IDLE, RUN, JUMP, ATTACK};
  void AnimationController::Update(SpineSprite* spine, Vec4& color, float rotation, Vec3& aimDir, bool aiming)
  {
    spine->SetColor(color);
    aimDir.Normalize();    

    Mat3D rot(-rotation);

    Vec3 temp = rot * aimDir;
    float theta = 0.0f;

    switch(AnimState)
    {
    case IDLE:
      spine->SetSequence(std::string("idle"));
      break;
    case RUN:
      if(!aiming)
      {
        spine->SetSequence(std::string("run"));
      }
      else
      {

        theta = atan2f(temp.y, temp.x) * 57.2957795;

        //ENGINE->TraceLog.Log(Framework::TraceLevel::DBG, "theta = %f", theta);

        if(theta > 0.0f)
        {

          if(theta < 90.0f)
            spine->FlipX(false);
          else
            spine->FlipX(true);

          if(theta < 22.5f)
          {
            spine->SetSequence(std::string("run_shoot_right"));
          }
          else
          if(theta < 67.5f)
          {
            spine->SetSequence(std::string("run_shoot_upright"));
          }
          else
          if(theta < 112.5f)
          {
            spine->SetSequence(std::string("run_shoot_up"));
          }
          else
          if(theta < 157.5f)
          {
            spine->SetSequence(std::string("run_shoot_upright"));
          }
          else
          {
            spine->SetSequence(std::string("run_shoot_right"));
          }
        }
        else
        {
          if(theta > -90.0f)
            spine->FlipX(false);
          else
            spine->FlipX(true);

          if(theta > -22.5f)
          {
            spine->SetSequence(std::string("run_shoot_right"));
          }
          else
          if(theta > -67.5f)
          {
            spine->SetSequence(std::string("run_shoot_downright"));
          }
          else
          if(theta > -112.5f)
          {
            spine->SetSequence(std::string("run_shoot_down"));
          }
          else
          if(theta > -157.5f)
          {
            spine->SetSequence(std::string("run_shoot_downright"));
          }
          else
          {
            spine->SetSequence(std::string("run_shoot_right"));
          }
        }
      }
      break;
    case JUMP:
      spine->SetComplexSequence(std::string("jump"), 24.0f, 2, 11);
      break;
    case ATTACK:
      break;
    }
  }

}