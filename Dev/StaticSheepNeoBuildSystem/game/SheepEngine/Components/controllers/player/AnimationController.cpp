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

    int start = 0;
    int end = 12;

    int state = -1;

    theta = atan2f(temp.y, temp.x) * 57.2957795;

        //ENGINE->TraceLog.Log(Framework::TraceLevel::DBG, "theta = %f", theta);
    if(aiming)
    {
      if(theta > 0.0f)
      {
        if(theta < 90.0f)
          spine->FlipX(false);
        else
          spine->FlipX(true);

        if(theta < 22.5f)
        {
          //spine->SetComplexSequence(std::string("run_shoot_right"), 24.0f, start, end);
          state = RIGHT;
        }
        else
        if(theta < 67.5f)
        {
          //spine->SetComplexSequence(std::string("run_shoot_upright"), 24.0f, start, end);
          state = UPRIGHT;
        }
        else
        if(theta < 112.5f)
        {
          //spine->SetComplexSequence(std::string("run_shoot_up"), 24.0f, start, end);
          state = UP;
        }
        else
        if(theta < 157.5f)
        {
          //spine->SetComplexSequence(std::string("run_shoot_upright"), 24.0f, start, end);
          state = UPRIGHT;
        }
        else
        {
          //spine->SetComplexSequence(std::string("run_shoot_right"), 24.0f, start, end);
          state = RIGHT;
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
          //spine->SetComplexSequence(std::string("run_shoot_right"), 24.0f, start, end);
          state = RIGHT;
        }
        else
        if(theta > -67.5f)
        {
          //spine->SetComplexSequence(std::string("run_shoot_downright"), 24.0f, start, end);
          state = DOWNRIGHT;
        }
        else
        if(theta > -112.5f)
        {
          //spine->SetComplexSequence(std::string("run_shoot_down"), 24.0f, start, end);
          state = DOWN;
        }
        else
        if(theta > -157.5f)
        {
          //spine->SetComplexSequence(std::string("run_shoot_downright"), 24.0f, start, end);
          state = DOWNRIGHT;
        }
        else
        {
          //spine->SetComplexSequence(std::string("run_shoot_right"), 24.0f, start, end);
          state = RIGHT;
        }
      }
    }

    switch(AnimState)
    {
    case IDLE:
      if(state == -1)
        spine->SetSequence(std::string("idle"));
      else
      {
        switch(state)
        {
          case UP:
            spine->SetSequence(std::string("idle_shoot_up"));
            break;
          case UPRIGHT:
            spine->SetSequence(std::string("idle_shoot_upright"));
            break;
          case RIGHT:
            spine->SetSequence(std::string("idle_shoot_right"));
            break;
          case DOWNRIGHT:
            spine->SetSequence(std::string("idle_shoot_downright"));
            break;
          case DOWN:
            spine->SetSequence(std::string("idle_shoot_down"));
            break;
        }
      }
      break;
    case JUMP:
      if(state == -1)
        spine->SetComplexSequence(std::string("jump"), 12.0f, 4, 12);
      else
      {
        switch(state)
        {
          case UP:
            spine->SetComplexSequence(std::string("jump_shoot_up"), 12.0f, 4, 12);
            break;
          case UPRIGHT:
            spine->SetComplexSequence(std::string("jump_shoot_upright"), 12.0f, 4, 12);
            break;
          case RIGHT:
            spine->SetComplexSequence(std::string("jump_shoot_right"), 12.0f, 4, 12);
            break;
          case DOWNRIGHT:
            spine->SetComplexSequence(std::string("jump_shoot_downright"), 12.0f, 4, 12);
            break;
          case DOWN:
            spine->SetComplexSequence(std::string("jump_shoot_down"), 12.0f, 4, 12);
            break;
        }
      }
      break;
    case RUN:
      if(state == -1)
      {
        spine->SetSequence(std::string("run"));
      }
      else
      {
        switch(state)
        {
          case UP:
            spine->SetSequence(std::string("run_shoot_up"));
            break;
          case UPRIGHT:
            spine->SetSequence(std::string("run_shoot_upright"));
            break;
          case RIGHT:
            spine->SetSequence(std::string("run_shoot_right"));
            break;
          case DOWNRIGHT:
            spine->SetSequence(std::string("run_shoot_downright"));
            break;
          case DOWN:
            spine->SetSequence(std::string("run_shoot_down"));
            break;
        }
      }
      break;
    case ATTACK:
      break;
    }
  }

}