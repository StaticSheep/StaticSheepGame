/*****************************************************************
Filename: AnimationController.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
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

    reverse = false;
  }

  AnimationController::~AnimationController() //4
	{

	}

  //enum AnimationState {IDLE, RUN, JUMP, ATTACK};
  void AnimationController::Update(SpineSprite* spine, Vec4& color, float rotation, Vec3& moveDir, Vec3& aimDir, bool aiming)
  {
    spine->SetColor(color);
    aimDir.Normalize();    

    Mat3D rot(-rotation);

    Vec3 temp = rot * aimDir;
    float move;
    float theta = 0.0f;

    int start = 0;
    int end = 12;

    int state = -1;

    theta = atan2f(temp.y, temp.x) * 57.2957795f;

    if(aiming)
    {
      move = moveDir * aimDir;

      if(move < 0.0f)
        reverse = true;
      else
        reverse = false;

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
    else
    if(moveDir.z != 1.0f)
    {
      Vec3 localX = Mat3D(rotation) * Vec3(1.0f, 0.0f, 0.0f);

      float dotP = localX * moveDir;

      if(dotP < 0.0f)
        spine->FlipX(true);
      else
        spine->FlipX(false);

      reverse = false;
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
        spine->SetComplexSequence(std::string("jump"), 24.0f, 4, 12);
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

    spine->SetReverse(reverse);
  }

}