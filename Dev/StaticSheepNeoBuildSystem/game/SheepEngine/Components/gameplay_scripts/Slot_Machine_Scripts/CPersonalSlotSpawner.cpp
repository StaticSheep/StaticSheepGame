/*****************************************************************
Filename: CPersonalSlotSpawner.cpp
Project:  Gam250
Author(s): Greg Walls (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CPersonalSlotSpawner.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../slotmachine/slotmachine.h"
#include "../../sound/CSoundEmitter.h"
#include "../../gameplay_scripts/Level_Scripts/CLevel1_Logic.h"
#include "CPersonalSlotController.h"
#include "../../basicps/CBasicPSystem.h"
#include "../../sound/CSoundPlayer.h"

namespace Framework
{
  static Vec3 drawPos[4] = { Vec3(-480.0f, -105.3f, 0.0f), Vec3(380.0f, -105.3f, 0.0f), Vec3(380.0f, 260.7f, 0.0f), Vec3(-480.0f, 260.7f, 0.0f) };
  PersonalSlotSpawner::PersonalSlotSpawner()
	{
    playerNum = 0;
    playerCoinTotal = 0;
    totalPSM = 0;
    for (int i = 0; i < 3; ++i)
      spawnedPSM[i] = Handle::null;
    level_logic = Handle::null;
    spawnedCoins = Handle::null;
    timer = 15.0f;
    coinsSpawned = false;
	}

  PersonalSlotSpawner::~PersonalSlotSpawner()
	{

	}

  void PersonalSlotSpawner::Initialize()
	{
		//logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(PersonalSlotSpawner::LogicUpdate));
    space->hooks.Add("Draw", self, BUILD_FUNCTION(PersonalSlotSpawner::Draw));
    space->hooks.Add("PersonalSlotDone", self, BUILD_FUNCTION(PersonalSlotSpawner::AllSlotsDone));

    bounceDownTimer = 0.5f;
    bounceDownDone = false;
    playing = true;
    done_ = false;
    stopEffects = false;
    stop_ = false;
    endOfGame_ = false;
	}

  void PersonalSlotSpawner::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("Draw", self);
	}

  void PersonalSlotSpawner::LogicUpdate(float dt)
	{
    if (!bounceDownDone)
    {
      BounceDown(dt);
      return;
    }

    if (!coinsSpawned)
    {
      spawnedCoins = (FACTORY->LoadObjectFromArchetype(space, "coin_rain"))->self;
      GameObject *coins = space->GetGameObject(spawnedCoins);
      coins->GetComponent<Transform>(eTransform)->SetTranslation(space->GetGameObject(owner)->GetComponent<Transform>(eTransform)->GetTranslation()
                                                                 + Vec3(72.0f, 64.0f, 0.0f));
      coinsSpawned = true;
    }
    
    if (endOfGame_)
    {
      EndOfGame(dt);
    }
    else
    {
      EndOfRound(dt);
    }
    

    timer -= dt;
    if (timer <= 0)
    {
      for (int i = 0; i < 3; ++i)
      {
        if (spawnedPSM[i] != Handle::null)
        {
          space->GetGameObject(spawnedPSM[i])->Destroy();
        }
      }
      if (spawnedCoins != Handle::null)
        space->GetGameObject(spawnedCoins)->Destroy();

      space->GetGameObject(owner)->Destroy();
    }
	}

  static bool soundFlag_ = false;
  void PersonalSlotSpawner::BounceDown(float dt)
  {
    Transform *rt = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);

    SoundEmitter *se = space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter);
    if (bounceDownTimer >= 0.3f)
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, -40.0f, 0.0f));
    else if (bounceDownTimer >= 0.2)
    {
      if (!soundFlag_)
      {
        se->Play("impact1", &SoundInstance(1.0f));
        soundFlag_ = true;
      }
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, 40.0f, 0.0f));
      rt->SetRotation(rt->GetRotation() + 0.035f);
    }
    else
    {
      rt->SetTranslation(rt->GetTranslation() + Vec3(0.0f, -20.0f, 0.0f));
      rt->SetRotation(rt->GetRotation() - 0.017f);
    }

    bounceDownTimer -= dt;
    if (bounceDownTimer <= 0)
    {
      rt->SetRotation(0.0f);
      bounceDownDone = true;
      SoundInstance instance;
      instance.volume = 1.0f;
      instance.mode = PLAY_LOOP;
      space->GetGameObject(owner)->GetComponent<SoundPlayer>(eSoundPlayer)->Play("slot_coin_jackpot", &instance);
      playing = true;
    }
  }

  void PersonalSlotSpawner::SpawnPersonalSM(int num)
  {
    Transform *spawnerPos_ = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);
    Vec3 initOffset(-32.0f, 800.0f, 0.0f);
    Vec3 psmOffset(180.0f, 0.0f, 0.0f);
    Vec3 spawnPos_;

    if (num == 1)
    {
      spawnPos_ = (spawnerPos_->GetTranslation() + initOffset);
    }
    else if (num == 2)
    {
      spawnPos_ = (spawnerPos_->GetTranslation() + initOffset + psmOffset);
    }
    else if (num == 3)
    {
      spawnPos_ = (spawnerPos_->GetTranslation() + initOffset + psmOffset + psmOffset);
      done_ = true;
    }

    spawnedPSM[num - 1] = (FACTORY->LoadObjectFromArchetype(space, "playerSlotMachine"))->self;
    GameObject *psm_ = space->GetGameObject(spawnedPSM[num - 1]);
    psm_->GetComponent<Transform>(eTransform)->SetTranslation(spawnPos_);
    psm_->GetComponent<PersonalSlotController>(ePersonalSlotController)->playerNum = playerNum;
    psm_->GetComponent<PersonalSlotController>(ePersonalSlotController)->psmNum = (num - 1);

  }

  void PersonalSlotSpawner::SpawnBonusChip(int num)
  {
    Transform *spawnerPos_ = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);
    Vec3 initOffset(-32.0f, 800.0f, 0.0f);
    Vec3 psmOffset(180.0f, 0.0f, 0.0f);
    Vec3 spawnPos_;

    if (num == 1)
    {
      spawnPos_ = (spawnerPos_->GetTranslation() + initOffset);
    }
    else if (num == 2)
    {
      spawnPos_ = (spawnerPos_->GetTranslation() + initOffset + psmOffset);
    }
    else if (num == 3)
    {
      spawnPos_ = (spawnerPos_->GetTranslation() + initOffset + psmOffset + psmOffset);
      done_ = true;
    }

    spawnedPSM[num - 1] = (FACTORY->LoadObjectFromArchetype(space, "playerSlotMachine"))->self;
    GameObject *psm_ = space->GetGameObject(spawnedPSM[num - 1]);
    psm_->GetComponent<Transform>(eTransform)->SetTranslation(spawnPos_);
    psm_->GetComponent<PersonalSlotController>(ePersonalSlotController)->playerNum = playerNum;
    psm_->GetComponent<PersonalSlotController>(ePersonalSlotController)->psmNum = (num - 1);
    if (endOfGame_)
      psm_->GetComponent<PersonalSlotController>(ePersonalSlotController)->gold_ = true;
  }

  void PersonalSlotSpawner::AllSlotsDone()
  {
    if (!stop_)
    {
      timer = 5.0f;
      space->GetGameObject(owner)->GetComponent<SoundPlayer>(eSoundPlayer)->Stop("slot_coin_jackpot", INSTANT);
      stop_ = true;
    }
  }

  void PersonalSlotSpawner::Draw()
  {
    if (playerCoinTotal == 0)
      return;

    Vec2D scale(30, 30);
    char playerString[10];

    Draw::SetPosition(drawPos[playerNum].x, drawPos[playerNum].y);
    Draw::SetColor(0.9, 0.9, 0.15f, 1); //yellow-ish color
    Draw::SetRotation(0);
    itoa(playerCoinTotal, playerString, 10);
    Draw::DrawString(playerString, scale, 1);
  }

  void PersonalSlotSpawner::EndOfRound(float dt)
  {
    //grab level logic so i can get a hold of the players coin counts
    Level1_Logic *ll = space->GetGameObject(level_logic)->GetComponent<Level1_Logic>(eLevel1_Logic);
    if (!done_)//as long as we are not done
    {
      for (int i = 0; i < 25; ++i) //25 is the number of coins per frame I'm deducting from their total
      {
        if (!ll) //if for some reason this is null
          break;

        if (ll->playerCoins[playerNum] != 0 && totalPSM < 3) //As long as they have coins and I haven't spawned three slots
        {
          ++playerCoinTotal;
          --(ll->playerCoins[playerNum]);
        }
        else
          break;
      }
    }

    if (playerCoinTotal >= 10000 || ll->playerCoins[playerNum] <= 0)
    {
      //if we have spawned all our slots OR we don't have any coins left
      done_ = true; //than we are done
      if (!stopEffects)//if we haven't stopped our effects
      {
        space->GetGameObject(spawnedCoins)->GetComponent<BasicParticleSystem>(eBasicParticleSystem)->Toggle(false); //stop coins
        stopEffects = true;
      }
    }

    if (playerCoinTotal >= 500 && totalPSM == 0)
    {
      ++totalPSM;
      SpawnPersonalSM(totalPSM);
    }
    else if (playerCoinTotal >= 5000 && totalPSM == 1)
    {
      ++totalPSM;
      SpawnPersonalSM(totalPSM);
    }
    else if (playerCoinTotal >= 10000 && totalPSM == 2)
    {
      ++totalPSM;
      SpawnPersonalSM(totalPSM);
    }
  }

  void PersonalSlotSpawner::EndOfGame(float dt)
  {
    int inc_amount = 50;
    if (playerCoinTotal > 10000)
      inc_amount = 100;
    //grab level logic so i can get a hold of the players coin counts
    Level1_Logic *ll = space->GetGameObject(level_logic)->GetComponent<Level1_Logic>(eLevel1_Logic);
    if (!done_)//as long as we are not done
    {
      for (int i = 0; i < inc_amount; ++i) //inc_amount is the number of coins per frame I'm deducting from their total
      {
        if (!ll) //if for some reason this is null
          break;

        if (ll->playerCoins[playerNum] != 0 && totalPSM < 3) //As long as they have coins and I haven't spawned three slots
        {
          ++playerCoinTotal;
          --(ll->playerCoins[playerNum]);
        }
        else
          break;
      }
    }

    if (ll->playerCoins[playerNum] <= 0)
    {
      //if we have spawned all our slots OR we don't have any coins left
      done_ = true; //than we are done
      if (!stopEffects)//if we haven't stopped our effects
      {
        space->GetGameObject(spawnedCoins)->GetComponent<BasicParticleSystem>(eBasicParticleSystem)->Toggle(false); //stop coins
        stopEffects = true;
      }
    }

    if (playerCoinTotal >= 5000 && totalPSM == 0)
    {
      ++totalPSM;
      SpawnBonusChip(totalPSM);
    }
    else if (playerCoinTotal >= 10000 && totalPSM == 1)
    {
      ++totalPSM;
      SpawnBonusChip(totalPSM);
    }
    else if (playerCoinTotal >= 50000 && totalPSM == 2)
    {
      ++totalPSM;
      SpawnBonusChip(totalPSM);
    }
  }
}
