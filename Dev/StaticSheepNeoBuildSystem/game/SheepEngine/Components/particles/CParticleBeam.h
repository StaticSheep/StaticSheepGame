
#pragma once

#include "components/base/Component.h"
#include "Particles.h"

namespace Framework
{

  class ParticleSystem;

  class ParticleBeam : public GameComponent
  {
  public:

    /*----- Methods -----*/
    ParticleBeam();
    ~ParticleBeam();

    void Initialize();
    void Remove();

    void UpdateEmitter(float dt);
    void FrameUpdate(float dt);

    void SetSpawnOffset(Vec3& offset);

    void SpawnParticle(float dt, ParticleSystem* system);

    void Toggle(bool state);

    void SetTimedSpawn(float time_);


    /*----- Members -----*/
    bool m_oneShot;
    bool spawning;
    bool parented = true;

    Vec3 m_spawnOffset;

    ParticleOptionShort<float> m_rate;
    ParticleOptionShort<float> m_amount;

  private:

    Handle transform;

    float time;


    friend class Saucer;

  };

}