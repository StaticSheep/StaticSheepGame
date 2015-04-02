/*****************************************************************
Filename: CAOEDamage.h
Project:  Gam200
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CAOEDamage.h"
#include "../../colliders/CCircleCollider.h"
#include "../../controllers/player/CPlayerController.h"

namespace Framework
{

  AOEDamage::AOEDamage()
  {

  }

  AOEDamage::~AOEDamage()
  {

  }

  void AOEDamage::Initialize()
  {
    space->hooks.Add("LogicUpdate", self,
      BUILD_FUNCTION(AOEDamage::LogicUpdate));

    GetOwner()->hooks.Add("SetPlayerOwner", self,
      BUILD_FUNCTION(AOEDamage::PlayerOwnerBroadcast));
    
    space->GetGameObject(owner)->hooks.Add(
      "OnCollision", self, BUILD_FUNCTION(AOEDamage::OnCollision));
  }

  void AOEDamage::PlayerOwnerBroadcast(int playerOwner)
  {
    m_playerOwner = playerOwner;
  }

  void AOEDamage::LogicUpdate(float dt)
  {
    if (m_firstRun)
    {
      CircleCollider* cl = space->GetGameObject(owner)->
        GetComponent<CircleCollider>(eCircleCollider);

      if (cl)
      {
        cl->SetBodyCollisionGroup("PlayerHitOnly");
        cl->SetBodyRadius(m_radius);
      }

      m_firstRun = false;
      
      if (m_shrinks)
      {
        if (m_shrinkTime <= 0.0f)
          m_shrinkTime = dt;

        m_radiusPerSecond = m_radius / m_shrinkTime;
      }
        
    }


    if (m_shrinks)
    {

      m_shrinkDelay -= dt;

      if (m_shrinkDelay < 0.0f)
      {
        m_radius -= m_radiusPerSecond * dt;

        CircleCollider* cl = space->GetGameObject(owner)->
          GetComponent<CircleCollider>(eCircleCollider);

        if (cl)
        {
          if (m_radius < 0.0f)
          {

            { /* Proper way to detach a circle collider from an object*/
              Transform* trans = (space->GetHandles().GetAs<GameObject>(owner))
                ->GetComponent<Transform>(eTransform);

              Vec3 position = trans->GetTranslation();
              float rotation = trans->GetRotation();

              trans->SetPhysicsBody(Handle::null);

              trans->SetTranslation(position);
              trans->SetRotation(rotation);
            }
            space->GetGameObject(owner)->DetatchComponent(eCircleCollider);

            m_shrinks = false;
            space->GetGameObject(owner)->hooks.Remove("OnCollision", self);
          }
          else
          {
            cl->SetBodyRadius(m_radius);
          }
        }
      }


    }


  }

  void AOEDamage::OnCollision(Handle otherObject,
    SheepFizz::ExternalManifold manifold)
  {
    GameObject* obj = space->GetGameObject(otherObject);

    if (!obj)
      return;

    PlayerController* pc = obj->GetComponent<PlayerController>(ePlayerController);

    if (!pc)
      return;

    /* Kill me */
    pc->Combat()->TakeDamage(m_damagePerSecond * 0.0167f, m_playerOwner);
  }

  void AOEDamage::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }


}