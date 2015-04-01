


#pragma once

#include "components/base/Component.h"
#include "../../transform/CTransform.h"
#include "../../colliders/CBoxCollider.h"
#include "../../sound/CSoundEmitter.h"
#include "../types/handle/Handle.h"
#include "components/gamepad/CGamePad.h"


namespace Framework
{

  class MovementController
  {
    public:

      MovementController();
      ~MovementController();

      void Update(GameObject* Owner);
      void DetermineSnap(BoxCollider* Collider, GameObject *OtherObject, SheepFizz::ExternalManifold Manifold);
      bool IsSnapped(){return m_isSnapped;};
      bool CanSnap();

    private:

      void SnappedMovement();
      void Jump();
      void Dash();
      void ClampVelocity(float clamp);

      Vec3 m_moveDirection;
      Vec3 m_otherObjectVelocity;
      Vec3 m_currentPosition;
      Vec3 m_otherObjectAngularVelocity;

      Vec3 m_snappedNormal;

      GamePad* m_pad;
      BoxCollider* m_collider;
      Handle m_transform;
      SoundEmitter* m_emitter;
      GameSpace* m_space;


      bool m_isSnapped;     // if we are snapped right now
      bool m_hasDashed;
      bool m_circleFound;   // if snapped to a circle
      bool m_otherObjectSpin; 

      int m_jumpFrame;      // how many frames before jump is possible
      int m_checkSnap;      // how many frames without collision before unsnapping
      int m_collisionTotal; // total number of collisions this frame
      int m_unsnappable;    // frames where snapping is not possible (after jumping)

      float m_maxVelocity;
      float m_angularVelocity;


  };


}