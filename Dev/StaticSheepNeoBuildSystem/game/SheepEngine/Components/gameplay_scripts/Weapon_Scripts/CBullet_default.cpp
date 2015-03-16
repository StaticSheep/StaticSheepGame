/*****************************************************************
Filename: CBullet_default.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CBullet_default.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../arena/CBlockLights.h"
#include "../Pickup_Scripts/CPowerupPickup.h"
#include "../Pickup_Scripts/CWeaponPickup.h"
#include "../../basicps/CBasicPSystem.h"
#include "../../lights/CPointLight.h"

namespace Framework
{
	Bullet_Default::Bullet_Default()
    : fading(false), fadeTime(0), limitedLife(false)
	{
    damage = 10;
    ttl = 0.375f;
    explosive_ = false;
	}

	Bullet_Default::~Bullet_Default()
	{

	}

	void Bullet_Default::Initialize()
	{
		//logic setup, you're attached and components are in place
		space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Bullet_Default::LogicUpdate));
		space->GetGameObject(owner)->hooks.Add("OnCollision", self, BUILD_FUNCTION(Bullet_Default::OnCollision));

    bTransfrom = space->GetGameObject(owner)->GetComponentHandle(eTransform);
    bGameObject = space->GetHandles().GetAs<GameObject>(owner);
    space->GetHandles().GetAs<BoxCollider>(space->GetGameObject(owner)->GetComponentHandle(eCircleCollider))->SetGravityOff();
	}

	void Bullet_Default::Remove()
	{
    space->GetGameObject(owner)->hooks.Remove("OnCollision", self);
		space->hooks.Remove("LogicUpdate", self);

	}

  static Transform *bt;
	void Bullet_Default::LogicUpdate(float dt)
	{
    bt = space->GetHandles().GetAs<Transform>(bTransfrom);
    bGameObject = space->GetHandles().GetAs<GameObject>(owner);

    if (bt->GetTranslation().x > 1000 ||
      bt->GetTranslation().x < -1000 ||
      bt->GetTranslation().y > 700 ||
      bt->GetTranslation().y < -700)
      Impact();

    if (fading)
    {
      fadeTime -= dt;

      PointLight* pl = bGameObject->GetComponent<PointLight>
        (ePointLight);

      if (pl)
      {
        pl->m_brightness.a -= lightFade * dt;
        if (pl->m_brightness.a <= 0)
          bGameObject->DetatchComponent(ePointLight);
      }

      if (fadeTime < 0)
        Impact();

      return;
    }

    if (limitedLife && !fading)
    {
      ttl -= dt;
      if (ttl <= 0)
      {
        Impact();
      }
    }

	}

  void Bullet_Default::Impact()
  {

    if (fadeTime > 0 && !fading)
    {
      fading = true;

      GameObject* obj = space->GetGameObject(owner);

      
      
      { /* Proper way to detach a circle collider from an object*/
        Transform* trans = (space->GetHandles().GetAs<GameObject>(owner))
          ->GetComponent<Transform>(eTransform);

        Vec3 position = trans->GetTranslation();
        float rotation = trans->GetRotation();

        trans->SetPhysicsBody(Handle::null);

        trans->SetTranslation(position);
        trans->SetRotation(rotation);
      }

      /* Remove old components */
      obj->DetatchComponent(eCircleCollider);

      obj->DetatchComponent(eSprite);
      obj->DetatchComponent(eParticleCircleEmitter);
      obj->DetatchComponent(eParticleBoxEmitter);


      BasicParticleSystem* bps = obj->GetComponent<BasicParticleSystem>
        (eBasicParticleSystem);

      if (bps)
        bps->Toggle(false);

      PointLight* pl = obj->GetComponent<PointLight>
        (ePointLight);

      if (pl) // How much to decrease the light alpha per second
        lightFade = pl->m_brightness.a / lightFade;

    }
    else
    {
      if (fadeTime <= 0)
        space->GetGameObject(owner)->Destroy();
    }
      
    
  }

  void Bullet_Default::OnCollision(Handle otherObject, SheepFizz::ExternalManifold manifold)
	{
    GameObject *OtherObject = space->GetHandles().GetAs<GameObject>(otherObject);

    /*if (OtherObject->archetype == "SmallArenaBlock")
    {
    BlockLights::EventData ed;
    ed.overrideDefault = false;
    ed.settings.color = Vec4(1.0f, 0.2f, 0.2f, 1.0f);
    ed.settings.fx = BlockLights::FLICKER;
    ed.duration = 0.5f;
    OtherObject->hooks.Call("LightingEvent", &ed);
    }*/

    Bullet_Default* blt = OtherObject->GetComponent<Bullet_Default>(eBullet_Default);
    WeaponPickup* wpi = OtherObject->GetComponent<WeaponPickup>(eWeaponPickup);
    PowerupPickup* pp = OtherObject->GetComponent<PowerupPickup>(ePowerupPickup);

    if (!blt && !wpi & !pp)
    {
      //if (explosive_)
      //{
      //  GameObject *temp = (FACTORY->LoadObjectFromArchetype(space, "explosion"));
      //  temp->GetComponent<Transform>(eTransform)->SetTranslation(bt->GetTranslation());
      //}

      Impact();
      //space->GetGameObject(owner)->Destroy();
    }


    
	}


}
