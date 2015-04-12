/*****************************************************************
Filename: CGiantKillBox.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CGiantKillBox.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../sound/CSoundPlayer.h"

namespace Framework
{
  GiantKillBox::GiantKillBox()
	{
    //default
    direction = true;
    numOfGrinders = 9;
    sizeOfGrinder = 60.0f;
    itemDropTimer = 1.0f;
	}

  GiantKillBox::~GiantKillBox()
	{

	}

  void GiantKillBox::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(GiantKillBox::LogicUpdate));
    space->hooks.Add("RoundOver", self, BUILD_FUNCTION(GiantKillBox::DestroySelf));
    //space->hooks.Add("CallingSM", self, BUILD_FUNCTION(GiantKillBox::CallingSM));

    kbTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    kbCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);
    GrindSpawn = false;
    playing = false;
	}

  void GiantKillBox::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("RoundOver", self);
	}

  void GiantKillBox::LogicUpdate(float dt)
	{
    Transform *pt = space->GetHandles().GetAs<Transform>(kbTransfrom);
    BoxCollider *pc = space->GetHandles().GetAs <BoxCollider>(kbCollider);
    if (!playing)
    {
      SoundPlayer *sp = space->GetGameObject(owner)->GetComponent<SoundPlayer>(eSoundPlayer);
      SoundInstance instance;
      instance.volume = 0.6f;
      instance.mode = PLAY_LOOP;
      sp->Play("saw", &instance);
      playing = true;
    }
    //spawn grinders
    if (!GrindSpawn)
    {
      for (int i = 0; i < numOfGrinders; ++i)
      {
        Grinders[i] = (FACTORY->LoadObjectFromArchetype(space, "GrinderBig"))->self;
      }
      GrindSpawn = true;
    }
    //move the level event
    if (direction)
    {
      pt->SetTranslation(pt->GetTranslation() + Vec3(-5.0, 0.0, 0.0));
      for (int i = 0; i < numOfGrinders; ++i)
      {
        GT[i] = (space->GetGameObject(Grinders[i])->GetComponent<Transform>(eTransform));
        GT[i]->SetTranslation(pt->GetTranslation() + Vec3(0.0f, ((float)i * sizeOfGrinder - (sizeOfGrinder * (numOfGrinders / 2))), -0.1f));
      }
    }
    else
    {
      pt->SetTranslation(pt->GetTranslation() + Vec3(5.0, 0.0, 0.0));
      for (int i = 0; i < numOfGrinders; ++i)
      {
        GT[i] = (space->GetGameObject(Grinders[i])->GetComponent<Transform>(eTransform));
        GT[i]->SetTranslation(pt->GetTranslation() + Vec3(0.0f, ((float)i * sizeOfGrinder - (sizeOfGrinder * (numOfGrinders / 2))), -0.1f));
      }
    }

    itemDropTimer -= dt;
    if (itemDropTimer <= 0)
    {
      if (!GetRandom(0, 1))
        space->hooks.Call("SpawnItemSet", pt->GetTranslation());
      itemDropTimer = 1.0f;
    }

    //kill grinders that are out of bounds
    if (pt->GetTranslation().x > 1008 || pt->GetTranslation().x < -1008)
    {
      for (int i = 0; i < numOfGrinders; ++i)
        space->GetGameObject(Grinders[i])->Destroy();

      GrindSpawn = false;
      space->GetGameObject(owner)->Destroy();
    }
	}

  void GiantKillBox::CallingSM()
  {
    /*if (owner)
      space->GetGameObject(owner)->Destroy();*/
  }

  void GiantKillBox::DestroySelf()
  {
    for (int i = 0; i < numOfGrinders; ++i)
      space->GetGameObject(Grinders[i])->Destroy();
    SoundPlayer *sp = space->GetGameObject(owner)->GetComponent<SoundPlayer>(eSoundPlayer);
    sp->Stop("saw", INSTANT);
    space->GetGameObject(owner)->Destroy();
  }
}
