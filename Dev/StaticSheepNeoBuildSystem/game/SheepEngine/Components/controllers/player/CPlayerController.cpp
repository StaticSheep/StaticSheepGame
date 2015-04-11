/*****************************************************************
Filename: CPlayerController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CPlayerController.h"
#include "types/space/Space.h"
#include "components/colliders/CBoxCollider.h"
#include "types/vectors/Vec3.h"
#include "../../colliders/CCircleCollider.h"
#include "../../sprites/CSprite.h"
#include "types/weapons/WPistol.h"
#include "../../gameplay_scripts/Weapon_Scripts/CBullet_default.h"
#include "../../sprites/CAniSprite.h"
#include "../../gameplay_scripts/CCheats.h"
#include "../systems/input/Input.h"
#include "systems/metrics/MetricInfo.h"
#include "systems/graphics/SheepGraphics.h"
#include "../../gameplay_scripts/Player_Scripts/CAimingArrow.h"
#include "../../gameplay_scripts/Player_Scripts/CDashEffect.h"
#include "../../particles/CParticleSystem.h"



namespace Framework
{
  static int pn = -1;
  static Vec2D aim(1.0f, 0.0f); //default aiming direction
  static GamePad *gp;           //players controller
  static BoxCollider *bc;       //players box collider
  static SoundEmitter *se;      //players sound emitter
  static Transform *ps;         //players transform

  PlayerController::PlayerController() //1
  {
    //set defaults
    playerNum = 0;
    playerGamePad = Handle::null; //this is how you null a handle right
    hasFired = false;
    snappedTo = Handle::null;
    hasRespawned = true;
    stoppedFX = false;
    blink = false;
    GoldenGun = false;
    normals.clear();
    lastRotation = 0.0f;
    frameSkip = false;
    arrowSpawn = false;
    respawnTimer = 3.0f;

  }

  PlayerController::~PlayerController() //4
  {

  }

  void PlayerController::TakeGlobalDamage(float damage, int target)
  {
    if (playerNum == target)
      m_combatController.TakeDamage(damage, -1);
  }


  void PlayerController::Initialize() //2
  {
    m_combatController.Initialize(GetOwner(), playerNum);

    //logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self,
      BUILD_FUNCTION(PlayerController::LogicUpdate));

    space->hooks.Add("DealDamageToPlayer", self,
      BUILD_FUNCTION(PlayerController::TakeGlobalDamage));

    GetOwner()->hooks.Add("PlayerKilled", self,
      BUILD_FUNCTION(PlayerController::PlayerKilled));


    space->GetGameObject(owner)->hooks.Add("OnCollision", self, BUILD_FUNCTION(PlayerController::OnCollision));

    //Generic* gobj = space->GetHandles().GetAs<Generic>(owner);

    playerGamePad = space->GetGameObject(owner)->GetComponentHandle(eGamePad); //gets the handle to the gamepad
    playerCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);
    playerTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    playerSound = space->GetGameObject(owner)->GetComponentHandle(eSoundEmitter);
    playerSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite);
    playerAnimation = space->GetGameObject(owner)->GetComponentHandle(eSpineSprite);

    ps = space->GetHandles().GetAs<Transform>(playerTransform);
    ps->SetScale(Vec3(1.0f, 1.0f, 0.0));

    switch (playerNum)
    {
    case 0: // ninja
      //ps->SetScale(Vec3(1.5f, 1.5f, 0.0f));
      playerColor = Vec4(0.5f, 1.0f, 0.5f, 1.0f);
      break;
    case 1: // ruiser
      playerColor = Vec4(0.75f, 0.15f, 0.1f, 1.0f);
      //ps->SetScale(Vec3(1.0f, 1.0f, 0.0f));
      break;
    case 2: // spacewitch
      playerColor = Vec4(0.6f, 0.25f, 0.6f, 1.0f);
      //ps->SetScale(Vec3(1.25f, 1.25f, 0.0f));
      break;
    case 3: // steve
      playerColor = Vec4(0.4f, 0.78f, 0.78f, 1.0f);
      //ps->SetScale(Vec3(1.0f, 1.0f, 0.0f));
      break;
    }

    //playerColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f);

    gp = space->GetHandles().GetAs<GamePad>(playerGamePad); //actually gets the gamepad
    gp->SetPad(playerNum); //setting pad number

    //intialize the playerWeaponGroup - used for Collisions in physics
    switch (playerNum)
    {
    case 0:
      weaponGroup = "Player1Weapon";
      break;

    case 1:
      weaponGroup = "Player2Weapon";
      break;

    case 2:
      weaponGroup = "Player3Weapon";
      break;

    case 3:
      weaponGroup = "Player4Weapon";
      break;
    }


    aimDir.x = 1;
    aimDir.y = 0;

    bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
    bc->SetGravityOff();
    se = space->GetHandles().GetAs<SoundEmitter>(playerSound);
    se->Play("robot_startup", &SoundInstance(0.50f));
    animCont = AnimationController(playerNum);
    animCont.AnimState = IDLE;
    bc->SetBodyCollisionGroup(space->GetGameObject(owner)->archetype);

    powerUp = nullptr;
    pn = -1;
    //SpawnEffect();

    firstUpdate = true;
	}

  //************************************
  // Method:    LogicUpdate
  // FullName:  Framework::PlayerController::LogicUpdate
  // Access:    public 
  // Returns:   void
  // Qualifier:
  // Parameter: float dt
  //************************************
  void PlayerController::LogicUpdate(float dt)
  {
    //update all the players pointers
    gp = space->GetHandles().GetAs<GamePad>(playerGamePad);
    bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
    se = space->GetHandles().GetAs<SoundEmitter>(playerSound);
    ps = space->GetHandles().GetAs<Transform>(playerTransform);

    if (firstUpdate)
    {
      SpawnEffect();
      firstUpdate = false;
    }
    if (powerUp != nullptr)
    {
      if (powerUp->inUse)
        powerUp->Update(dt);
      else if (powerUp->inUse == false)
      {
        delete powerUp;
        powerUp = nullptr;
      }
    }

    //if the player has just respawned, run the blink function
    if (hasRespawned)
      RespawnBlink(dt);


    if(gp->GetIndex() == 0)
      DebugKeyboardInput();

    if (gp->RStick_InDeadZone() == false)     //if the right stick is NOT inside of its dead zone
    {
      SpawnAimArrow();
    }
    else
      arrowSpawn = false;

    //fire on trigger pull
    if (
      (gp->RightTrigger() && !hasFired)
      ||
      (SHEEPINPUT->KeyIsDown(VK_SPACE)
      && !hasFired && gp->GetIndex() == 0)
      )
    {
      if (Combat()->GetWeapon()->semi)
        hasFired = true;

      onFire();
    }
    else
    {
      if (!gp->RightTrigger())
      {
        hasFired = false;
      }
    }

    //  //check to see if the weapon is semi-auto
    //  if (weapon->semi == false)
    //  {
    //    if (weapon->delay <= 0)
    //    {
    //      hasFired = false;
    //      weapon->ResetDelay();
    //    }
    //  }

    // keyboard input for first player
    if (SHEEPINPUT->KeyIsDown(VK_SPACE))
      SHEEPINPUT->Pads[0].State.Gamepad.bRightTrigger = (BYTE)255;

    ////if the trigger is released, reset the bool
    //if (!gp->RightTrigger() && weapon->semi)
    //  {
    //    if (weapon->delay <= 0)
    //    {
    //      hasFired = false;
    //      
    //    }
    //  }

    



    moveController.Update(GetOwner());
    m_combatController.Update(GetOwner(), dt);

    PlayerButtonPress(); //check to see if the player has pressed any of the controller buttons (for cheats or other things)

    SetAnimations(); //depending on movement or action, set the players current sprite animation

    //this check makes sure the player hasn't gone "OUTSIDE" the level, if they have it kills them
    if (ps->GetTranslation().x > 1000
      || ps->GetTranslation().x < -1000
      || ps->GetTranslation().y > 500
      || ps->GetTranslation().y < -500)
      Combat()->Kill(-1);

    //isSnapped = false;

    //keyboard movement
    if (gp->GetIndex() == 0 && !gp->Connected())
    {
      if ((SHEEPINPUT->KeyIsDown(VK_LEFT)))
      {
        aim += Vec2D(-1.0f, 0.0f);
      }

      if ((SHEEPINPUT->KeyIsDown(VK_RIGHT)))
      {
        aim += Vec2D(1.0f, 0.0f);
      }

      if ((SHEEPINPUT->KeyIsDown(VK_UP)))
      {
        aim += Vec2D(0.0f, 1.0f);
      }

      if ((SHEEPINPUT->KeyIsDown(VK_DOWN)))
      {
        aim += Vec2D(0.0f, -1.0f);
      }

      aim.Normalize();
      SHEEPINPUT->Pads[0].SetRightStick(aim);
      aimDir = Vec3D(aim.x, aim.y, 0.0);
    }

    //if(normals.size() == 0)
    //{
    //  if(snapFrame <= 0)
    //  {
    //    bc->SetGravityOff();
    //    isSnapped = false;
    //  }
    //}

    /*if(snapFrame > 0)
    --snapFrame;

    if(checkJump > 0)
    --checkJump;

    collisionTotal = 0;
    otherObjectVelocity = Vec3();*/

    //MetricInfo metricData;
    //metricData.mt = PLAYER_LOCATION;
    //metricData.playerNum = playerNum;
    //metricData.x = (int)ps->GetTranslation().x;
    //metricData.y = (int)ps->GetTranslation().y;
    //ENGINE->SystemMessage(MetricsMessage(&metricData));

  }

  //************************************
  // Method:    OnCollision
  // FullName:  Framework::PlayerController::OnCollision
  // Access:    public 
  // Returns:   void
  // Qualifier:
  // Parameter: Handle otherObject
  // Parameter: SheepFizz::ExternalManifold manifold
  //************************************
  void PlayerController::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
  {
    //update all the players pointers
    //gp = space->GetHandles().GetAs<GamePad>(playerGamePad);
    bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
    //se = space->GetHandles().GetAs<SoundEmitter>(playerSound);
    //ps = space->GetHandles().GetAs<Transform>(playerTransform);

    GameObject *OtherObject = space->GetHandles().GetAs<GameObject>(otherObject); //get the game object from the handle

    m_combatController.Collision(GetOwner(), OtherObject);

    /*if(OtherObject->name == "WeaponPickup" || OtherObject->archetype == "Grinder" ||
    OtherObject->name == "PowerUpPickup" || OtherObject->name == "CoinPickup" || OtherObject->name == "CoinBall" || OtherObject->name == "Player"
    || OtherObject->HasComponent(eBullet_Default))
    return;*/

    if (moveController.CanSnap())
      moveController.DetermineSnap(bc, OtherObject, manifold);
  }


  //************************************
  // Method:    Remove
  // FullName:  Framework::PlayerController::Remove
  // Access:    public 
  // Returns:   void
  // Qualifier: //3
  //************************************
  void PlayerController::Remove() //3
  {
    if (spawnEffect != Handle::null)
    {
      GameObject* obj = space->GetGameObject(spawnEffect);
      if (obj)
        obj->Destroy();
      spawnEffect = Handle::null;
    }
    //opposite of init
    space->hooks.Remove("LogicUpdate", self);

    Combat()->DeleteWeapon();

    if (powerUp != nullptr)
    {
      delete powerUp; //release dynamic memory
      powerUp = nullptr;
    }
  }

  //************************************
  // Method:    onFire
  // FullName:  Framework::PlayerController::onFire
  // Access:    public 
  // Returns:   void
  // Qualifier:
  //************************************
  void PlayerController::onFire()
  {
    /*if (GodMode == true || GoldenGun == true)
    weapon->damage = 100;*/

    if (m_combatController.GetWeapon()->delay > 0.0f)
      return;

    m_combatController.GetWeapon()
      ->Fire(space->GetHandles().GetAs<GameObject>(owner));

    m_combatController.GetWeapon()->ResetDelay();

    if (!moveController.IsSnapped())
    {
      BoxCollider *bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
      bc->AddToVelocity(-aimDir * (float)(Combat()->GetWeapon()->knockback));
    }
  }

  //************************************
  // Method:    aimingDirection
  // FullName:  Framework::PlayerController::aimingDirection
  // Access:    public 
  // Returns:   Framework::Vec3
  // Qualifier:
  // Parameter: GamePad * gp
  //************************************
  Vec3 PlayerController::aimingDirection(GamePad *gp, char stick)
  {
    Vec3 returnVec;

    if (stick == 'L')
    {
      returnVec.x = gp->LeftStick_X();
      returnVec.y = gp->LeftStick_Y();
    }
    else
    {
      //here I'm grabbing where the right stick's x and y is and saving that to a vector
      returnVec.x = gp->RightStick_X();
      returnVec.y = gp->RightStick_Y();
    }
    //then I normalize that vector and multiply it by a constant (1.5)
    returnVec.Normalize();
    //returnVec *= 1.5;

    return returnVec;
  }

  //************************************
  // Method:    SpawnAimArrow
  // FullName:  Framework::PlayerController::SpawnAimArrow
  // Access:    public 
  // Returns:   void
  // Qualifier:
  //************************************
  void PlayerController::SpawnAimArrow()
  {
    aimDir = aimingDirection(gp); //get the direction the player is currently aiming;
    if (!arrowSpawn)
    {
    //draw aiming arrow
    GameObject *AA = (FACTORY->LoadObjectFromArchetype(space, "AimingArrow"));
    AA->GetComponent<AimingArrow>(eAimingArrow)->playerGamePad = playerGamePad;
    AA->GetComponent<AimingArrow>(eAimingArrow)->playerTransform = playerTransform;
    AA->GetComponent<AimingArrow>(eAimingArrow)->playerController = owner;
    AA->GetComponent<Transform>(eTransform)->SetTranslation(ps->GetTranslation());

    Vec4 color_;
    if (playerNum == 0)
      color_ = Vec4(0.2f, 1.0f, 0.2f, 0.7f);
    else if (playerNum == 1)
      color_ = Vec4(1.0f, 0.2f, 0.2f, 0.7f);
    else if (playerNum == 2)
      color_ = Vec4(1.0f, 0.2f, 1.0f, 0.7f);
    else if (playerNum == 3)
      color_ = Vec4(0.2f, 0.2f, 1.0f, 0.7f);

    AA->GetComponent<Sprite>(eSprite)->Color = color_;
    arrowSpawn = true;
    }
  }


  //************************************
  // Method:    RespawnBlink
  // FullName:  Framework::PlayerController::RespawnBlink
  // Access:    public 
  // Returns:   void
  // Qualifier:
  // Parameter: float dt
  //************************************
  void PlayerController::RespawnBlink(float dt)
  {
    SpineSprite *pa = space->GetHandles().GetAs<SpineSprite>(playerAnimation);
    Transform *effectTrans;

    if (respawnTimer < 0.0f)
    {
      if (spawnEffect != Handle::null)
      {
        space->GetGameObject(spawnEffect)->Destroy();
        spawnEffect = Handle::null;

        pa->Color.A = 1.0f;

        hasRespawned = false;
      }
    }

    if (respawnTimer > 0.0f)
    {
      if (hasRespawned)
      {
        if (!blink)
          pa->Color.A -= dt * 10.0f;

        else
          pa->Color.A += dt * 10.0f;

        respawnTimer -= dt;

        if (pa->Color.A <= 0.5f)
          blink = true;

        if (pa->Color.A >= 1.0f)
          blink = false;
      }

      if (spawnEffect != Handle::null)
      {
        effectTrans = space->GetGameObject(spawnEffect)->GetComponent<Transform>(eTransform);
        effectTrans->SetTranslation(ps->GetTranslation());

        if (respawnTimer < 1.5f && !stoppedFX)
        {
          ParticleCircleEmitter* psys = space->GetGameObject(spawnEffect)->
            GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter);
          if (psys)
            psys->Toggle(false);
          stoppedFX = true;
        }
      }
    }

  }




  void PlayerController::PlayerKilled(int attacker)
  {
    Handle explosion = (FACTORY->LoadObjectFromArchetype(space, "explosion"))->self;
    Transform *exT = space->GetGameObject(explosion)->GetComponent<Transform>(eTransform);
    exT->SetTranslation(ps->GetTranslation());
    exT->SetRotation((float)GetRandom(0, (int)(2.0f * (float)PI)));

    space->hooks.Call("SpawnCoins", ps->GetTranslation());

    space->GetGameObject(owner)->Destroy();
  }

  //************************************
  // Method:    SetAnimations
  // FullName:  Framework::PlayerController::SetAnimations
  // Access:    public 
  // Returns:   void
  // Qualifier:
  //************************************
  void PlayerController::SetAnimations()
  {
    GamePad *gp = space->GetHandles().GetAs<GamePad>(playerGamePad);
    //get animated sprite component
    SpineSprite *pa = space->GetHandles().GetAs<SpineSprite>(playerAnimation);

    if ((moveController.IsSnapped() && !(gp->LStick_InDeadZone())) || (moveController.IsSnapped() && gp->GetIndex() == 0 && (SHEEPINPUT->KeyIsDown('D') || SHEEPINPUT->KeyIsDown('A') || SHEEPINPUT->KeyIsDown('W') || SHEEPINPUT->KeyIsDown('S'))))
    {
      //set animated sprite to run
      if (animCont.AnimState != RUN)
      {
        //pa->SetRange(Vec2((float)animCont.run.beginFrame, (float)animCont.run.endFrame));
        animCont.AnimState = RUN;
      }
    }
    else if (!moveController.IsSnapped())
    {
      if (animCont.AnimState != JUMP)
      {
        //set animated sprite to jump
        //pa->SetRange(Vec2((float)animCont.jump.beginFrame, (float)animCont.jump.endFrame));
        animCont.AnimState = JUMP;
      }
    }
    else
    {
      if (animCont.AnimState != IDLE)
      {
        //set animated sprite to idle
        //pa->SetRange(Vec2((float)animCont.idle.beginFrame, (float)animCont.idle.endFrame));
        animCont.AnimState = IDLE;
      }
    }

    Transform* trans = space->GetHandles().GetAs<Transform>(playerTransform);

    Vec3 moveDir(0.0f, 0.0f, 1.0f);

    if(!gp->LStick_InDeadZone())
      moveDir = Vec3(gp->LeftStick_X(), gp->LeftStick_Y(), 0.0f);

    animCont.Update(pa, playerColor, trans->GetRotation(), moveDir, aimDir, arrowSpawn);

  }

  //************************************
  // Method:    PlayerButtonPress
  // FullName:  Framework::PlayerController::PlayerButtonPress
  // Access:    public 
  // Returns:   void
  // Qualifier:
  //************************************
  void PlayerController::PlayerButtonPress()
  {
    MetricInfo playerButton;
    playerButton.mt = PLAYER_BUTTON_PRESS;
    playerButton.playerNum = playerNum;
    playerButton.x = (int)ps->GetTranslation().x;
    playerButton.y = (int)ps->GetTranslation().y;
    playerButton.button = Buttons::NONE;


    bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
    Transform* trans = space->GetHandles().GetAs<Transform>(playerTransform);

    if (gp->ButtonPressed(XButtons.A))
    {
      playerButton.button = Buttons::A;
    }
    if (gp->ButtonPressed(XButtons.B))
    {
      playerButton.button = Buttons::B;
    }
    if (gp->ButtonPressed(XButtons.X))
    {
      //bc->AddToAngVelocity(.5f);
      space->GetGameObject(owner)->hooks.Call("ButtonPressed", Buttons::X);
      playerButton.button = Buttons::X;
    }
    if (gp->ButtonPressed(XButtons.Y))
    {
      space->GetGameObject(owner)->hooks.Call("ButtonPressed", Buttons::Y);
      playerButton.button = Buttons::Y;
    }
    if (gp->ButtonPressed(XButtons.DPad.Up))
    {
      space->GetGameObject(owner)->hooks.Call("ButtonPressed", Buttons::UP);
      playerButton.button = Buttons::UP;
    }
    if (gp->ButtonPressed(XButtons.DPad.Down))
    {
      space->GetGameObject(owner)->hooks.Call("ButtonPressed", Buttons::DOWN);
      playerButton.button = Buttons::DOWN;
    }
    if (gp->ButtonPressed(XButtons.DPad.Left))
    {
      space->GetGameObject(owner)->hooks.Call("ButtonPressed", Buttons::LEFT);
      playerButton.button = Buttons::LEFT;
    }
    if (gp->ButtonPressed(XButtons.DPad.Right))
    {
      space->GetGameObject(owner)->hooks.Call("ButtonPressed", Buttons::RIGHT);
      playerButton.button = Buttons::RIGHT;
    }

    if (SHEEPINPUT->Keyboard.KeyIsPressed('V'))
    {
      Handle explosion = (FACTORY->LoadObjectFromArchetype(space, "explosion"))->self;
      Transform *exT = space->GetGameObject(explosion)->GetComponent<Transform>(eTransform);
      exT->SetTranslation(ps->GetTranslation());
      exT->SetRotation((float)GetRandom(0, (int)(2.0f * (float)PI)));
    }

    ENGINE->SystemMessage(MetricsMessage(&playerButton));
  }

  //************************************
  // Method:    CurrentHealth
  // FullName:  Framework::PlayerController::CurrentHealth
  // Access:    public 
  // Returns:   int
  // Qualifier:
  //************************************
  int PlayerController::CurrentHealth()
  {
    return m_combatController.GetHealth();
  }

  //void PlayerController::DealDamage(float damage, int playerNum_)
  //{
  //  if (playerNum != playerNum_)
  //    return;

  //  if ((shields - damage) < 0) //if the damage would do more than we have shields
  //  {
  //    float leftOver = damage - shields;
  //    shields = 0;
  //    health -= leftOver;
  //  }
  //  else
  //    shields -= damage;

  //}

  void PlayerController::SpawnEffect()
  {
    Transform *effectTrans;
    //ParticleSystem* ps;


    switch (playerNum)
    {
    case 0:
      spawnEffect = (FACTORY->LoadObjectFromArchetype(space, "p1_spawnEffect"))->self;
      break;
    case 1:
      spawnEffect = (FACTORY->LoadObjectFromArchetype(space, "p2_spawnEffect"))->self;
      break;
    case 2:
      spawnEffect = (FACTORY->LoadObjectFromArchetype(space, "p3_spawnEffect"))->self;
      break;
    case 3:
      spawnEffect = (FACTORY->LoadObjectFromArchetype(space, "p4_spawnEffect"))->self;
      break;
    }

    effectTrans = space->GetGameObject(spawnEffect)->GetComponent<Transform>(eTransform);
    effectTrans->SetTranslation(ps->GetTranslation());

    stoppedFX = false;
  }

  void PlayerController::DebugKeyboardInput()
  {
    // left
    if(SHEEPINPUT->KeyIsDown('A'))
    {
      SHEEPINPUT->Pads[0].State.Gamepad.sThumbLX = -32767;
    }
    else // right
    if(SHEEPINPUT->KeyIsDown('D'))
    {
      SHEEPINPUT->Pads[0].State.Gamepad.sThumbLX = 32767;
    }

    // up
    if(SHEEPINPUT->KeyIsDown('W'))
    {
      SHEEPINPUT->Pads[0].State.Gamepad.sThumbLY = 32767;
    }
    else // down
    if(SHEEPINPUT->KeyIsDown('S'))
    {
      SHEEPINPUT->Pads[0].State.Gamepad.sThumbLY = -32767;
    }

    // jump
    if(SHEEPINPUT->KeyIsPressed('Q'))
    {
      SHEEPINPUT->Pads[0].State.Gamepad.wButtons = XINPUT_GAMEPAD_A;
    }


    if(SHEEPINPUT->KeyIsDown(VK_LEFT))
    {
      SHEEPINPUT->Pads[0].State.Gamepad.sThumbRX = -32767;
    }
    else
    if(SHEEPINPUT->KeyIsDown(VK_RIGHT))
    {
      SHEEPINPUT->Pads[0].State.Gamepad.sThumbRX = 32767;
    }

    if(SHEEPINPUT->KeyIsDown(VK_UP))
    {
      SHEEPINPUT->Pads[0].State.Gamepad.sThumbRY = 32767;
    }
    else
    if(SHEEPINPUT->KeyIsDown(VK_DOWN))
    {
      SHEEPINPUT->Pads[0].State.Gamepad.sThumbRY = -32767;
    }
  }
}


