/*****************************************************************
Filename: CLevel1_Logic.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CLevel1_Logic.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../sound/CSoundPlayer.h"
#include "../Level_Event_Scripts/CElevatorPlat.h"
#include "../../controllers/player/CPlayerController.h"
#include "../../SheepUtil/include/SheepMath.h"
#include "../Level_Event_Scripts/CGiantKillBox.h"
#include "../../camera/CCamera.h"
#include "../../particles/CParticleSystem.h"
#include "../../sprites/CSprite.h"
#include "types/levelEvents/LEGrinderBig.h"
#include "types/levelEvents/LEAsteroids.h"
#include "types/levelEvents/LEPinwheel.h"
#include "../Slot_Machine_Scripts/CSlotController.h"
#include "../../colliders/CCircleCollider.h"
#include "types/powerUps/PDamage.h"
#include "../FX_Scripts/CJuggernautEffect.h"
#include "types/weapons/WShotgun.h"
#include "types/weapons/WMissile.h"
#include "../arena/CBlockLights.h"
#include "../../controllers/round/CRoundController.h"
#include "../../controllers/chip/CChipController.h"
#include "../../controllers/round/CRoundText.h"
#include "../../controllers/lobby/CLobbyController.h"
#include "../../controllers/light patterns/CLightPatternController.h"
#include "../../controllers/player/CombatController.h"

static const char *playerNames[] = { "Player1", "Player2", "Player3", "Player4" };
static int juggKills[4] = { 0, 0, 0, 0 };
static bool warning;
static float camShakeTime;
static float camShakeMagnitude;
static bool playing;
static float deltaTime;

#define BONUS_SHIELD 50.0f


namespace Framework
{

  Level1_Logic::Level1_Logic()
  {
    spawnTimer = 3;
    numOfPlayers = 1;
    playerLoadouts[0] = 0;
    playerLoadouts[1] = 0;
    playerLoadouts[2] = 0;
    playerLoadouts[3] = 0;
    spawnPos[0] = Vec3(-610.0f, -440.0f, 0.0f);
    spawnPos[1] = Vec3(610.0f, -440.0f, 0.0f);
    spawnPos[2] = Vec3(610.0f, 435.0f, 0.0f);
    spawnPos[3] = Vec3(-610.0f, 435.0f, 0.0f);
    spawnPos[4] = Vec3(0.0f, 352.0f, 0.0f);
    spawnPos[5] = Vec3(0.0f, -352.0f, 0.0f);
    CoinStackPos[0] = Vec3(-906.4776f, -650.0f, 0.0f);
    CoinStackPos[1] = Vec3(906.4776f, -650.0f, 0.0f);
    CoinStackPos[2] = Vec3(906.4776f, 650.0f, 0.0f);
    CoinStackPos[3] = Vec3(-906.4776f, 650.0f, 0.0f);
    deadPlayers = 0;
    for (int i = 0; i < 4; ++i)
    {
      playerCoins[i] = 0;
      Players[i] = Handle::null;
    }

    warning = false;
    camShake = false;
    shake = true;
    camShakeTime = 0.25f;
    camShakeMagnitude = 10;
    countDownDone = false;
    slotFinished = false;
    roundStart = false;
    lobbySpawned = false;
    playerJoined[0];
  }

  Level1_Logic::~Level1_Logic()
  {
  }

  void Level1_Logic::Initialize()
  {
    //logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Level1_Logic::LogicUpdate));
    space->hooks.Add("Draw", self, BUILD_FUNCTION(Level1_Logic::Draw));
    space->hooks.Add("PlayerDied", self, BUILD_FUNCTION(Level1_Logic::PlayerDied));
    space->hooks.Add("CheatWin", self, BUILD_FUNCTION(Level1_Logic::CheatWin));
    space->hooks.Add("SlotFinished", self, BUILD_FUNCTION(Level1_Logic::SlotFinished));
    space->hooks.Add("SetMods", self, BUILD_FUNCTION(Level1_Logic::SetMods));
    space->hooks.Add("SpawnItem", self, BUILD_FUNCTION(Level1_Logic::SpawnItem));
    space->hooks.Add("SpawnItemSet", self, BUILD_FUNCTION(Level1_Logic::SpawnItemSet));
    space->hooks.Add("GivePlayerCoins", self, BUILD_FUNCTION(Level1_Logic::GivePlayerCoins));
    space->hooks.Add("SpawnCoins", self, BUILD_FUNCTION(Level1_Logic::SpawnCoins));
    space->hooks.Add("RoundOver", self, BUILD_FUNCTION(Level1_Logic::RoundOver));
    space->hooks.Add("SpawnCoinsEx", self, BUILD_FUNCTION(Level1_Logic::SpawnCoinsEx));
    space->hooks.Add("GameStart", self, BUILD_FUNCTION(Level1_Logic::GameStart));

    space->hooks.Add("GivePlayerItem", self,
      BUILD_FUNCTION(Level1_Logic::GivePlayerLoadoutItem));

    levelSound = space->GetGameObject(owner)->GetComponentHandle(eSoundPlayer);
    levelCamera = space->GetGameObject(owner)->GetComponentHandle(eCamera);
    levelTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    levelEmitter = space->GetGameObject(owner)->GetComponentHandle(eSoundEmitter);
    levelSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite);

    lastMode = IDLE_STATE;

    eventTimer = 6;
    startFlag = true;
    playing = true;
    countDownDone = false;
    countDownTimer = 3.5f;
    timeAsJugg = 0;

    for (int i = 0; i < 4; ++i)
    {
      spawnTimers[i] = 2.0f;
      juggernaut[i] = false;
      playerCoinsThisFrame[i] = 0;
      num_spawned[i] = false;
    }

    for (int i = 0; i < 4; ++i)
    {
      playerLoadouts[i] = 0;
    }

    mode = SLOTMACHINE;

    fontIndex = Draw::GetFontIndex("BN_Jinx");

  }

  void Level1_Logic::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("Draw", self);
    space->hooks.Remove("PlayerDied", self);
    space->hooks.Remove("CheatWin", self);
    space->hooks.Remove("SlotFinished", self);
    space->hooks.Remove("SetMods", self);
    space->hooks.Remove("SpawnItem", self);
    space->hooks.Remove("SpawnItemSet", self);
    space->hooks.Remove("GivePlayerCoins", self);
    space->hooks.Remove("SpawnCoins", self);
    space->hooks.Remove("RoundOver", self);
    space->hooks.Remove("SpawnCoinsEx", self);
    space->hooks.Remove("GivePlayerItem", self);
  }

  void Level1_Logic::LogicUpdate(float dt)
  {
    if (camShake)
      CameraShake(dt, camShakeTime, camShakeMagnitude);
    deltaTime = dt;
    GoToGameMode(dt);
    //UpdateCoinStacks();
    if (!playing)
    {
      SpawnCoinStacks();
      SoundPlayer *sp = space->GetHandles().GetAs<SoundPlayer>(levelSound);
      SoundInstance instance;
      instance.volume = 0.50f;
      instance.mode = PLAY_LOOP;
      instance.isMusic = true;

      sp->Play("tripg", &instance);
      playing = true;
    }

    if (LE != nullptr)
      LE->Update(dt);
  }

  void Level1_Logic::GameStart()
  {
    ResetPlayers();
    mode = SLOTMACHINE;
  }

  void Level1_Logic::GivePlayerLoadoutItem(int playerNum, PlayerLoadout item)
  {
    playerLoadouts[playerNum] = playerLoadouts[playerNum] | item;

    TRACELOG->Log(TraceLevel::DBG,
      "Player %d won loadout item %d.", playerNum, item);
  }

  void Level1_Logic::SetupPlayer(PlayerController* pc)
  {
    if (mod2 == SHOTGUNS)
    {
      pc->Combat()->SetWeapon(new Shotgun());
    }
    else if (mod2 == ROCKETS)
    {
      pc->Combat()->SetWeapon(new Missile());
    }
    if (mod1 == EXPLOSIVEROUNDS)
    {
      pc->Combat()->GiveExplosiveBullets();
    }

    if (playerLoadouts[pc->playerNum] & BonusShield)
    {
      pc->Combat()->SetMaxShields(pc->Combat()->GetMaxShields() + BONUS_SHIELD);
      pc->Combat()->SetShields(pc->Combat()->GetMaxShields());

      TRACELOG->Log(TraceLevel::DBG,
        "Gave player %d bonus shields!", pc->playerNum);
    }

    if (playerLoadouts[pc->playerNum] & ExplosiveRounds)
    {
      pc->Combat()->GiveExplosiveBullets();

      TRACELOG->Log(TraceLevel::DBG,
        "Gave player %d explosive bullets!", pc->playerNum);
    }

    if (playerLoadouts[pc->playerNum] & DoubleDamage)
    {
      pc->Combat()->GiveDoubleDamage();

      TRACELOG->Log(TraceLevel::DBG,
        "Gave player %d double damage!", pc->playerNum);
    }
  }

  void Level1_Logic::SpawnPlayers(float dt)
  {
    Transform *playTrans;
    if (startFlag)
    {
      for (int i = 0; i < numOfPlayers; ++i)
      {
        Players[i] = (FACTORY->LoadObjectFromArchetype(space, playerNames[i]))->self;
        GameObject *temp = space->GetGameObject(Players[i]);

        SetupPlayer(temp->GetComponent<PlayerController>(ePlayerController));

        playTrans = space->GetGameObject(Players[i])->GetComponent<Transform>(eTransform);
        playTrans->SetTranslation(spawnPos[i]);

        if (mode == SUDDENDEATH)
        {
          PlayerController* pc = temp->GetComponent
            <PlayerController>(ePlayerController);

          /* @TODO: Redo player spawning */
          //temp->GetComponent<PlayerController>(ePlayerController)->
          //temp->GetComponent<PlayerController>(ePlayerController)->shields = 200;
        }
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
          GameObject *temp = space->GetGameObject(Players[i]);
          playTrans = space->GetGameObject(Players[i])->GetComponent<Transform>(eTransform);
          playTrans->SetTranslation(spawnPos[ranStart]);

          SetupPlayer(temp->GetComponent<PlayerController>(ePlayerController));

          spawnTimers[i] = 2.0f;
          space->GetGameObject(Players[i])->
            GetComponent<PlayerController>(ePlayerController)->hasRespawned = true;
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
    RoundController *RC = space->GetGameObject(owner)->GetComponent<RoundController>(eRoundController);


    if (!GetRandom(0, 3))
    {
      SoundEmitter* se = GetOwner()->GetComponent<SoundEmitter>(eSoundEmitter);

      if (who_killed_him != -1)
      {
        if (GetRandom(0, 1))
          se->Play("crowd_approve00", &SoundInstance(1.0f));
        else
          se->Play("crowd_approve02", &SoundInstance(1.0f));
      }
      else
      {
        if (GetRandom(0, 1))
          se->Play("crowd_disapprove00", &SoundInstance(1.0f));
        else
          se->Play("crowd_disapprove01", &SoundInstance(1.0f));
      }
    }

    Players[ply] = Handle::null;
    if (who_killed_him != -1 && RC->state_ == RoundController::RoundState::ROUNDINPRO)
      playerCoins[who_killed_him] += 200;
    if (juggernaut[ply] == true)
    {
      juggernaut[ply] = false;
      if (who_killed_him != -1)
      {
        juggernaut[who_killed_him] = true;
        GivePlayerCoins(who_killed_him, 750);
        MakeJuggernaut();
      }
      space->hooks.Call("JuggDied", ply, timeAsJugg);
      timeAsJugg = 0;
    }
    else if (juggernaut[who_killed_him] == true)
      ++juggKills[who_killed_him];

    if (!camShake)
    {
      camShakeTime = 0.25f;
      camShakeMagnitude = 20;
      camShake = true;
    }

  }

  void Level1_Logic::SpawnCoinStacks()
  {
    /*for (int i = 0; i < 4; ++i)
    {
    playerCoinStack[i] = (FACTORY->LoadObjectFromArchetype(space, "coin_stack"))->self;
    space->GetGameObject(playerCoinStack[i])->GetComponent<Transform>(eTransform)->SetTranslation(CoinStackPos[i]);
    }*/
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

    camShakeTime -= dt;

    if (camShakeTime <= 0)
    {
      camShake = false;
      lc->SetTranslation(Vec3(0, 0, 0));
    }


  }

  bool Level1_Logic::LevelCountdown(float dt)
  {
    Sprite *ls = space->GetGameObject(owner)->GetComponent<Sprite>(eSprite);
    SoundEmitter *se = space->GetGameObject(owner)->GetComponent<SoundEmitter>(eSoundEmitter);
    //run countdown
    if (countDownTimer <= 3.0f && countDownTimer > 2.0f && !num_spawned[2])
    {
      se->Play("digital_thud", &SoundInstance(1.0f));
      se->Play("digital_thud2", &SoundInstance(1.0f));
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->number = 3;
      round_number->GetComponent<RoundText>(eRoundText)->middleSpeed = 10.0f;
      num_spawned[2] = true;

      //GameObject *round_start = (FACTORY->LoadObjectFromArchetype(space, "roundStartsIn_text"));
      //round_start->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(-1000.0f, 64.0f, 0.0f));

      SpawnModeText();

    }
    else if (countDownTimer <= 2.0f && countDownTimer > 1.0f && !num_spawned[1])
    {
      se->Play("digital_thud", &SoundInstance(1.0f));
      se->Play("digital_thud2", &SoundInstance(1.0f));
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->number = 2;
      round_number->GetComponent<RoundText>(eRoundText)->middleSpeed = 10.0f;
      num_spawned[1] = true;
    }
    else if (countDownTimer <= 1.0f && countDownTimer > 0.0f && !num_spawned[0])
    {
      se->Play("digital_thud", &SoundInstance(1.0f));
      se->Play("digital_thud2", &SoundInstance(1.0f));
      GameObject *round_number = (FACTORY->LoadObjectFromArchetype(space, "round_number"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(1000.0f, -64.0f, 0.0f));
      round_number->GetComponent<RoundText>(eRoundText)->number = 1;
      round_number->GetComponent<RoundText>(eRoundText)->middleSpeed = 10.0f;
      num_spawned[0] = true;
    }


    countDownTimer -= dt;
    if (countDownTimer <= 0)
    {
      countDownDone = true;
      ls->SetTexture("blank.png");
      for (int i = 0; i < 4; ++i)
        num_spawned[i] = false;
      //spawn GO text

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

  int Level1_Logic::GetPlayerCoins(int ply)
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
      playerCoins[i] = 50000;
    space->GetGameObject(owner)->GetComponent<RoundController>(eRoundController)->mode_ = GAMEOVER;
    space->GetGameObject(owner)->GetComponent<ChipController>(eChipController)->playerChips[0] = 99;
    mode = GAMEOVER;
  }

  void Level1_Logic::ResetPlayers()
  {
    for (int i = 0; i < 4; ++i)
    {
      if (Players[i] != Handle::null)
      {
        space->GetGameObject(Players[i])->
          GetComponent<PlayerController>(ePlayerController)->Combat()->Kill(-1);
      }
    }
  }

  void Level1_Logic::ResetSpawnTimers()
  {
    for (int i = 0; i < 4; ++i)
      spawnTimers[i] = 2.0f;
  }

  void Level1_Logic::GivePlayerCoins(int player, int coins)
  {
    playerCoins[player] += coins;
    playerCoinsThisFrame[player] += coins;
  }

  void Level1_Logic::UpdateCoinStacks()
  {
    ////88.8 tall
    ////65.6 wide
    //Transform *coinStack;
    //Vec3 offSet(0.0f, 0.0f, 0.0f);

    //for (int i = 0; i < 4; ++i)
    //{
    //  if (playerCoinStack[i] == Handle::null || playerCoins[i] >= 50000.0f)
    //    continue;

    //  if (!space->GetGameObject(playerCoinStack[i]))
    //  {
    //    playerCoinStack[i] = Handle::null;
    //    continue;
    //  }

    //  offSet.y = 0.0f;
    //  coinStack = space->GetGameObject(playerCoinStack[i])->GetComponent<Transform>(eTransform);
    //  if (i == 0 || i == 1) //player one and two
    //  {
    //    offSet.y += (250.0f / 50000.0f) * (float)playerCoins[i];
    //    coinStack->SetTranslation(CoinStackPos[i] + offSet);
    //  }
    //  else //player three and four
    //  {
    //    offSet.y -= (250.0f / 50000.0f) * (float)playerCoins[i];
    //    coinStack->SetTranslation(CoinStackPos[i] + offSet);
    //  }
    //}
  }

  void Level1_Logic::SpawnItem(const char *itemName, Vec3 pos)
  {
    GameObject *item = (FACTORY->LoadObjectFromArchetype(space, itemName));
    Transform *ItemTransform = item->GetComponent<Transform>(eTransform);
    if (itemName != "CoinPickup" && itemName != "CoinBall")
    {
      BoxCollider *ItemCollider = item->GetComponent<BoxCollider>(eBoxCollider);
      ItemCollider->SetBodyCollisionGroup("NonCollide");
    }
    else
    {
      CircleCollider *ItemCollider = item->GetComponent<CircleCollider>(eCircleCollider);
      if (itemName != "CoinBall")
        ItemCollider->SetBodyCollisionGroup("NonCollide");
      else
        ItemCollider->SetBodyCollisionGroup("Resolve");
    }

    ItemTransform->SetTranslation(pos);
  }

  void Level1_Logic::SpawnItemSet(Vec3 pos)
  {
    int drop = GetRandom(0, 15);
    if (drop == 0 || drop == 1 || drop == 13)
      SpawnItem("AutoPickup", pos);
    else if (drop == 2 || drop == 3 || drop == 9)
      SpawnItem("ShotgunPickup", pos);
    else if (drop == 4 || drop == 5)
      SpawnItem("PowerUpPickup_Damage", pos);
    else if (drop == 6 || drop == 7)
      SpawnItem("PowerUpPickup_Shield", pos);
    else if (drop == 8)
      SpawnItem("PowerUpPickup_Explosive", pos);
    else if (drop == 10 || drop == 11)
      SpawnItem("CoinPickup", pos);
    else if (drop == 12)
      SpawnItem("MissilePickup", pos);
    else if (drop == 14 || drop == 15)
      SpawnItem("CoinBall", pos);


    if (mod1 == BONUS || mod2 == BONUS)
      SpawnCoins(pos);
    if (mod1 == BONUS && mod2 == BONUS)
      SpawnCoins(pos);
  }

  void Level1_Logic::SpawnCoins(Vec3 pos)
  {
    for (int i = 0; i < 4; ++i)
    {
      SpawnItem("CoinPickup", pos);
    }
  }

  void Level1_Logic::SpawnCoinsEx(Vec3 pos, int amount)
  {
    for (int i = 0; i < amount; ++i)
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

      switch (event)
      {
      case 0:
        LE = new LEGrinderBig();
        break;
      case 1:
        LE = new LEAsteroids();
        break;
      case 2:
        LE = new LEPinwheel();
        break;
      default:
        LE = new LEGrinderBig();
        break;
      }

      LE->FireEvent(space->GetHandles().GetAs<GameObject>(owner));

      eventTimer = (float)GetRandom(20, 30);
      warning = false;
      camShakeTime = 6.5f;
      camShakeMagnitude = 4;
      camShake = true;
    }
    else if (eventTimer > 0.0f && eventTimer < 2.0f && warning == false)
    {
      event = GetRandom(0, 2);
      if (event == 0)
        (FACTORY->LoadObjectFromArchetype(space, "warning_text_grinders"))->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(0.0, 0.0, -2.0));
      else if (event == 1)
        (FACTORY->LoadObjectFromArchetype(space, "warning_text_asteroids"))->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(0.0, 0.0, -2.0));
      else if (event == 2)
        (FACTORY->LoadObjectFromArchetype(space, "warning_text_pinwheel"))->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(0.0, 0.0, -2.0));
      warning = true;
      space->GetHandles().GetAs<SoundEmitter>(levelEmitter)->Play("warning");
    }
  }

  void Level1_Logic::SpawnModeText()
  {
    GameObject *round_number;
    switch (mode)
    {
    case LOBBY:

      break;
    case FFA:
      round_number = (FACTORY->LoadObjectFromArchetype(space, "FFA_text"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(-1000.0f, 200.0f, 0.0f));
      break;
    case JUGGERNAUT:
      round_number = (FACTORY->LoadObjectFromArchetype(space, "DeathTag_text"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(-1000.0f, 200.0f, 0.0f));
      break;
    case SUDDENDEATH:
      round_number = (FACTORY->LoadObjectFromArchetype(space, "SuddenDeath_text"));
      round_number->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(-1000.0f, 200.0f, 0.0f));
      break;
    case SLOTMACHINE:

      break;
    case BONUSMODE:

      break;
    }
  }

  void Level1_Logic::GoToGameMode(float dt)
  {
    switch (mode)
    {
    case IDLE_STATE:
      Idle(dt);
      break;
    case LOBBY:
      Lobby(dt);
      break;
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
    case GAMEOVER:
      GameOverMode(dt);
    }
  }

  void Level1_Logic::Idle(float dt)
  {

  }

  void Level1_Logic::Lobby(float dt)
  {
    //if (!lobbySpawned)
    //{
    //  //spawn lobby
    //  lobbyHandle = (FACTORY->LoadObjectFromArchetype(space, "Lobby"))->self;
    //  space->GetGameObject(lobbyHandle)->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(0.0f, 900.0f, 0.0f));
    //  lobbySpawned = true;
    //}

    ////for loop that spawns the players and keeps them spawned while they are joining
    //for (int i = 0; i < 4; ++i)
    //{
    //  if (space->GetGameObject(lobbyHandle)->GetComponent<LobbyController>(eLobbyController)->playerJoined[i] &&
    //    Players[i] == Handle::null)
    //  {
    //    Players[i] = (FACTORY->LoadObjectFromArchetype(space, playerNames[i]))->self;
    //    GameObject *temp = space->GetGameObject(Players[i]);
    //    Transform *playTrans = space->GetGameObject(Players[i])->GetComponent<Transform>(eTransform);
    //    playTrans->SetTranslation(spawnPos[i]);
    //  }
    //}

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
      return;

    spawnTimer -= dt;
    eventTimer -= dt;
    if (spawnTimer <= 0)
    {
      spawnTimer = 3.0f;
      float ranX = GetRandom(-600, 600);
      float ranY = GetRandom(-200, 200);
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
      for (int i = 0; i < 4; ++i)
        juggernaut[i] = false;
      return;
    }

    SpawnPlayers(dt);

    int i;
    for (i = 0; i < 4; ++i)
    {
      if (juggernaut[i] == true)
        break;
    }
    if (i == 4)
      MakeJuggernaut();

    spawnTimer -= dt;
    eventTimer -= dt;
    timeAsJugg += dt;
    space->GetGameObject(owner)->GetComponent<ChipController>(eChipController)->roundTimeAsJugg[i] += dt;
    if (spawnTimer <= 0)
    {
      spawnTimer = 3.0f;
      float ranX = GetRandom(-600, 600);
      float ranY = GetRandom(-200, 200);
      float ranZ = GetRandom(150, 200);
      Vec3 pos(ranX, ranY, ranZ);
      SpawnItemSet(pos);
    }
    juggernautTimer -= dt;
    if (juggernautTimer <= 0)
    {
      juggernautTimer = 1.0f;
      space->hooks.Call("GivePlayerCoins", i, (25 + (10 * juggKills[i])));
    }
    SpawnLevelEvent();
  }

  void Level1_Logic::MakeJuggernaut()
  {
    int i;
    for (i = 0; i < 4; ++i)
    {
      if (juggernaut[i])
        break;
    }
    if (Players[0] == Handle::null && Players[1] == Handle::null && Players[2] == Handle::null && Players[3] == Handle::null)
      return;
    GameObject *juggernaut_;
    if (i == 4) //no one is currently juggernaut
    {
      do
      {
        i = GetRandom(0, 3);
        juggernaut_ = space->GetGameObject(Players[i]);
      } while (juggernaut_ == nullptr);

      juggernaut[i] = true;
    }
    else
      juggernaut_ = space->GetGameObject(Players[i]);

    if (juggernaut_ != nullptr)
    {
      PlayerController *playerController = juggernaut_->GetComponent<PlayerController>(ePlayerController);
      if (playerController->powerUp != nullptr)
        delete playerController->powerUp;
      playerController->powerUp = new DamageBoost();
      playerController->powerUp->Use(juggernaut_);
      playerController->Combat()->AddHealth(50.0f);
      playerController->Combat()->AddShields(50.0f);

      GameObject *effect = (FACTORY->LoadObjectFromArchetype(space, "jugg_effect"));
      effect->GetComponent<JuggernautEffect>(eJuggernautEffect)->pTransform = (space->GetGameObject(Players[i]))->GetComponentHandle(eTransform);
      effect->GetComponent<Transform>(eTransform)->SetTranslation(
        (space->GetGameObject(Players[i]))->GetComponent<Transform>(eTransform)->GetTranslation());
    }
    else
      juggernaut[i] = false;
  }

  void Level1_Logic::ResetJuggernaut()
  {
    for (int i = 0; i < 4; ++i)
    {
      juggKills[i] = 0;
    }
    juggernautTimer = 0.5f;
  }

  void Level1_Logic::SuddenDeathMode(float dt)
  {
    if (!countDownDone)
    {
      LevelCountdown(dt);
      if (countDownDone)
        SpawnPlayers(dt);
      return;
    }
    else
      roundTimer -= dt;

    if (roundTimer <= 0)
      return;

    if (LastManStanding(dt))
    {
      space->GetGameObject(owner)->GetComponent<RoundController>(eRoundController)->round_state_timer = 0;
      return;
    }

    spawnTimer -= dt;
    eventTimer -= dt;
    SpawnLevelEvent();
    if (spawnTimer <= 0)
    {
      spawnTimer = 3.0f;
      float ranX = GetRandom(-600, 600);
      float ranY = GetRandom(-200, 200);
      float ranZ = GetRandom(150, 200);
      Vec3 pos(ranX, ranY, ranZ);
      SpawnItemSet(pos);
    }

  }

  void Level1_Logic::GameOverMode(float dt)
  {

  }

  void Level1_Logic::SlotMachineMode(float dt)
  {
    if (!roundStart)
      return;
    else if (!slotFinished)
    {
      //ResetPlayers();
      if (LE)
      {
        delete LE;
        LE = 0;
      }
      if (mod1 == LIGHTSOUT)
      {
        space->hooks.Call("ToggleLevelLights");
        //BlockLights::EventData ed;
        //ed.overrideDefault = true;
        //ed.settings.color = Vec4(0.2f, 0.8f, 0.2f, 0.8f);
        //ed.settings.fx = BlockLights::NONE;
        //space->hooks.Call("LightingEvent", 0xFFFFFFFF, &ed);
      }
      space->hooks.Call("CallingSM");
      GameObject *SM = (FACTORY->LoadObjectFromArchetype(space, "LevelSlotMachine"));
      space->hooks.Call("SetLightPattern", LightPatternController::SLOTSPIN);
      SM->GetComponent<Transform>(eTransform)->SetTranslation(Vec3(0.0f, 900.0f, 1.0f));
      SM->GetComponent<SlotController>(eSlotController)->roundNum = space->GetGameObject(owner)->GetComponent<RoundController>(eRoundController)->current_round;
      SM->GetComponent<SlotController>(eSlotController)->lastMode = lastMode;
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
      return;

    if (spawnTimer <= 0)
    {
      float ranX = GetRandom(-600, 600);
      float ranY = GetRandom(-200, 200);
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
    lastMode = mode_;
    mode = mode_;
    countDownDone = false;
    countDownTimer = 3.0f;
    roundTimer = space->GetGameObject(owner)->GetComponent<RoundController>(eRoundController)->timeOfRound;
    slotFinished = false;
    startFlag = true;
    ResetSpawnTimers();
    ResetJuggernaut();
  }

  void Level1_Logic::SetMods(GameMods mod1_, GameMods mod2_)
  {
    mod1 = mod1_;
    mod2 = mod2_;
    if (mod1 == LIGHTSOUT)
    {
      space->hooks.Call("ToggleLevelLights");

      //BlockLights::EventData ed;
      //ed.overrideDefault = true;
      //ed.settings.color = Vec4(0.2f, 0.8f, 0.2f, 0.5f);
      //ed.settings.fx = BlockLights::NONE;
      //ed.settings.customData.duration = 60.0f;
      //space->hooks.Call("LightingEvent", 0xFFFFFFFF, &ed);
    }
    if (mod1 == BONUS && mod2 == BONUS)
    {
      mode = BONUSMODE;
      roundTimer = 10.0f;
    }
  }

  bool Level1_Logic::LastManStanding(float dt)
  {
    int deadPlayers = 0;
    int lms = 0;
    for (int i = 0; i < 4; ++i)
    {
      if (Players[i] == Handle::null)
        ++deadPlayers;
      else
      {
        lms = i;
        space->GetGameObject(owner)->GetComponent<ChipController>(eChipController)->LMSTimeAlive[i] += dt;
      }
    }

    if (deadPlayers == 3)
    {
      space->GetGameObject(owner)->GetComponent<ChipController>(eChipController)->LMSThisRound = lms;
      return true;
    }
    else if (deadPlayers > 3) //died at same time
      return true;

    return false;
  }

  void Level1_Logic::RoundOver()
  {
    if (mode == JUGGERNAUT)
    {
      int theJugg;
      for (int i = 0; i < 4; ++i)
      {
        if (juggernaut[i])
        {
          theJugg = i;
          juggernaut[i] = false;
          break;
        }
      }

      space->hooks.Call("JuggDied", theJugg, timeAsJugg);
      timeAsJugg = 0;

    }

    if (LE != nullptr)
    {
      delete LE;
      LE = nullptr;
    }


    /* Reset Player load out*/
    for (int i = 0; i < 4; ++i)
    {
      playerLoadouts[i] = 0;
    }

  }

  void Level1_Logic::Draw()
  {
    Vec3 pos(0.0f, 0.0f, 0.0f);
    Vec2D scale(50, 50);
    char playerCoinsString[10];
    //  for (int i = 0; i < 4; ++i)
    //  {

    //    if (Players[i] == Handle::null)
    //      continue;

    //    if (playerCoinsThisFrame[i] != 0)
    //    {
    //      std::pair<int, float> newCoinString(playerCoinsThisFrame[i], 1.0f);
    //      coinStringsAlive[i].push_back(newCoinString);
    //      playerCoinsThisFrame[i] = 0;
    //    }
    //    if (!coinStringsAlive[i].empty())
    //    {
    //      for (int j = 0; j < coinStringsAlive[i].size(); ++j)
    //      {
    //        itoa(coinStringsAlive[i][j].first, playerCoinsString, 10);
    //        if (Players[i] != Handle::null)
    //          pos = space->GetGameObject(Players[i])->GetComponent<Transform>(eTransform)->GetTranslation();

    //        Draw::SetPosition(pos.x, pos.y + (64 - (coinStringsAlive[i][j].second * 64)));
    //        Draw::SetColor(0.9, 0.9, 0.15f, fontIndex); //yellow-ish color
    //        Draw::SetRotation(0);
    //        Draw::DrawString(playerCoinsString, scale, fontIndex);
    //        coinStringsAlive[i][j].second -= deltaTime;
    //        if (coinStringsAlive[i][j].second <= 0.0f)
    //          coinStringsAlive[i].pop_front();
    //      }
    //    }
    //  }
  }

}
