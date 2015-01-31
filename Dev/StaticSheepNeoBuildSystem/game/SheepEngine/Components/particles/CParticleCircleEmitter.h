
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

      void SpawnParticle(float dt, ParticleSystem* system);


      /*----- Members -----*/
      float m_innerRadius;
      float m_outerRadius;
      bool m_oneShot;
      bool spawning;
      bool outward;
      bool inward;

      ParticleOptionShort<float> m_rate;
      ParticleOptionShort<float> m_amount;

    private:

      Handle transform;

      float time;
      


  };

}