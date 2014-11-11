#include "pch/precompiled.h"
#include "CPlayerController.h"
#include "types/space/Space.h"
#include "components/colliders/CBoxCollider.h"
#include "types/vectors/Vec3.h"
#include "../../colliders/CCircleCollider.h"
#include "../../sprites/CSprite.h"
#include "types/weapons/WPistol.h"
#include "../../gameplay_scripts/CBullet_default.h"

namespace Framework
{
  
	PlayerController::PlayerController() //1
	{
		//set defaults
		playerNum = 0;
		playerGamePad = Handle::null; //this is how you null a handle right
		isSnapped = true;
		hasFired = false;
    health = 100;
    snappedTo = Handle::null;
    respawnTimer = 2.0f;
    hasRespawned = false;
    blink = false;
    weapon = nullptr;

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
		space->GetGameObject(owner)->hooks.Add("OnCollision", self, BUILD_FUNCTION(PlayerController::OnCollision));

    //Generic* gobj = space->GetHandles().GetAs<Generic>(owner);

		playerGamePad = space->GetGameObject(owner)->GetComponentHandle(eGamePad); //gets the handle to the gamepad
		playerCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);
		playerTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    playerSound = space->GetGameObject(owner)->GetComponentHandle(eSoundEmitter);
    playerSprite = space->GetGameObject(owner)->GetComponentHandle(eSprite);

		GamePad *gp = space->GetHandles().GetAs<GamePad>(playerGamePad); //actually gets the gamepad
		gp->SetPad(playerNum); //setting pad number

		aimDir.x = 1;
		aimDir.y = 0;

    BoxCollider *bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
    bc->SetGravityOff();
    weapon = (Pistol*)GET_TYPE(Pistol)->New();
    shotDelay = weapon->delay;
    SoundEmitter *se = space->GetHandles().GetAs<SoundEmitter>(playerSound);
    se->Play("robot_startup", &SoundInstance(0.25f));
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
		//get the game pad
		GamePad *gp = space->GetHandles().GetAs<GamePad>(playerGamePad);
		//get the box collider of player
		BoxCollider *bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
    SoundEmitter *se = space->GetHandles().GetAs<SoundEmitter>(playerSound);
    Transform *ps = space->GetHandles().GetAs<Transform>(playerTransform);

    if (health <= 0)
      PlayerDeath(se, ps);

    if (hasRespawned)
      RespawnBlink(dt);

		if (gp->RStick_InDeadZone() == false)       //if the right stick is NOT inside of its dead zone
			aimDir = aimingDirection(gp); //get the direction the player is currently aiming;

