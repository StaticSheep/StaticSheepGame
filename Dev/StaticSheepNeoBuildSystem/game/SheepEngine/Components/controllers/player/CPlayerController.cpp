/*****************************************************************
Filename: CPlayerController.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
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
		isSnapped = true;
		hasFired = false;
    health = 100;
    shields = 100;
    snappedTo = Handle::null;
    respawnTimer = 3.0f;
    hasRespawned = true;
    stoppedFX = false;
    blink = false;
    weapon = nullptr;
    GodMode = false;
    GoldenGun = false;
    PerfectMachine = false;
    normals.clear();
    lastRotation = 0.0f;
    frameSkip = false;
    arrowSpawn = false;
    hasDashed = false;
    firstUpdate = true;
	}

	PlayerController::~PlayerController() //4
	{

	}


	//************************************
	// Method:    Initialize
	// FullName:  Framework::PlayerController::Initialize
	// Access:    public 
	// Returns:   void
	// Qualifier: //2
	//************************************
	void PlayerController::Initialize() //2
	{
		//logic setup, you're attached and components are in place
		space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(PlayerController::LogicUpdate));
    space->hooks.Add("DealDamageToPlayer", self, BUILD_FUNCTION(PlayerController::DealDamage));
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

    switch(playerNum)
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

    weapon = (Pistol*)GET_TYPE(Pistol)->New();
    se = space->GetHandles().GetAs<SoundEmitter>(playerSound);
    se->Play("robot_startup", &SoundInstance(0.50f));
    animCont = AnimationController(playerNum);
    animCont.AnimState = IDLE;
    bc->SetBodyCollisionGroup(space->GetGameObject(owner)->archetype);

    powerUp = nullptr;
    pn = -1;
    //SpawnEffect();
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
    //if the player is out of health run the player death function
    if (health <= 0)
    {
      MetricInfo metricData(playerNum, (int)ps->GetTranslation().x, (int)ps->GetTranslation().y, PLAYER_DEATH, Buttons::NONE, Weapons::PISTOL);
      ENGINE->SystemMessage(MetricsMessage(&metricData));
      PlayerDeath(se, ps, pn);
    }

    //if the player has just respawned, run the blink function
    if (hasRespawned)
      RespawnBlink(dt);

    if (gp->RStick_InDeadZone() == false)     //if the right stick is NOT inside of its dead zone
    {
      SpawnAimArrow();
    }
    else
      arrowSpawn = false;
    
    //update the weapons delay
    weapon->DelayUpdate(dt);

		//fire on trigger pull
		if ((gp->RightTrigger() && hasFired == false) || (SHEEPINPUT->KeyIsDown(VK_SPACE) && hasFired == false && gp->GetIndex() == 0))
		{
			hasFired = true;
			onFire();
		}

    //check to see if the weapon is semi-auto
    if (weapon->semi == false)
    {
      if (weapon->delay <= 0)
      {
        hasFired = false;
        weapon->ResetDelay();
      }
    }

    // keyboard input for first player
    if(SHEEPINPUT->KeyIsDown(VK_SPACE))
        SHEEPINPUT->Pads[0].State.Gamepad.bRightTrigger = (BYTE)255;

		//if the trigger is released, reset the bool
		if (!gp->RightTrigger() && weapon->semi)
    {
      if (weapon->delay <= 0)
      {
        hasFired = false;
        weapon->ResetDelay();
      }
    }

    if (isSnapped)
    {
      SnappedMovement();
    }
    else
    {
      normals.clear();
    }

		//dash, formally known as melee
    if (gp->ButtonPressed(XButtons.LShoulder))
      Melee(Buttons::LB);
    else if (gp->ButtonPressed(XButtons.RShoulder))
      Melee(Buttons::RB);

    PlayerButtonPress(); //check to see if the player has pressed any of the controller buttons (for cheats or other things)

    SetAnimations(); //depending on movement or action, set the players current sprite animation

    //this check makes sure the player hasn't gone "OUTSIDE" the level, if they have it kills them
    if (ps->GetTranslation().x > 1000 || ps->GetTranslation().x < -1000 || ps->GetTranslation().y > 500 || ps->GetTranslation().y < -500)
      PlayerDeath(se, ps);

    isSnapped = false;
    
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

    //MetricInfo metricData;
    //metricData.mt = PLAYER_LOCATION;
    //metricData.playerNum = playerNum;
    //metricData.x = (int)ps->GetTranslation().x;
    //metricData.y = (int)ps->GetTranslation().y;
    //ENGINE->SystemMessage(MetricsMessage(&metricData));

	}

  void PlayerController::SnappedMovement()
  {
    hasDashed = false;
    if (frameSkip)
    {
      bc->SetBodyRotation(-snappedNormal);
      normals.clear();
      normals.push_back(snappedNormal);
    }
    frameSkip = !frameSkip;

    bc->SetVelocity(snappedNormal * 100); //artificial pull or "gravity" to snapped normal
    bc->SetAngVelocity(0.0);              //if snapped to surface take away all angular velocity

    if (snappedTo != Handle::null) //if the object we're supposed to be snapped to isn't dead
    {
      GameObject *snappedObject = space->GetHandles().GetAs<GameObject>(snappedTo);
      if ((snappedObject->name == "SmallPlatform" || snappedObject->name == "SmallPlat"))
      {
        Vec3 addedVel = (snappedObject->GetComponent<BoxCollider>(eBoxCollider))->GetCurrentVelocity();
        bc->AddToVelocity(addedVel);
      }
    }

    //left stick movement in the X
    if (gp->LeftStick_X() > 0.2 || (SHEEPINPUT->KeyIsDown(0x44) && gp->GetIndex() == 0))
    {
      if (snappedNormal.y > 0)
        bc->AddToVelocity((snappedNormal.CalculateNormal() * 450));
      else if (snappedNormal.y < 0)
        bc->AddToVelocity(-(snappedNormal.CalculateNormal() * 450));
    }
    else if (gp->LeftStick_X() < -0.2 || (SHEEPINPUT->KeyIsDown(0x41) && gp->GetIndex() == 0))
    {
      if (snappedNormal.y > 0)
        bc->AddToVelocity(-(snappedNormal.CalculateNormal() * 450));
      if (snappedNormal.y < 0)
        bc->AddToVelocity((snappedNormal.CalculateNormal() * 450));
    }

    //left stick movement in the Y
    if (gp->LeftStick_Y() > 0.2 || (SHEEPINPUT->KeyIsDown(0x57) && gp->GetIndex() == 0))
    {
      if (snappedNormal.x > 0)
        bc->AddToVelocity(-(snappedNormal.CalculateNormal() * 450));
      else if (snappedNormal.x < 0)
        bc->AddToVelocity((snappedNormal.CalculateNormal() * 450));
    }
    else if (gp->LeftStick_Y() < -0.2 || (SHEEPINPUT->KeyIsDown(0x53) && gp->GetIndex() == 0))
    {
      if (snappedNormal.x > 0)
        bc->AddToVelocity((snappedNormal.CalculateNormal() * 450));
      if (snappedNormal.x < 0)
        bc->AddToVelocity(-(snappedNormal.CalculateNormal() * 450));
    }

    //clamp the players velocity
    clampVelocity(450.0f);

    //jump
    if (((gp->ButtonDown(XButtons.A) || gp->LeftTrigger()) && isSnapped) || (SHEEPINPUT->KeyIsDown('Q') && gp->GetIndex() == 0))
    {
      jump(); //player jump
      if (GetRandom(0, 1)) //determine sound for jump
        se->Play("jump2", &SoundInstance(0.75f));
      else
        se->Play("jump1", &SoundInstance(0.75f));
    }
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
    gp = space->GetHandles().GetAs<GamePad>(playerGamePad);
    bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
    se = space->GetHandles().GetAs<SoundEmitter>(playerSound);
    ps = space->GetHandles().GetAs<Transform>(playerTransform);
    GameObject *OtherObject = space->GetHandles().GetAs<GameObject>(otherObject); //get the game object from the handle
    
    CollisionDamage(OtherObject); //determine if the colliding object does damage to the player

    DetermineSnap(OtherObject, otherObject,manifold); //determine the snapped normal based on collided object
	}


  void PlayerController::CollisionDamage(GameObject *OtherObject)
  {

    Bullet_Default* bullet = OtherObject->GetComponent<Bullet_Default>(eBullet_Default);

    if (bullet && !hasRespawned && !GodMode && !PerfectMachine)
    {
      DealDamage(bullet->damage, playerNum);
      float randomX = (float)GetRandom(-25, 25);
      float randomY = (float)GetRandom(-25, 25);
      se->Play("hit1", &SoundInstance(1.0f));
      ps = space->GetHandles().GetAs<Transform>(playerTransform);
      Handle hit = (FACTORY->LoadObjectFromArchetype(space, "hit"))->self;
      Transform *exT = space->GetGameObject(hit)->GetComponent<Transform>(eTransform);
      exT->SetTranslation(ps->GetTranslation() + Vec3(randomX, randomY, -1.0f));

      //for metrics, need to determine where the bullet came from by checking its collision group
      if (health <= 0 && OtherObject->GetComponent<CircleCollider>(eCircleCollider) != nullptr)
      {
        pn = -1;

        CircleCollider* cc = OtherObject->GetComponent<CircleCollider>(eCircleCollider);

        if (!cc)
          return;

        if (cc->GetBodyCollisionGroup() == "Player1Weapon")
          pn = 0;
        else if (cc->GetBodyCollisionGroup() == "Player2Weapon")
          pn = 1;
        else if (cc->GetBodyCollisionGroup() == "Player3Weapon")
          pn = 2;
        else if (cc->GetBodyCollisionGroup() == "Player4Weapon")
          pn = 3;

        MetricInfo metricData(pn, 0, 0, PLAYER_KILL, Buttons::NONE, Weapons::PISTOL);
        ENGINE->SystemMessage(MetricsMessage(&metricData));
      }
      return;
    }

    else if ((OtherObject->archetype == "KillBox" || OtherObject->archetype == "KillBoxBig" || OtherObject->name == "GrinderBig")
      && !hasRespawned && !GodMode && !PerfectMachine)
    {
      isSnapped = false;
      DealDamage(5, playerNum);
    }
    else if (OtherObject->name == "WeaponPickup")
      se->Play("weapon_pickup", &SoundInstance(0.75f));

    else if (OtherObject->name == "Asteroid")
    {
      DealDamage(50, playerNum);
      float ranY = (float)GetRandom(-500, 500);
      float ranX = (float)GetRandom(-500, 500);
      bc->AddToVelocity(Vec3(ranX, ranY, 0.0f));
    }

    if ((OtherObject->GetComponentHandle(eGrinder) != Handle::null)
      && !hasRespawned && !GodMode && !PerfectMachine)
    {
      isSnapped = false;
      DealDamage(5, playerNum);
    }

  }

  void PlayerController::DetermineSnap(GameObject *OtherObject, Handle otherObject, SheepFizz::ExternalManifold manifold)
  {
    //get the transform of the thing we are colliding with
    Transform *OOT = OtherObject->GetComponent<Transform>(eTransform);
    //if that thing we collided with's transform is missing, get the fuck outta here, i mean what are you even doing?
    if (!OOT)
      return;

    if (OtherObject->HasComponent(eBoxCollider) && OtherObject->name != "Player" && OtherObject->name != "WeaponPickup"
      && OtherObject->archetype != "Grinder" && OtherObject->name != "PowerUpPickup")
    {
      float dotNormals;
      Vec3 nextSnappedNormal;
      Vec3 oldSnappedNormal = snappedNormal;
      BoxCollider *OOBc = OtherObject->GetComponent<BoxCollider>(eBoxCollider);

      //bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
      //ps = space->GetHandles().GetAs<Transform>(playerTransform);

      if (oldSnappedNormal.x != OOBc->GetCollisionNormals(manifold).x && oldSnappedNormal.y != OOBc->GetCollisionNormals(manifold).y &&
        snappedNormal.x != OOBc->GetCollisionNormals(manifold).x && snappedNormal.y != OOBc->GetCollisionNormals(manifold).y)
      {
        nextSnappedNormal = OOBc->GetCollisionNormals(manifold);
        nextSnappedNormal.Rotate((float)PI / 2);
        dotNormals = snappedNormal.DotProduct(nextSnappedNormal);
        if (dotNormals > 0)
          bc->AddToVelocity((nextSnappedNormal * 10));
        else
          bc->AddToVelocity(-(nextSnappedNormal * 10));
      }
      snappedNormal = OOBc->GetCollisionNormals(manifold);

      //i have to set up a bool flag here for finding a matching vector
      bool dublicate = false;
      for (unsigned i = 0; i < normals.size(); ++i)
      {
        if (snappedNormal.x == normals[i].x && snappedNormal.y == normals[i].y)
          dublicate = true;
      }
      if (dublicate == false)
        normals.push_back(Vec3(snappedNormal));

      if (normals.size() == 0)
        normals.push_back(Vec3(snappedNormal));

      isSnapped = true;
      //get the thing we are colliding with
      snappedTo = otherObject;
      float avX = 0, avY = 0;
      for (unsigned i = 0; i < normals.size(); ++i)
      {
        avX += normals[i].x;
        avY += normals[i].y;
      }
      if (normals.size() != 0)
      {
        avX /= normals.size();
        avY /= normals.size();
        Vec3 averaged(avX, avY, 0.0f);
        snappedNormal = averaged;
      }
    }
    //else if (OtherObject->HasComponent(eCircleCollider) && OtherObject->archetype != "CoinPickup")
    //{
    //  CircleCollider *OOCc = OtherObject->GetComponent<CircleCollider>(eCircleCollider);
    //  snappedNormal = OOCc->GetCollisionNormals(manifold);
    //}
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

    if (weapon != nullptr)
    {
      delete weapon; //release dynamic memory
      weapon = nullptr;
    }
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
    if (GodMode == true || GoldenGun == true)
      weapon->damage = 100;

    weapon->Fire(space->GetHandles().GetAs<GameObject>(owner));

    if (!isSnapped)
    {
      BoxCollider *bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
      bc->AddToVelocity(-aimDir * (float)(weapon->knockback));
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
    returnVec *= 1.5;

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

    if(!arrowSpawn)
      arrowSpawn = true;

    /*
    if (!arrowSpawn)
    {
      //draw aiming arrow
      GameObject *AA = (FACTORY->LoadObjectFromArchetype(space, "AimingArrow"));
      AA->GetComponent<AimingArrow>(eAimingArrow)->playerGamePad = playerGamePad;
      AA->GetComponent<AimingArrow>(eAimingArrow)->playerTransform = playerTransform;
      AA->GetComponent<Transform>(eTransform)->SetTranslation(ps->GetTranslation());

      AniSprite *playerS = space->GetHandles().GetAs<AniSprite>(playerAnimation); //get the player's ani-sprite
      AA->GetComponent<Sprite>(eSprite)->Color = playerS->Color; //set the colors equal
      AA->GetComponent<Sprite>(eSprite)->Color.a = 0.7f; //make sure the alpha isn't low (happens during respawn)
      arrowSpawn = true;
    }*/
  }

  //************************************
  // Method:    Melee
  // FullName:  Framework::PlayerController::Melee
  // Access:    public 
  // Returns:   void
  // Qualifier:
  //************************************
  void PlayerController::Melee(Buttons butt)
  {
    if (hasDashed || isSnapped)
      return;

    //zero out all the velocity the player has
    bc->SetVelocity(Vec3(0.0f, 0.0f, 0.0f));
    if (gp->LStick_InDeadZone())
    {
      if (butt == Buttons::LB)
        bc->SetVelocity(Vec3(-1000.0f, 0.0f, 0.0f));
      else if (butt == Buttons::RB)
        bc->SetVelocity(Vec3(1000.0f, 0.0f, 0.0f));
    }
    else
      bc->SetVelocity(aimingDirection(gp, 'L') * 1000);

    GameObject *dash_effect = (FACTORY->LoadObjectFromArchetype(space, "fire_effect1"));
    dash_effect->GetComponent<DashEffect>(eDashEffect)->pTransform = playerTransform;
    dash_effect->GetComponent<Transform>(eTransform)->SetTranslation(ps->GetTranslation());
    se->Play("dash", &SoundInstance(1.0f));
    hasDashed = true;
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

  //************************************
  // Method:    PlayerDeath
  // FullName:  Framework::PlayerController::PlayerDeath
  // Access:    public 
  // Returns:   void
  // Qualifier:
  // Parameter: SoundEmitter * se
  // Parameter: Transform * ps
  //************************************
  void PlayerController::PlayerDeath(SoundEmitter *se, Transform *ps, int who_killed_me)
  {
    se->Play("death_explosion", &SoundInstance(1.0f));
    Handle explosion = (FACTORY->LoadObjectFromArchetype(space, "explosion"))->self;
    Transform *exT = space->GetGameObject(explosion)->GetComponent<Transform>(eTransform);
    exT->SetTranslation(ps->GetTranslation());
    exT->SetRotation((float)GetRandom(0, (int)(2.0f * (float)PI)));
    space->hooks.Call("SpawnCoins", ps->GetTranslation());
    space->hooks.Call("PlayerDied", playerNum, who_killed_me); //calling an event called player died
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

    if ((isSnapped && !(gp->LStick_InDeadZone())) || (isSnapped && gp->GetIndex() == 0 && (SHEEPINPUT->KeyIsDown('D') || SHEEPINPUT->KeyIsDown('A') || SHEEPINPUT->KeyIsDown('W') || SHEEPINPUT->KeyIsDown('S'))))
    {
      //set animated sprite to run
      if (animCont.AnimState != RUN)
      {
        //pa->SetRange(Vec2((float)animCont.run.beginFrame, (float)animCont.run.endFrame));
        animCont.AnimState = RUN;
      }
    }
    else if (!isSnapped)
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
    
    animCont.Update(pa, playerColor, trans->GetRotation(), aimDir, arrowSpawn);

  }

  //Takes a players box collider and clamps the velocity of that box collider
  //to be from the value of clamp to the negative value of clamp
  //************************************
  // Method:    clampVelocity
  // FullName:  Framework::PlayerController::clampVelocity
  // Access:    public 
  // Returns:   void
  // Qualifier:
  // Parameter: float clamp
  //************************************
  void PlayerController::clampVelocity(float clamp)
  {
    if (bc->GetCurrentVelocity().x > clamp)
      bc->SetVelocity(Vec3(clamp, bc->GetCurrentVelocity().y, 0.0f));
    if (bc->GetCurrentVelocity().x < -clamp)
      bc->SetVelocity(Vec3(-clamp, bc->GetCurrentVelocity().y, 0.0f));
    if (bc->GetCurrentVelocity().y > 450)
      bc->SetVelocity(Vec3(bc->GetCurrentVelocity().x, clamp, 0.0f));
    if (bc->GetCurrentVelocity().y < -clamp)
      bc->SetVelocity(Vec3(bc->GetCurrentVelocity().x, -clamp, 0.0f));
  }

  //************************************
  // Method:    jump
  // FullName:  Framework::PlayerController::jump
  // Access:    public 
  // Returns:   void
  // Qualifier:
  //************************************
  void PlayerController::jump()
  {
    Vec3 jmpDir;
    if (gp->LStick_InDeadZone())
      jmpDir = -snappedNormal;
    else
    {
      jmpDir = aimingDirection(gp, 'L');
      if (-snappedNormal * jmpDir < 0)
      {
        jmpDir += ((-snappedNormal * jmpDir) * snappedNormal) *2;
      }
    }

    bc->AddToVelocity(jmpDir * 500);
    isSnapped = false;
    normals.clear();
    
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
    return health;
  }

  void PlayerController::DealDamage(float damage, int playerNum_)
  {
    if (playerNum != playerNum_)
      return;

    if ((shields - damage) < 0) //if the damage would do more than we have shields
    {
      float leftOver = damage - shields;
      shields = 0;
      health -= leftOver;
    }
    else
      shields -= damage;

  }

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
}