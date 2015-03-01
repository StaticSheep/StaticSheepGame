/*****************************************************************
Filename: CLevel1_Logic.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
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
#include "types/levelEvents/LEGrinderBig.h"
#include "types/levelEvents/LEAsteroids.h"
#include "CSlotController.h"
#include "../colliders/CCircleCollider.h"

static const char *playerNames[] = { "Player1", "Player2", "Player3", "Player4" };
static bool warning;
static float camShakeTime;
static float camShakeMagnitude;
static bool playing;

namespace Framework
{
  Level1_Logic::Level1_Logic()
	{
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
      playerCoins[i] = 1;

    warning = false;
    camShake = false;
    shake = true;
    camShakeTime = 0.25f;
    camShakeMagnitude = 10;
    countDownDone = false;
    slotFinished = false;
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
    space->hooks.Add("SlotFinished", self, BUILD_FUNCTION(Level1_Logic::SlotFinished));
    space->hooks.Add("SetMods", self, BUILD_FUNCTION(Level1_Logic::SetMods));
    space->hooks.Add("SpawnItem", self, BUILD_FUNCTION(Level1_Logic::SpawnItem));
    space->hooks.Add("SpawnItemSet", self, BUILD_FUNCTION(Level1_Logic::SpawnItemSet));
    space->hooks.Add("GivePlayerCoins", self, BUILD_FUNCTION(Level1_Logic::GivePlayerCoins));
    space->hooks.Add("SpawnCoins", self, BUILD_FUNCTION(Level1_Logic::SpawnCoins));

    levelSound = space->GetGameObject(owner)->GetComponentHandle(eSoundPlayer);
    levelCamera = space->GetGameObject(owner)->GetComponentHandle(eCamera);
    levelTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    levelEmitter = space->GetGameObject(owner)->GetComponentHandle(eSoundEmitter);
    levelSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite);

    eventTimer = 6;
    startFlag = true;
    playing = false;
    countDownDone = false;
    countDownTimer = 4.0f;

    for (int i = 0; i < 4; ++i)
      spawnTimers[i] = 2.0f;

    mode = SLOTMACHINE;

	}

  void Level1_Logic::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void Level1_Logic::LogicUpdate(float dt)
	{

    if (camShake)
      CameraShake(dt, camShakeTime, camShakeMagnitude);

    GoToGameMode(dt);
    
    if(!playing)
    {
      SoundPlayer *sp = space->GetHandles().GetAs<SoundPlayer>(levelSound);
      SoundInstance instance;
      instance.volume = 0.35f;
      instance.mode = PLAY_LOOP;

      sp->Play("Main Music", &instance);
      playing = true;
    } 
    //SoundPlayer *sp = space->GetHandles().GetAs<SoundPlayer>(levelSound);
    //sp->SetVolume(0.35f);

    if (LE)
      LE->Update(dt);

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

  void Level1_Logic::PlayerDied(int ply, int who_killed_him)
  {
    if (ply < 0 || ply > numOfPlayers)
      return;
    
    Players[ply] = Handle::null;
    if (who_killed_him != -1)
      playerCoins[who_killed_him] += 5000;
    if (!camShake)
    {
      camShakeTime = 0.25f;
      camShakeMagnitude = 20;
      camShake = true;
    }

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

  bool Level1_Logic::LevelCountdown(float dt)
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
      return true;
    }

    return false;
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
    return playerCoins[ply];
  }

  int Level1_Logic::GetWinner()
  {
    int winner = 1;
    int numAlive = 0;

    for (int i = 0; i < 4; ++i)
    {
      if (playerCoins[i] > 0)
      {
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
      playerCoins[i] = 0;
  }

  void Level1_Logic::ResetPlayers()
  {
    for (int i = 0; i < 4; ++i)
    {
      if (Players[i] != Handle::null)
      {
        space->GetGameObject(Players[i])->GetComponent<PlayerController>(ePlayerController)->health = 0;
      }
    }
  }

  void Level1_Logic::ResetSpawnTimers()
  {
    for (int i = 0; i < 4; ++i)
      spawnTimers[i] = 0.0f;
  }

  void Level1_Logic::GivePlayerCoins(int player, int coins)
  {
    playerCoins[player] += coins;
  }

  void Level1_Logic::SpawnItem(const char *itemName, Vec3 pos)
  {
    GameObject *item = (FACTORY->LoadObjectFromArchetype(space, itemName));
    Transform *ItemTransform = item->GetComponent<Transform>(eTransform);
    if (itemName != "CoinPickup")
    {
      BoxCollider *ItemCollider = item->GetComponent<BoxCollider>(eBoxCollider);
      ItemCollider->SetBodyCollisionGroup("NonCollide");
    }
    else
    {
      CircleCollider *ItemCollider = item->GetComponent<CircleCollider>(eCircleCollider);
      ItemCollider->SetBodyCollisionGroup("NonCollide");
    }

    ItemTransform->SetTranslation(pos);
  }

  void Level1_Logic::SpawnItemSet(Vec3 pos)
  {
    int drop = GetRandom(0, 9);
    if (drop == 0 || drop == 1)
      SpawnItem("AutoPickup", pos);
    else if (drop == 2 || drop == 3)
      SpawnItem("ShotgunPickup", pos);
    else if (drop == 4 || drop == 5)
      SpawnItem("PowerUpPickup_Damage", pos);
    else if (drop == 6 || drop == 7)
      SpawnItem("PowerUpPickup_Shield", pos);
    else if (drop == 8 || drop == 9)
      SpawnItem("PowerUpPickup_Damage", pos);
    if (mod1 == BONUS)
      SpawnCoins(pos);
    if (mod2 == BONUS)
      SpawnCoins(pos);
  }

  void Level1_Logic::SpawnCoins(Vec3 pos)
  {
    for (int i = 0; i < 4; ++i)
    {
      SpawnItem("CoinPickup", pos);
    }
  }

  void Level1_Logic::SpawnLevelEvent()
  {
    if (eventTimer <= 0)
    {
      if (LE)
        delete LE;
      //fire event
      if (GetRandom(0, 1))
        LE = new LEGrinderBig();
      else
        LE = new LEAsteroids();

      LE->FireEvent(space->GetHandles().GetAs<GameObject>(owner));

      eventTimer = (float)GetRandom(20, 30);
      warning = false;
      camShakeTime = 6.5f;
      camShakeMagnitude = 4;
      camShake = true;
    }
    else if (eventTimer > 0.0f && eventTimer < 2.0f && warning == false)
    {
      (FACTORY->LoadObjectFromArchetype(space, "WarnText"))->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(0.0, 0.0, -2.0));
      warning = true;
      space->GetHandles().GetAs<SoundEmitter>(levelEmitter)->Play("warning");
    }
  }

  void Level1_Logic::GoToGameMode(float dt)
  {
    switch (mode)
    {
    case FFA:
      FFAMode(dt);
      break;
    case JUGGERNAUT:
      JuggernautMode(dt);
      break;
    case SUDDENDEATH:
      SuddenDeathMode(dt);
      break;
    case SLOTMACHINE:
      SlotMachineMode(dt);
      break;
    case BONUSMODE:
      BonusMode(dt);
      break;
    }
  }

  void Level1_Logic::FFAMode(float dt)
  {
    if (!countDownDone)
    {
      LevelCountdown(dt);
      return;
    }
    else
      roundTimer -= dt;

    if (roundTimer <= 0)
    {
      mode = SLOTMACHINE;
      return;
    }

    spawnTimer -= dt;
    eventTimer -= dt;
    if (spawnTimer <= 0)
    {
      spawnTimer = 3.0f;
      float ranX = GetRandom(-600, 600);
      float ranY = GetRandom(-300, 300);
      float ranZ = GetRandom(150, 200);
      Vec3 pos(ranX, ranY, ranZ);
      SpawnItemSet(pos);
    }
    SpawnPlayers(dt);

    SpawnLevelEvent();
  }

  void Level1_Logic::JuggernautMode(float dt)
  {
    if (!countDownDone)
    {
      LevelCountdown(dt);
      return;
    }
    else
      roundTimer -= dt;

    if (roundTimer <= 0)
    {
      mode = SLOTMACHINE;
      return;
    }

    spawnTimer -= dt;
    eventTimer -= dt;
    if (spawnTimer <= 0)
    {
      spawnTimer = 3.0f;
      float ranX = GetRandom(-600, 600);
      float ranY = GetRandom(-300, 300);
      float ranZ = GetRandom(150, 200);
      Vec3 pos(ranX, ranY, ranZ);
      SpawnItemSet(pos);
    }
    SpawnPlayers(dt);

    SpawnLevelEvent();
  }

  void Level1_Logic::SuddenDeathMode(float dt)
  {
    if (!countDownDone)
    {
      LevelCountdown(dt);
      return;
    }
    else
      roundTimer -= dt;

    if (roundTimer <= 0)
    {
      mode = SLOTMACHINE;
      return;
    }

    spawnTimer -= dt;
    eventTimer -= dt;

    SpawnPlayers(dt);

  }

  void Level1_Logic::SlotMachineMode(float dt)
  {
    if (!slotFinished)
    {
      ResetPlayers();
      if (LE)
      {
        delete LE;
        LE = 0;
      }
      space->hooks.Call("CallingSM");
      (FACTORY->LoadObjectFromArchetype(space, "LevelSlotMachine"));
      slotFinished = true;
    }
  }

  void Level1_Logic::BonusMode(float dt)
  {
    if (!countDownDone)
    {
      LevelCountdown(dt);
      return;
    }
    else
      roundTimer -= dt;

    if (roundTimer <= 0)
    {
      mode = SLOTMACHINE;
      return;
    }
    if (spawnTimer <= 0)
    {
      float ranX = GetRandom(-600, 600);
      float ranY = GetRandom(-300, 300);
      Vec3 pos(ranX, ranY, 0.0f);
      SpawnCoins(pos);
      spawnTimer = 0.5f;
    }
    spawnTimer -= dt;
    eventTimer -= dt;
    SpawnPlayers(dt);

    SpawnLevelEvent();
  }

  void Level1_Logic::SlotFinished(GameTypes mode_)
  {
    mode = mode_;
    countDownDone = false;
    countDownTimer = 3.0f;
    if (mode != SUDDENDEATH)
      roundTimer = 60.0f;
    else
      roundTimer = 5.0f;

    slotFinished = false;
    ResetSpawnTimers();
  }

  void Level1_Logic::SetMods(GameMods mod1_, GameMods mod2_)
  {
    mod1 = mod1_;
    mod2 = mod2_;
  }
}
