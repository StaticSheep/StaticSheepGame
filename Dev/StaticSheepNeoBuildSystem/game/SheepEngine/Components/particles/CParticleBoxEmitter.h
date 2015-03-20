
#pragma once

#include "components/base/Component.h"
#include "Particles.h"

namespace Framework
{

  class ParticleSystem;

  class ParticleBoxEmitter : public GameComponent
  {
    public:

      /*----- Methods -----*/
      ParticleBoxEmitter();
      ~ParticleBoxEmitter();

      void Initialize();
      void Remove();

      void UpdateEmitter(float dt);
      void FrameUpdate(float dt);

      void SetSpawnOffset(Vec3& offset);

      void SpawnParticle(float dt, ParticleSystem* system);

      void Toggle(bool state);

      void SetTimedSpawn(float time_);


      /*----- Members -----*/
      float m_width;
      float m_height;
      float m_depth;
      bool m_oneShot;
      bool spawning;
      bool timedSpawning;
      float timed;
      bool parented;

      Vec3 m_spawnOffset;

      ParticleOptionShort<float> m_rate;
      ParticleOptionShort<float> m_amount;

    private:

      Handle transform;

      float time;
      
      


  };

}