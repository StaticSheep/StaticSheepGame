

#include "pch/precompiled.h"
#include "Handle.h"
#include "CParticleBoxEmitter.h"
#include "CParticleSystem.h"
#include "systems/graphics/SheepGraphics.h"
#include "../SheepUtil/include/SheepMath.h"

namespace Framework
{

  ParticleBoxEmitter::ParticleBoxEmitter()
  {
    transform = NULL;
    time = 0.0f;
    m_amount.m_startMin = 1.0f;
    m_rate.m_startMin = 30.0f;
    m_spawnOffset = Vec3();
  }

  ParticleBoxEmitter::~ParticleBoxEmitter()
  {
  }

  void ParticleBoxEmitter::Initialize()
  { 
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(ParticleBoxEmitter::UpdateEmitter));
    space->hooks.Add("FrameUpdate", self, BUILD_FUNCTION(ParticleBoxEmitter::FrameUpdate));
  }

  void ParticleBoxEmitter::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("FrameUpdate", self);
  }

  // for seeing the particles in the editor
  void ParticleBoxEmitter::FrameUpdate(float dt)
  {
    if(ENGINE->m_editorActive)
      UpdateEmitter(dt);
  }

  // 
  void ParticleBoxEmitter::UpdateEmitter(float dt)
  {
    if(space->Paused())
      return;

    // if we are set to currently spawn
    if(spawning)
    {
      ParticleSystem* system = (ParticleSystem*)space->GetComponent(eParticleSystem, owner);

      // and a particle system is actually on this object
      if(system)
        SpawnParticle(dt, system);
    }
    else
    if(timedSpawning && timed >= 0.0f)
    {
      ParticleSystem* system = (ParticleSystem*)space->GetComponent(eParticleSystem, owner);

      timed -= dt;

      if(system)
        SpawnParticle(dt, system);

      if(timed < 0.0f)
        timedSpawning = false;
    }
  }

  void ParticleBoxEmitter::SetSpawnOffset(Vec3& offset)
  {
    m_spawnOffset = offset;
    return;
  }

  // spawns at the specified rate and amount 
  void ParticleBoxEmitter::SpawnParticle(float dt, ParticleSystem* system)
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    time += dt;

    // this loop handles how many particles per second
    while(time > 0.0f)
    {

      // this loop handles how many particles per "burst"
      for(int i = 0; i < (int)m_amount.m_startMin; ++i)
      {
        float randX = GetRandom(-m_width / 2.0f, m_width / 2.0f);
        float randY = GetRandom(-m_height / 2.0f, m_height / 2.0f);

        Vec3 spawnLocation = Vec3(randX, randY, 0.0f);

        Mat3D rot = Mat3D(trans->GetRotation());

        spawnLocation = rot * spawnLocation;

        Vec3 location = trans->GetTranslation() + m_spawnOffset;

        Particle &particle = system->SpawnParticle(location + spawnLocation, true);

      }

      time -= 1.0f / (m_rate.m_startMin);
    }
  }

  void ParticleBoxEmitter::Toggle(bool state)
  {
    spawning = state;
  }

  void ParticleBoxEmitter::SetTimedSpawn(float time_)
  {
    timedSpawning = true;
    timed = time_;
  }

}
