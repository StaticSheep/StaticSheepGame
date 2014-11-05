#include "pch/precompiled.h"
#include "CPlayerController.h"
#include "types/space/Space.h"
#include "components/colliders/CBoxCollider.h"
#include "types/vectors/Vec3.h"
#include "components/transform/CTransform.h"
#include "../../colliders/CCircleCollider.h"
#include "../../sound/CSoundEmitter.h"

namespace Framework
{
#define delay 5
	PlayerController::PlayerController() //1
	{
		//set defaults
		playerNum = 0;
		playerGamePad = Handle::null; //this is how you null a handle right
		isSnapped = true;
		hasFired = false;
    health = 100;
    snappedTo = Handle::null;
    respawnTimer = 0.0;
    shotDelay = delay;
	}

	PlayerController::~PlayerController() //4
	{
		//release dynamic memory
	}


	void PlayerController::Initialize() //2
	{
		//logic setup, you're attached and components are in place
		space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(PlayerController::LogicUpdate));
		space->GetGameObject(owner)->hooks.Add("OnCollision", self, BUILD_FUNCTION(PlayerController::OnCollision));

		playerGamePad = space->GetGameObject(owner)->GetComponentHandle(eGamePad); //gets the handle to the gamepad
		playerCollider = space->GetGameObject(owner)->GetComponentHandle(eBoxCollider);
		playerTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    playerSound = space->GetGameObject(owner)->GetComponentHandle(eSoundEmitter);

		GamePad *gp = space->GetHandles().GetAs<GamePad>(playerGamePad); //actually gets the gamepad
		gp->SetPad(playerNum); //setting pad number

		aimDir.x = 1;
		aimDir.y = 0;

    BoxCollider *bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
    bc->SetGravityOff();
	}

	void PlayerController::LogicUpdate(float dt)
	{
		//get the game pad
		GamePad *gp = space->GetHandles().GetAs<GamePad>(playerGamePad);
		//get the box collider of player
		BoxCollider *bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
    SoundEmitter *se = space->GetHandles().GetAs<SoundEmitter>(playerSound);
    Transform *ps = space->GetHandles().GetAs<Transform>(playerTransform);

    if (health <= 0)
    {
      se->PlayEx("explosion", 1.0f);
      //space->GetGameObject(owner)->Destroy();
      ps->SetTranslation(Vec3(0.0, 0.0, 0.0));
      health = 100;

    }


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
        shotDelay = delay;
      }
    }
		//if the trigger is released, reset the bool
		if (!gp->RightTrigger())
    {
			hasFired = false;
      shotDelay = delay;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////
    if (isSnapped)
    {
      bc->SetVelocity(snappedNormal * 50);
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
      if (gp->LeftStick_X() > 0.2 && snappedNormal.y != 0)
      {
        //bc->SetVelocity(Vec3(0.0f, 0.0f, 0.0f));
        if (snappedNormal.y > 0)
          bc->AddToVelocity((snappedNormal.CalculateNormal() * 250));
        else if (snappedNormal.y < 0)
          bc->AddToVelocity(-(snappedNormal.CalculateNormal() * 250));
      }
      else if (gp->LeftStick_X() < -0.2 && snappedNormal.y != 0)
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

		}

		if (gp->ButtonPressed(XButtons.X))
		{
			//bc->AddToAngVelocity(.5f);
		}

    isSnapped = false;
		
	}

  /////////////////////////////////////////////////////////////////////////////////////////////////////
	void PlayerController::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
	{
    GameObject *OtherObject = space->GetHandles().GetAs<GameObject>(otherObject);
    if (OtherObject->name == "Bullet")
    {
      health -= 10;
      return;
    }
    if (OtherObject->name == "KillBox" || OtherObject->name == "KillBoxBig")
      health = 0;

		isSnapped = true;
		//get the thing we are colliding with
    snappedTo = otherObject;
		//get the transform of the thing we are colliding with
		Transform *OOT = OtherObject->GetComponent<Transform>(eTransform);
		//if that thing we collided with's transform is missing, get the fuck otta here, i mean what are you even doing?
		if (!OOT)
			return;

		if (OtherObject->HasComponent(eBoxCollider))
		{
      BoxCollider *OOBc = OtherObject->GetComponent<BoxCollider>(eBoxCollider);
      snappedNormal = OOBc->GetCollisionNormals(manifold);
		}
		else if (OtherObject->HasComponent(eCircleCollider))
		{
      CircleCollider *OOCc = OtherObject->GetComponent<CircleCollider>(eCircleCollider);
      snappedNormal = OOCc->GetCollisionNormals(manifold);
		}

	}

	void PlayerController::Remove() //3
	{
		//opposite of init
		space->hooks.Remove("LogicUpdate", self);
	}

	void PlayerController::onFire()
	{
		GameObject *bullet = (FACTORY->LoadObjectFromArchetype(space, "Bullet"));
		Transform *BT = bullet->GetComponent<Transform>(eTransform);
		CircleCollider *bulletC = bullet->GetComponent <CircleCollider>(eCircleCollider);
		Transform *playerTrans = space->GetHandles().GetAs<Transform>(playerTransform);
		BT->SetTranslation(playerTrans->GetTranslation() + aimDir * 25);
		bulletC->AddToVelocity(aimDir * 1000);

    SoundEmitter *se = space->GetHandles().GetAs<SoundEmitter>(playerSound);
    se->PlayEx("gunshot", 0.125f);

    if (!isSnapped)
    {
      BoxCollider *bc = space->GetHandles().GetAs<BoxCollider>(playerCollider);
      bc->AddToVelocity(-aimDir * 25);
    }
	}

  ///////////////////////////////////////////////////////////////////////////////////////////
	Vec3 PlayerController::aimingDirection(GamePad *gp)
	{
		Vec3 returnVec;
    float thresh = 1.0f; //the threshold minimum for aiming

		returnVec.x = gp->RightStick_X() * 10;
		returnVec.y = gp->RightStick_Y() * 10;
    //you're about to see a lot of if statements, just know that all of these are 
    //making sure that the default return vector is within a certain range so that
    //when bullets spawn using that return vector they don't spawn to close to the player.
    if (returnVec.x > 1.0)
      returnVec.x = 1.0;
    if (returnVec.y > 1.0)
      returnVec.y = 1.0;

    if (returnVec.x < -1.0)
      returnVec.x = -1.0;
    if (returnVec.y < -1.0)
      returnVec.y = -1.0;

    /*if (returnVec.x < thresh && returnVec.x > 0 && !(returnVec.y >= thresh || returnVec.y <= -thresh))
      returnVec.x = thresh;
    if (returnVec.y < thresh && returnVec.y > 0 && !(returnVec.x >= thresh || returnVec.x <= -thresh))
      returnVec.y = thresh;

    if (returnVec.x > -thresh && returnVec.x < 0 && !(returnVec.y <= thresh || returnVec.y >= -thresh))
      returnVec.x = -thresh;
    if (returnVec.y > -thresh && returnVec.y < 0 && !(returnVec.x <= thresh || returnVec.x >= -thresh))
      returnVec.y = -thresh;*/

		return returnVec;

	}
}