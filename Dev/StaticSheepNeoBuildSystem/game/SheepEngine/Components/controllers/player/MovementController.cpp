
#include "pch/precompiled.h"
#include "MovementController.h"
#include "../../SheepUtil/include/Matrix3D.h"
#include "../systems/input/Input.h"
#include "systems/metrics/MetricInfo.h"
#include "../../gameplay_scripts/Player_Scripts/CDashEffect.h"


namespace Framework
{

  MovementController::MovementController()
  {
    m_moveDirection = Vec3();
    m_otherObjectVelocity = Vec3();

    m_snappedNormal = Vec3();

    m_pad = nullptr;
    m_collider = nullptr;
    m_emitter = nullptr;

    m_isSnapped = false;
    m_hasDashed = false;
    m_circleFound = false;
    m_otherObjectSpin = false; 

    m_jumpFrame = 30;
    m_checkSnap = 4;
    m_unsnappable = 0;
    m_collisionTotal = 0;

    m_maxVelocity = 500.0f;
  }

  MovementController::~MovementController()
  {
    
  }

  void MovementController::Update(GameObject* Owner)
  {

    m_pad = Owner->GetComponent<GamePad>(eGamePad);

    if(m_pad)
      m_moveDirection = Vec3(m_pad->LeftStick_X(), m_pad->LeftStick_Y(), 0.0f);
    else
      return;

    if (!m_triggerReset)
      m_triggerReset = m_pad->LeftTrigger() == 0.0f;

    // if we are snapped, handle movement. Otherwise let physics
    // handle moving through space.
    if(m_isSnapped)
    {
      
      m_collider = Owner->GetComponent<BoxCollider>(eBoxCollider);

      m_transform = Owner->GetComponentHandle(eTransform);

      m_emitter = Owner->GetComponent<SoundEmitter>(eSoundEmitter);
      m_space = Owner->space;

      // if none of these components exist, get out
      if(!m_collider || !m_emitter)
        return;

      SnappedMovement();
      m_hasDashed = false;
    }
    else
    if (m_pad && (m_pad->ButtonPressed(XButtons.LShoulder)
      || m_pad->ButtonPressed(XButtons.RShoulder)
      || m_pad->ButtonPressed(XButtons.A)
      || (m_pad->LeftTrigger() && m_triggerReset))
      )
    {
      m_collider = Owner->GetComponent<BoxCollider>(eBoxCollider);

      m_transform = Owner->GetComponentHandle(eTransform);

      m_emitter = Owner->GetComponent<SoundEmitter>(eSoundEmitter);
      m_space = Owner->space;

      Dash();
    }

    // clear out the other object velocity to recalculate for next frame
    m_otherObjectVelocity = Vec3();

    if(m_jumpFrame > 0)
      --m_jumpFrame;

    if(m_checkSnap > 0)
      --m_checkSnap;

    if(m_unsnappable > 0)
      --m_unsnappable;

    return;
  }

  void MovementController::DetermineSnap(BoxCollider* Collider, GameObject *OtherObject, SheepFizz::ExternalManifold Manifold)
  {
    m_circleFound = false;

    // if the other object does not have a transform, gtfo
    if(!OtherObject->HasComponent(eTransform))
      return;
    
    // try to grab the box collider first
    RigidBody* body = OtherObject->GetComponent<RigidBody>(eBoxCollider);

    // if that was not successful
    if(!body)
    {
      // try to get the circle collider
      body = OtherObject->GetComponent<RigidBody>(eCircleCollider);
      m_circleFound = true;
    }

    /*if(OtherObject->GetArchetype() == "Grinder")
    {
      m_isSnapped = false;
      Vec3 normal = body->GetBodyPosition() - Collider->GetBodyPosition();

      if(normal.y > 0)
        normal = Vec3(0.0f, 4.0f, 0.0f);
      else
        normal = Vec3(0.0f, -4.0f, 0.0f);

      Collider->SetGravityOn();
      Collider->SetGravityNormal(normal);

      m_unsnappable = 6;
      return;
    }*/

    // if neither collider was found, what the hell, gtfo
    if(!body || !body->m_snap)
      return;

    // get the object's velocity for moving platform calculations
    m_otherObjectVelocity += body->GetCurrentVelocity();

    // keep track of how many collisions happened this frame
    ++m_collisionTotal;

    // check if the platform is spinning
    if (body->GetBodyAngVelocity() != 0 || body->GetCurrentVelocity().SquareLength() > 1.0f)
      m_otherObjectSpin = true;
    else
      m_otherObjectSpin = false;

    // grab the normal to the collision
    Vec3 normal = body->GetCollisionNormals(Manifold);
    bool found = false;

    // add up all normals to average at the next logic update
    m_snappedNormal += normal;

    // we collided with something, reset this value. 
    // this says to skip 4 frames of non-collisions before
    // automatically unsnapping
    m_checkSnap = 4;

    // if this is our first time snapping onto something
    if(!m_isSnapped)
    {
      m_snappedNormal = normal;

      // set rotation and gravity appropriately and turn gravity on
      Collider->SetBodyRotation(-normal);
      Collider->SetGravityNormal(normal);
      Collider->SetGravityOn();

      // we are now "snapped"
      m_isSnapped = true;
    }
    else
    {
      Mat3D rot;

      // for box colliders
      if(!m_circleFound)
      {
        // need to recalculate the gravity direction, which will be
        // the local down vector
        rot = Mat3D(Collider->GetBodyRotation() - PI / 2.0f);


/******************************************************************************

Calculating angular velocites for rotating box collider platforms... -HelpJon

******************************************************************************/
        Vec3 normalX = m_snappedNormal.CalculateNormal();

        float mod = 1;
        float direction = normalX * (Collider->GetBodyPosition() - body->GetBodyPosition());
        if(direction < 0)
          mod = -1;
        m_angularVelocity = mod * body->GetBodyAngVelocity() * 1.5f;
        m_otherObjectAngularVelocity = m_snappedNormal * m_angularVelocity;

/*****************************************************************************/

      }
      else // for circle colliders
      {
        // get the vector between us and the circle
        Vec3 orientation = body->GetBodyPosition() - Collider->GetBodyPosition();
        rot = Mat3D(atan2f(orientation.y, orientation.x));

        // and reset our rotation
        Collider->SetBodyRotation(-orientation);

        m_otherObjectAngularVelocity = Vec3();

      }

      // set our gravity to our local down vector
      Collider->SetGravityNormal(rot * Vec3(1.0f, 0.0f, 0.0f));

      // and damp our velocity 
      Collider->SetVelocity(Collider->GetCurrentVelocity() * 0.5f);
    }
  }