		//fire on trigger pull
		if (gp->RightTrigger() && hasFired == false)
		{
			hasFired = true;
			onFire();
		}
    else
    {
      --shotDelay;
      if(shotDelay < 0)
      {
        hasFired = false;
        shotDelay = weapon->delay;
      }
    }
		//if the trigger is released, reset the bool
		if (!gp->RightTrigger())
    {
			hasFired = false;
      shotDelay = weapon->delay;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////
    if (isSnapped)
    {
      bc->SetVelocity(snappedNormal * 100);
      bc->SetAngVelocity(0.0);
      if (snappedTo != Handle::null)
      {
        //snappedObject->Get(BoxCollider); => snappedObject->GetComponent<BoxCollider>(eBoxCollider);
        GameObject *snappedObject = space->GetHandles().GetAs<GameObject>(snappedTo);
        if ((snappedObject->name == "SmallPlatform" || snappedObject->name == "SmallPlat"))
        {
          Vec3 addedVel = (snappedObject->GetComponent<BoxCollider>(eBoxCollider))->GetCurrentVelocity();
          bc->AddToVelocity(addedVel);
        }
      }
      //left stick move
      if (gp->LeftStick_X() > 0.2 && snappedNormal.x == 0)
      {
        //bc->SetVelocity(Vec3(0.0f, 0.0f, 0.0f));
        if (snappedNormal.y > 0)
          bc->AddToVelocity((snappedNormal.CalculateNormal() * 250));
        else if (snappedNormal.y < 0)
          bc->AddToVelocity(-(snappedNormal.CalculateNormal() * 250));
      }
      else if (gp->LeftStick_X() < -0.2 && snappedNormal.x == 0)
      {
        //bc->SetVelocity(Vec3(0.0f, 0.0f, 0.0f));
        if (snappedNormal.y > 0)
          bc->AddToVelocity(-(snappedNormal.CalculateNormal() * 250));
        if (snappedNormal.y < 0)
          bc->AddToVelocity((snappedNormal.CalculateNormal() * 250));
      }
      ////////////////////////////////////////////////////////////////////
      if (gp->LeftStick_Y() > 0.2 && snappedNormal.x != 0)
      {
        //bc->SetVelocity(Vec3(0.0f, 0.0f, 0.0f));
        if (snappedNormal.x > 0)
          bc->AddToVelocity(-(snappedNormal.CalculateNormal() * 250));
        else if (snappedNormal.x < 0)
          bc->AddToVelocity((snappedNormal.CalculateNormal() * 250));
      }
      else if (gp->LeftStick_Y() < -0.2 && snappedNormal.x != 0)
      {
        //bc->SetVelocity(Vec3(0.0f, 0.0f, 0.0f));
        if (snappedNormal.x > 0)
          bc->AddToVelocity((snappedNormal.CalculateNormal() * 250));
        if (snappedNormal.x < 0)
          bc->AddToVelocity(-(snappedNormal.CalculateNormal() * 250));
      }

      //jump
      if ((gp->ButtonPressed(XButtons.A) || gp->ButtonPressed(XButtons.LShoulder)) && isSnapped)
      {
        bc->AddToVelocity(-(snappedNormal * 300));
        isSnapped = false;
      }
      else if (!gp->ButtonPressed(XButtons.A))
      {
        //isSnapped = true;
      }
    }
    else
    {

    }
////////////////////////////////////////////////////////////////
		//melee
		if (gp->ButtonPressed(XButtons.B))
		{
      Melee();
		}

		if (gp->ButtonPressed(XButtons.X))
		{
			//bc->AddToAngVelocity(.5f);
		}

    isSnapped = false;
    //snappedNormal = Vec3(0.0, 0.0, 0.0);
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
    SoundEmitter *se = space->GetHandles().GetAs<SoundEmitter>(playerSound);
    GameObject *OtherObject = space->GetHandles().GetAs<GameObject>(otherObject);
    if (OtherObject->name == "Bullet" && !hasRespawned)
    {
      health -= OtherObject->GetComponent<Bullet_Default>(eBullet_Default)->damage;
      
      se->Play("energy_hit", &SoundInstance(0.25f));
      return;
    }
    if ((OtherObject->name == "KillBox" || OtherObject->name == "KillBoxBig") && !hasRespawned)
      health = 0;

    if ((OtherObject->name == "Grinder") && !hasRespawned)
      health -= 10;
    if (OtherObject->name == "WeaponPickup")
      se->Play("weapon_pickup", &SoundInstance(0.5f));

		isSnapped = true;
		//get the thing we are colliding with
    snappedTo = otherObject;
		//get the transform of the thing we are colliding with
		Transform *OOT = OtherObject->GetComponent<Transform>(eTransform);
		//if that thing we collided with's transform is missing, get the fuck otta here, i mean what are you even doing?
		if (!OOT)
			return;

		if (OtherObject->HasComponent(eBoxCollider) && OtherObject->name != "Player")
		{
      BoxCollider *OOBc = OtherObject->GetComponent<BoxCollider>(eBoxCollider);
      Transform *ps = space->GetHandles().GetAs<Transform>(playerTransform);
      if (snappedNormal.x != OOBc->GetCollisionNormals(manifold).x && snappedNormal.y != OOBc->GetCollisionNormals(manifold).y)
        ps->SetTranslation(ps->GetTranslation() + -(snappedNormal * 1.5));
      snappedNormal = OOBc->GetCollisionNormals(manifold);
      ps->SetRotation(OOT->GetRotation());
		}
		else if (OtherObject->HasComponent(eCircleCollider))
		{
      CircleCollider *OOCc = OtherObject->GetComponent<CircleCollider>(eCircleCollider);
      snappedNormal = OOCc->GetCollisionNormals(manifold);
		}

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
		//opposite of init
		space->hooks.Remove("LogicUpdate", self);

    if (weapon != nullptr)
    {
      free(weapon); //release dynamic memory
      weapon = nullptr;
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
    weapon->Fire(space->GetHandles().GetAs<GameObject>(owner));

    if (!isSnapped)
    {
      BoxCollider *bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
      bc->AddToVelocity(-aimDir * 25);
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
	Vec3 PlayerController::aimingDirection(GamePad *gp)
	{
		Vec3 returnVec;
    float thresh = 1.0f; //the threshold minimum for aiming

		returnVec.x = gp->RightStick_X();
		returnVec.y = gp->RightStick_Y();
    returnVec.Normalize();
    returnVec *= 1.5;

    //making sure that the default return vector is within a certain range so that
    //when bullets spawn using that return vector they don't spawn to far away from the player.
    if (returnVec.x > 1.0)
      returnVec.x = 1.0;
    if (returnVec.y > 1.0)
      returnVec.y = 1.0;

    if (returnVec.x < -1.0)
      returnVec.x = -1.0;
    if (returnVec.y < -1.0)
      returnVec.y = -1.0;

		return returnVec;

	}

  //************************************
  // Method:    Melee
  // FullName:  Framework::PlayerController::Melee
  // Access:    public 
  // Returns:   void
  // Qualifier:
  //************************************
  void PlayerController::Melee()
  {

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
    Sprite *ps = space->GetHandles().GetAs<Sprite>(playerSprite);

    if (respawnTimer > 0.0f)
    {
      if (!blink)
        ps->Color.A -= dt * 10.0f;
      else
        ps->Color.A += dt * 10.0f;

      respawnTimer -= dt;

      if (ps->Color.A <= 0.0f)
        blink = true;

      if (ps->Color.A >= 1.0f)
        blink = false;
    }
    else
    {
      ps->Color.A = 255.0f;
      hasRespawned = false;
      respawnTimer = 2.0f;
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
  void PlayerController::PlayerDeath(SoundEmitter *se, Transform *ps)
  {
    se->Play("explosion", &SoundInstance(0.5f));
    Handle explosion = (FACTORY->LoadObjectFromArchetype(space, "explosion"))->self;
    Transform *exT = space->GetGameObject(explosion)->GetComponent<Transform>(eTransform);
    exT->SetTranslation(ps->GetTranslation());
    space->hooks.Call("PlayerDied", playerNum); //calling an event called player died
    space->GetGameObject(owner)->Destroy();
  }
}