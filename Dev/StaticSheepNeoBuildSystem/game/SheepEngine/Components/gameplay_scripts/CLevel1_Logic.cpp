/*****************************************************************
Filename: CLevel1_Logic.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CLevel1_Logic.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CBoxCollider.h"
#include "../sound/CSoundPlayer.h"
#include "CElevatorPlat.h"
#include "../controllers/player/CPlayerController.h"
#include "../SheepUtil/include/SheepMath.h"
#include "CGiantKillBox.h"
#include "../camera/CCamera.h"
#include "../particles/CParticleSystem.h"
#include "../sprites/CSprite.h"

static const char *playerNames[] = { "Player1", "Player2", "Player3", "Player4" };
static bool warning;
static float camShakeTime;
static float camShakeMagnitude;
static bool playing;

namespace Framework
{
  Level1_Logic::Level1_Logic()
	{
    timeLimit = 60;
    spawnTimer = 3;
    numOfPlayers = 1;
    spawnPos[0] = Vec3(-610.0f, -440.0f, 0.0f);
    spawnPos[1] = Vec3(610.0f, -440.0f, 0.0f);
    spawnPos[2] = Vec3(610.0f, 435.0f, 0.0f);
    spawnPos[3] = Vec3(-610.0f, 435.0f, 0.0f);
    spawnPos[4] = Vec3(0.0f, 435.0f, 0.0f);
    spawnPos[5] = Vec3(0.0f, -440.0f, 0.0f);
    deadPlayers = 0;
    for (int i = 0; i < 4; ++i)
      playerLives[i] = 5;

    warning = false;
    camShake = false;
    shake = true;
    camShakeTime = 0.25f;
    camShakeMagnitude = 10;
    countDownDone = false;
	}

  Level1_Logic::~Level1_Logic()
	{
	}

  void Level1_Logic::Initialize()
	{
		//logic setup, you're attached and components are in place
		space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Level1_Logic::LogicUpdate));
    space->hooks.Add("PlayerDied", self, BUILD_FUNCTION(Level1_Logic::PlayerDied));
    space->hooks.Add("CheatWin", self, BUILD_FUNCTION(Level1_Logic::CheatWin));
    levelSound = space->GetGameObject(owner)->GetComponentHandle(eSoundPlayer);
    levelCamera = space->GetGameObject(owner)->GetComponentHandle(eCamera);
    levelTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    levelEmitter = space->GetGameObject(owner)->GetComponentHandle(eSoundEmitter);
    levelSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite);

    timeLimit = 6;
    startFlag = true;
    playing = false;
    countDownDone = false;
    countDownTimer = 4.0f;

    for (int i = 0; i < 4; ++i)
      spawnTimers[i] = 2.0f;

	}

  void Level1_Logic::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void Level1_Logic::LogicUpdate(float dt)
	{

    if (!countDownDone)
    {
      Sprite *ls = space->GetHandles().GetAs<Sprite>(levelSprite);
      //run countdown
      if (countDownTimer <= 3.0f && countDownTimer > 2.0f)
      {
        ls->SetTexture("cd_3.png");
      }
      else if (countDownTimer <= 2.0f && countDownTimer > 1.0f)
      {
        //change sprite
        ls->SetTexture("cd_2.png");
        
      }
      else if (countDownTimer <= 1.0f && countDownTimer > 0.0f)
      {
        //change sprite
        ls->SetTexture("cd_1.png");

      }

      countDownTimer -= dt;
      if (countDownTimer <= 0)
      {
        countDownDone = true;
        ls->SetTexture("blank.png");
      }
      return;
    }

    if (camShake)
      CameraShake(dt, camShakeTime, camShakeMagnitude);

    SpawnPlayers(dt);
    
    
    spawnTimer -= dt;
    timeLimit -= dt;

    if (spawnTimer <= 0)
    {
      int randomDrop = GetRandom(0, 5);

      if (randomDrop == 0)
      {
        GameObject *weap = (FACTORY->LoadObjectFromArchetype(space, "ShotgunPickup"));
        Transform *WT = weap->GetComponent<Transform>(eTransform);
        BoxCollider *WC = weap->GetComponent<BoxCollider>(eBoxCollider);
        WC->SetBodyCollisionGroup("Collide");
        int ranX = GetRandom(-300, 300);
        int ranY = GetRandom(-300, 300);
        WT->SetTranslation(Vec3(ranX, ranY, 0.0f));
      }
      else if (randomDrop == 3)
      {
        GameObject *weap = (FACTORY->LoadObjectFromArchetype(space, "AutoPickup"));
        Transform *WT = weap->GetComponent<Transform>(eTransform);
        BoxCollider *WC = weap->GetComponent<BoxCollider>(eBoxCollider);
        WC->SetBodyCollisionGroup("Collide");
        int ranX = GetRandom(-300, 300);
        int ranY = GetRandom(-300, 300);
        WT->SetTranslation(Vec3(ranX, ranY, 0.0f));
      }

      spawnTimer = 2;
    }

    if (timeLimit <= 0)
    {
      GameObject *eGiantPlat = (FACTORY->LoadObjectFromArchetype(space, "KillBoxBig"));
      Transform *GPT = eGiantPlat->GetComponent<Transform>(eTransform);
      //BoxCollider *gaintPlatC = eGiantPlat->GetComponent <BoxCollider>(eBoxCollider);
      if (GetRandom(0, 1))
      {
        eGiantPlat->GetComponent<GiantKillBox>(eGiantKillBox)->direction = true;
        GPT->SetTranslation(Vec3(1000.0, 0.0, 0.0));
      }
      else
      {
        eGiantPlat->GetComponent<GiantKillBox>(eGiantKillBox)->direction = false;
        GPT->SetTranslation(Vec3(-1000.0, 0.0, 0.0));
      }
      timeLimit = (float)GetRandom(30, 60);
      warning = false;
      camShakeTime = 8.5f;
      camShakeMagnitude = 4;
      camShake = true;
    }
    else if (timeLimit > 0.0f && timeLimit < 2.0f && warning == false)
    {
      (FACTORY->LoadObjectFromArchetype(space, "WarnText"))->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(0.0,0.0,-2.0));
      warning = true;
      space->GetHandles().GetAs<SoundEmitter>(levelEmitter)->Play("warning");
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
      instance.volume = 0.40f;
      instance.mode = PLAY_LOOP;

      sp->Play("Main Music", &instance);
      playing = true;
    } 
    SoundPlayer *sp = space->GetHandles().GetAs<SoundPlayer>(levelSound);
    sp->SetVolume(0.35f);

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
    else
    {
      for (int i = 0; i < numOfPlayers; ++i)
      {
        if (playerLives[i] <= 0)
          continue;
        int ranStart = GetRandom(0, 5);
        if (Players[i] == Handle::null && spawnTimers[i] <= 0)
        {
          Players[i] = (FACTORY->LoadObjectFromArchetype(space, playerNames[i]))->self;

          playTrans = space->GetGameObject(Players[i])->GetComponent<Transform>(eTransform);
          playTrans->SetTranslation(spawnPos[ranStart]);
          spawnTimers[i] = 2.0f;
          space->GetGameObject(Players[i])->GetComponent<PlayerController>(ePlayerController)->hasRespawned = true;
        }
        else if (Players[i] == Handle::null && spawnTimers[i] > 0)
          spawnTimers[i] -= dt;
      }
    }

  }

  void Level1_Logic::PlayerDied(int ply)
  {
    if (ply < 0 || ply > numOfPlayers)
      return;
    
    Players[ply] = Handle::null;
    if (!camShake)
    {
      camShakeTime = 0.25f;
      camShakeMagnitude = 10;
      camShake = true;
    }
    playerLives[ply] -= 1;
    if (playerLives[ply] <= 0)
      ++deadPlayers;
  }

  void Level1_Logic::CameraShake(float dt, float shakeDuration, float magnitude)
  {
    Transform *lc = space->GetHandles().GetAs<Transform>(levelTransform);
    if (shake)
    {
      float distanceX = (float)GetRandom((int)-magnitude, (int)magnitude);
      float distanceY = (float)GetRandom((int)-magnitude, (int)magnitude);
      lc->SetTranslation(lc->GetTranslation() + Vec3(distanceX, distanceY, 0.0));
      shake = false;
    }
    else
    {
      lc->SetTranslation(Vec3(0, 0, 0));
      shake = true;
    }
    if (camShakeTime <= 0)
    {
      camShake = false;
      lc->SetTranslation(Vec3(0, 0, 0));
    }

    camShakeTime -= dt;
  }

  int Level1_Logic::GetPlayerHealth(int ply)
  {
    if (Players[ply] == Handle::null)
      return 0;
    else
    {
      return space->GetGameObject(Players[ply])->GetComponent<PlayerController>(ePlayerController)->CurrentHealth();
    }
  }

  int Level1_Logic::GetPlayerLives(int ply)
  {
    return playerLives[ply];
  }

  int Level1_Logic::GetWinner()
  {
    int winner = 1;
    int numAlive = 0;

    for (int i = 0; i < 4; ++i)
    {
      if (playerLives[i] > 0)
      {
        //restart the level here
        winner = i + 1;
        ++numAlive;
      }
    }

    if (numAlive > 1)
      return 0;
    else
      return winner;
  }

  void Level1_Logic::EndMatch()
  {
    ENGINE->ChangeLevel("Asteroid");
  }

  void Level1_Logic::CheatWin()
  {
    for (int i = 0; i < 4; ++i)
      playerLives[i] = 0;
  }
}
