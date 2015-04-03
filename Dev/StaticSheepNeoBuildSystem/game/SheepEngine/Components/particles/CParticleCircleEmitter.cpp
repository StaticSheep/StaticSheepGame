/*****************************************************************
Filename: CParticleCircleEmitter.cpp
Project: 
Author(s): Zakary Wilson

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "Handle.h"
#include "CParticleCircleEmitter.h"
#include "CParticleSystem.h"
#include "systems/graphics/SheepGraphics.h"
#include "../SheepUtil/include/SheepMath.h"


namespace Framework
{

  ParticleCircleEmitter::ParticleCircleEmitter()
  {
    transform = NULL;
    time = 0.0f;
    m_amount.m_startMin = 1.0f;
    m_rate.m_startMin = 30.0f;
    m_spawnOffset = Vec3();
  }

  ParticleCircleEmitter::~ParticleCircleEmitter()
  {
  }

  void ParticleCircleEmitter::Initialize()
  { 
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    ParticleSystem* system = (ParticleSystem*)space->GetComponent(eParticleSystem, owner);

    // and a particle system is actually on this object
    if (system)
      system->parentToOwner = parentToOwner;

    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(ParticleCircleEmitter::UpdateEmitter));
    space->hooks.Add("FrameUpdate", self, BUILD_FUNCTION(ParticleCircleEmitter::FrameUpdate));
  }

  void ParticleCircleEmitter::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("FrameUpdate", self);
  }

  // for seeing the particles in the editor
  void ParticleCircleEmitter::FrameUpdate(float dt)
  {
    if(ENGINE->m_editorActive)
      UpdateEmitter(dt);
  }

  // updates the emitter
  void ParticleCircleEmitter::UpdateEmitter(float dt)
  {
    if(space->Paused() && !ENGINE->m_editorActive)
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

  // Set the offset from the transform of the object this is attached to
  void ParticleCircleEmitter::SetSpawnOffset(Vec3& offset)
  {
    m_spawnOffset = offset;
    return;
  }

  // spawns at the specified rate and amount 
  void ParticleCircleEmitter::SpawnParticle(float dt, ParticleSystem* system)
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    time += dt;

    // this loop handles how many particles per second
    while(time > 0.0f)
    {

      // this loop handles how many particles per "burst"
      for(int i = 0; i < (int)m_amount.m_startMin; ++i)
      {
        float randX = GetRandom(-100.0f, 100.0f);
        float randY = GetRandom(-100.0f, 100.0f);
        float randLength = GetRandom(m_innerRadius, m_outerRadius);

        Vec3 direction = Vec3(randX, randY, 0.0f).Normalize();

        Vec3 location;
        if (parentToOwner)
          location = m_spawnOffset;
        else
          location = trans->GetTranslation() + m_spawnOffset;

        if(!outward && !inward)
          Particle &particle = system->SpawnParticle(location + direction * randLength, true);

        if(outward)
        {
          Particle &particle = system->SpawnParticle(location + direction * randLength, false);
          particle.direction = direction;
          particle.currentDirection = direction;
          particle.endDirection = direction;
        }
        else
        if(inward)
        {
          Particle &particle = system->SpawnParticle(location + direction * randLength, false);
          particle.direction = -direction;
          particle.currentDirection = -direction;
          particle.endDirection = -direction;
        }
      }

      time -= 1.0f / (m_rate.m_startMin);
    }

    //system->ResetZ();
  }

  // toggles spawning on or off
  void ParticleCircleEmitter::Toggle(bool state)
  {
    spawning = state;
  }

  // tells the emitter to spawn for a set amount of time then stop
  void ParticleCircleEmitter::SetTimedSpawn(float time_)
  {
    timedSpawning = true;
    timed = time_;
  }

}
