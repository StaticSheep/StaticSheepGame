#include "pch/precompiled.h"
#include "CLevel1_Logic.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CBoxCollider.h"
#include "../sound/CSoundPlayer.h"
#include "CElevatorPlat.h"
#include "../controllers/player/CPlayerController.h"
#include "../SheepUtil/include/SheepMath.h"

static const char *playerNames[] = { "Player1", "Player2", "Player3", "Player4" };

namespace Framework
{
  Level1_Logic::Level1_Logic()
	{
    timeLimit = 60;
    spawnTimer = 2;
    numOfPlayers = 1;
    spawnPos[0] = Vec3(-490.0f, -266.0f, 0.0f);
    spawnPos[1] = Vec3(490.0f, -266.0f, 0.0f);
    spawnPos[2] = Vec3(490.0f, 266.0f, 0.0f);
    spawnPos[3] = Vec3(-490.0f, 266.0f, 0.0f);
	}

  Level1_Logic::~Level1_Logic()
	{

	}

  void Level1_Logic::Initialize()
	{
		//logic setup, you're attached and components are in place
		space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Level1_Logic::LogicUpdate));
    space->hooks.Add("PlayerDied", self, BUILD_FUNCTION(Level1_Logic::PlayerDied));
    levelSound = space->GetGameObject(owner)->GetComponentHandle(eSoundPlayer);
    timeLimit = 60;
    startFlag = true;
    for (int i = 0; i < 4; ++i)
      spawnTimers[i] = 2.0f;
	}

  void Level1_Logic::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void Level1_Logic::LogicUpdate(float dt)
	{

    SpawnPlayers(dt);
    
    static bool playing = false;
    spawnTimer -= dt;
    timeLimit -= dt;

    if (spawnTimer <= 0)
    {
      int randomDrop = GetRandom(0, 2);

      GameObject *ePlat = (FACTORY->LoadObjectFromArchetype(space, "SmallPlat"));
      Transform *PT = ePlat->GetComponent<Transform>(eTransform);
      //BoxCollider *platC = ePlat->GetComponent <BoxCollider>(eBoxCollider);
      ePlat->GetComponent<ElevatorPlat>(eElevatorPlat)->direction = true;
      PT->SetTranslation(Vec3(242.0,-320.0,0.0));

      if (randomDrop == 0)
      {
        GameObject *weap = (FACTORY->LoadObjectFromArchetype(space, "ShotgunPickup"));
        Transform *WT = weap->GetComponent<Transform>(eTransform);
        WT->SetTranslation(PT->GetTranslation() + Vec3(0.0, 16.0, 0.0));
      }

      randomDrop = GetRandom(0, 2);

      GameObject *ePlat2 = (FACTORY->LoadObjectFromArchetype(space, "SmallPlat"));
      Transform *PT2 = ePlat2->GetComponent<Transform>(eTransform);
      //BoxCollider *platC2 = ePlat2->GetComponent <BoxCollider>(eBoxCollider);
      ePlat2->GetComponent<ElevatorPlat>(eElevatorPlat)->direction = false;
      PT2->SetTranslation(Vec3(-235.0, 320.0, 0.0));

      if (randomDrop == 0)
      {
        GameObject *weap = (FACTORY->LoadObjectFromArchetype(space, "AutoPickup"));
        Transform *WT = weap->GetComponent<Transform>(eTransform);
        WT->SetTranslation(PT2->GetTranslation() + Vec3(0.0, -16.0, 0.0));
      }

      spawnTimer = 2;
    }

    if (timeLimit <= 0)
    {
      GameObject *eGiantPlat = (FACTORY->LoadObjectFromArchetype(space, "KillBoxBig"));
      Transform *GPT = eGiantPlat->GetComponent<Transform>(eTransform);
      //BoxCollider *gaintPlatC = eGiantPlat->GetComponent <BoxCollider>(eBoxCollider);
      GPT->SetTranslation(Vec3(600.0, 0.0, 0.0));
      timeLimit = 60;
    }

    
    if(!playing)
    {
      SoundPlayer *sp = space->GetHandles().GetAs<SoundPlayer>(levelSound);

      // So for SoundPlayers and SoundEmitters, if you want to change the way they 
      // play things, you will have to make a SoundInstance, and change the things
      // in the instance and give it to the Play function.

      // SoundPlayer keeps track of instances, so if you want to stop a song,
      // you can do sp->Stop("space_brawl") and it'll stop.

      SoundInstance instance;
      instance.volume = 0.25f;
      instance.mode = PLAY_LOOP;

      sp->Play("Main Music", &instance);
      playing = true;
    } 
    SoundPlayer *sp = space->GetHandles().GetAs<SoundPlayer>(levelSound);
    sp->SetVolume(0.25f);

	}

  void Level1_Logic::SpawnPlayers(float dt)
  {
    Transform *playTrans;
    if (startFlag)
    {
      for (int i = 0; i < numOfPlayers; ++i)
      {
        Players[i] = (FACTORY->LoadObjectFromArchetype(space, playerNames[i]))->self;

        playTrans = space->GetGameObject(Players[i])->GetComponent<Transform>(eTransform);
        playTrans->SetTranslation(spawnPos[i]);
      }
      startFlag = false;
    }

    for (int i = 0; i < numOfPlayers; ++i)
    {
      if (Players[i] == Handle::null && spawnTimers[i] <= 0)
      {
        Players[i] = (FACTORY->LoadObjectFromArchetype(space, playerNames[i]))->self;

        playTrans = space->GetGameObject(Players[i])->GetComponent<Transform>(eTransform);
        playTrans->SetTranslation(spawnPos[i]);
        spawnTimers[i] = 2.0f;
        space->GetGameObject(Players[i])->GetComponent<PlayerController>(ePlayerController)->hasRespawned = true;
      }
      else if (Players[i] == Handle::null && spawnTimers[i] > 0)
        spawnTimers[i] -= dt;
      
    }

  }

  void Level1_Logic::PlayerDied(int ply)
  {
    if (ply < 0 || ply > numOfPlayers)
      return;
    
    Players[ply] = Handle::null;
  }

}