  bool MovementController::CanSnap()
  {
    if(m_unsnappable <= 0)
      return true;

    return false;
  }


  void MovementController::SnappedMovement()
  {
    //hasDashed = false;

    m_snappedNormal.Normalize();

    // if we have not collided with anything for awhile
    if(m_checkSnap <= 0)
    {
      // unsnap
      m_isSnapped = false;
      ClampVelocity(50.0f);
      m_collider->SetGravityOff();
      return;
    }

    // set our rotation to the average collision normals
    // if we are colliding with only box colliders
    if(!m_circleFound)
    {
      m_collider->SetBodyRotation(-m_snappedNormal);
    }

    if (
      ((m_pad->ButtonDown(XButtons.A) || m_pad->LeftTrigger()) && m_isSnapped) 
      || (SHEEPINPUT->KeyIsDown('Q') && m_pad->GetIndex() == 0))
    {
      // if enough time has passed to jump...
      if(m_jumpFrame == 0)
      {
        Jump(); //player jump
        if (GetRandom(0, 1)) //determine sound for jump
          m_emitter->Play("jump2", &SoundInstance(0.75f));
        else
          m_emitter->Play("jump1", &SoundInstance(0.75f));

        m_jumpFrame = 30;
        m_unsnappable = 4;
        m_isSnapped = false;
        m_triggerReset = false;

        return;
      }
    }

    // if there were collisions... add the platform's velocities to the
    // player to offset platform movement
    if(m_collisionTotal)
    {
      m_collider->AddToVelocity(m_otherObjectVelocity * 0.5f);

      // Adding angular velocity. -HelpJon
      m_collider->AddToVelocity(m_otherObjectAngularVelocity);
    }


    if (m_pad->LStick_InDeadZone())
    {
      if (m_otherObjectSpin)
      {
        m_collider->SetBodyFrictionMod(1.89f);
      }

      // intentional return
      return;
    }
      

    m_collider->SetBodyFrictionMod(0.0f);

    

    // get the local x axis of the player
    Vec3 localX = Mat3D(m_collider->GetBodyRotation()) * Vec3(1.0f, 0.0f, 0.0f);

    // project movement direction onto the local x axis
    Vec3 movementDir = (m_moveDirection * localX) * localX;

    // and add that vector multiplied by speed
    m_collider->AddToVelocity(movementDir * 400.0f);

    ClampVelocity(m_maxVelocity);

    return;
  }


  void MovementController::Jump()
  {
    Vec3 jumpDir;
    if (m_pad->LStick_InDeadZone())
      jumpDir = -m_snappedNormal;
    else
    {
      jumpDir = m_moveDirection;

      jumpDir.Normalize();

      if (-m_snappedNormal * jumpDir < 0)
      {
        jumpDir += ((m_snappedNormal * jumpDir) * -m_snappedNormal) * 2.0f;
      }
    }

    m_collider->AddToVelocity(jumpDir * 1000);
    m_collider->SetGravityOff();
    m_isSnapped = false;

    return;
  }

  void MovementController::ClampVelocity(float clamp)
  {
    Vec3 velocity = m_collider->GetCurrentVelocity();
    
    m_collider->SetVelocity(velocity.Normalize() * clamp);

    return;
  }

  void MovementController::Dash()
  {
    if (m_hasDashed || m_isSnapped)
      return;

    //zero out all the velocity the player has
    m_collider->SetVelocity(Vec3(0.0f, 0.0f, 0.0f));
    if (m_pad->LStick_InDeadZone())
    {
      if (m_pad->ButtonPressed(XButtons.LShoulder))
        m_collider->SetVelocity(Vec3(-1000.0f, 0.0f, 0.0f));
      else
        m_collider->SetVelocity(Vec3(1000.0f, 0.0f, 0.0f));
    }
    else
      m_collider->SetVelocity( m_moveDirection * 1000);

    GameObject *dash_effect = (FACTORY->LoadObjectFromArchetype(m_space, "fire_effect1"));
    dash_effect->GetComponent<DashEffect>(eDashEffect)->pTransform = m_transform;
    dash_effect->GetComponent<Transform>(eTransform)->SetTranslation(m_collider->GetBodyPosition());
    m_emitter->Play("dash", &SoundInstance(1.0f));
    m_hasDashed = true;
  }

}
