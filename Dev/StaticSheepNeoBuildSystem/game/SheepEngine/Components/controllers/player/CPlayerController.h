/*****************************************************************
Filename: CPlayerController.h
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "components/gamepad/CGamePad.h"
#include "../../sound/CSoundEmitter.h"
#include "../../transform/CTransform.h"
#include "types/weapons/WBase.h"
#include "AnimationController.h"
#include "../../colliders/CBoxCollider.h"
#include "../../gameplay_scripts/CCheats.h"
#include "systems/metrics/MetricInfo.h"
#include "types/powerUps/PBase.h"

namespace Framework
{
	class PlayerController : public GameComponent
	{
	public:
		PlayerController();
		~PlayerController();
		void LogicUpdate(float dt);
		void OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold);
		void Initialize();
		void Remove();
		void onFire();
		Vec3 aimingDirection(GamePad *gp, char stick = 'R');
    void Melee(Buttons butt);
    void RespawnBlink(float dt);
    void PlayerDeath(SoundEmitter *se, Transform *pt, int who_killed_me = -1);
    void SetAnimations();

    void clampVelocity(float clamp);
    int CurrentHealth();
    void jump();
    void PlayerButtonPress();
    void DealDamage(float damage, int playNum);
    void SpawnEffect();

    void CollisionDamage(GameObject *OtherObject);
    void DetermineSnap(GameObject *OtherObject, Handle otherObject, SheepFizz::ExternalManifold manifold);
    void SpawnAimArrow();
    void SnappedMovement();


		//member variables
		int playerNum; //the player number, i.e. 1, 2, 3, 4
    float health; //players health
    float shields;
		bool hasFired, isSnapped, hasRespawned, blink, stoppedFX, firstUpdate; //has fired is a flag to prevent fully auto fire, is snapped is a bool to see if the player is snapped to something
    bool GodMode, GoldenGun, PerfectMachine;  //the cheats 
    bool frameSkip, arrowSpawn, hasDashed;
    float rotation, lastRotation;
    float respawnTimer;


    std::string weaponGroup;

    std::vector<Vec3> normals;
    AnimationController animCont;
    Weapon *weapon;
    PowerUp *powerUp;
		Vec3 aimDir;  //the direction the player is currently aiming
    Vec3 snappedNormal; //the normal of the thing we are currently snapped to
    Vec3 nextSnappedNormal;
		Handle playerGamePad; //the players game pad
		Handle playerCollider; //the players collider (box)
		Handle playerTransform; //the player's transform
		Handle snappedTo; //the thing we are currently "snapped" to
    Handle playerSound; //to play sound from the player
    Handle playerSprite; //to adjust the sprite colors and alpha
    Handle playerAnimation;
    Handle spawnEffect;
    Handle spineHandle;

    Vec4 playerColor;
    bool animationFlip;

	};
}