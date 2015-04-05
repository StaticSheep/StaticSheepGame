/*****************************************************************
Filename: WBase.cpp
Project:  Gam250
Author(s): Zachary Nawar (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/


#include "pch/precompiled.h"
#include "WBase.h"
#include "components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/Weapon_Scripts/CBullet_default.h"
#include "components/particles/CParticleSystem.h"
#include "components/colliders/CCircleCollider.h"

namespace Framework
{

  GameObject* Weapon::CreateBullet(GameObject* ply,
    const char* archetype, float speed)
  {
    GameObject* obj = FACTORY->LoadObjectFromArchetype(ply->space, archetype);

    PlayerController* pc = ply->GetComponent<PlayerController>(ePlayerController);
    Transform* ptrans = ply->GetComponent<Transform>(eTransform);
    SoundEmitter* pse = ply->GetComponent<SoundEmitter>(eSoundEmitter);

    Bullet_Default* bd = obj->GetComponent<Bullet_Default>(eBullet_Default);
    Transform* btrans = obj->GetComponent<Transform>(eTransform);
    CircleCollider* bcol = obj->GetComponent<CircleCollider>(eCircleCollider);
    ParticleSystem* bps = obj->GetComponent<ParticleSystem>(eParticleSystem);
    ParticleCircleEmitter* bce = obj->GetComponent<ParticleCircleEmitter>(eParticleCircleEmitter);
    
    bd->damage = damage;
    bd->playerOwner = pc->playerNum;
    bd->explosive_ = explosive_;

    bce->spawning = false;
    bce->timedSpawning = true;
    bce->timed = 0.001f;

    Vec3 AimDir = pc->aimDir;
    float theta = atan2f(AimDir.y, AimDir.x) - (PI / 2.0f);
    Mat3D rotation(theta);

    bps->direction.m_startMin = rotation * bps->direction.m_startMin;
    bps->direction.m_startMax = rotation * bps->direction.m_startMax;

    bcol->SetBodyCollisionGroup(pc->weaponGroup);
    bcol->AddToVelocity(AimDir * speed);

    btrans->SetTranslation(ptrans->GetTranslation() + (AimDir * 25) + (ply->GetComponent<BoxCollider>(eBoxCollider)->GetBodyUpNormal() * 20));

    pse->Play(fireSound, &SoundInstance(fireVolume));

    return obj;
  }
}