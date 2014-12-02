#pragma once

#include "components/base/Component.h"
#include "components/gamepad/CGamePad.h"
#include "../../sound/CSoundEmitter.h"
#include "../../transform/CTransform.h"
#include "types/weapons/WBase.h"
#include "AnimationController.h"

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
		Vec3 aimingDirection(GamePad *gp);
    void Melee();
    void RespawnBlink(float dt);
    void PlayerDeath(SoundEmitter *se, Transform *pt);
    void SetAnimations();

    int CurrentHealth();

		//member variables
		int playerNum; //the player number, i.e. 1, 2, 3, 4
    int health; //players health
		bool hasFired, isSnapped, hasRespawned, blink; //has fired is a flag to prevent fully auto fire, is snapped is a bool to see if the player is snapped to something
    bool GodMode, GoldenGun, PerfectMachine;
    float shotDelay;
    float respawnTimer;
    AnimationController animCont;
    Weapon *weapon;
		Vec3 aimDir;  //the direction the player is currently aiming
    Vec3 snappedNormal; //the normal of the thing we are currently snapped to
		Handle playerGamePad; //the players game pad
		Handle playerCollider; //the players collider (box)
		Handle playerTransform; //the player's transform
		Handle snappedTo; //the thing we are currently "snapped" to
    Handle playerSound; //to play sound from the player
    Handle playerSprite; //to adjust the sprite colors and alpha
    Handle playerAnimation;
	};
}