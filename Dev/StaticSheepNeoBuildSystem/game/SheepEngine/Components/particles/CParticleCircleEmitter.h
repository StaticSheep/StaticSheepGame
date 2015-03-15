
#pragma once

#include "components/base/Component.h"
#include "Particles.h"

namespace Framework
{

  class ParticleSystem;

  class ParticleCircleEmitter : public GameComponent
  {
    public:

      /*----- Methods -----*/
      ParticleCircleEmitter();
      ~ParticleCircleEmitter();

      void Initialize();
      void Remove();

      void UpdateEmitter(float dt);
      void FrameUpdate(float dt);

      void SetSpawnOffset(Vec3& offset);

      void SpawnParticle(float dt, ParticleSystem* system);

      void Toggle(bool state);

      void SetTimedSpawn(float time_);


      /*----- Members -----*/
      float m_innerRadius;
      float m_outerRadius;
      bool m_oneShot;
      bool spawning;
      bool timedSpawning;
      bool outward;
      bool inward;
      bool parentToOwner;
      float timed;

      Vec3 m_spawnOffset;

      ParticleOptionShort<float> m_rate;
      ParticleOptionShort<float> m_amount;

    private:

      Handle transform;

      float time;
      
      


  };

}