

#include "pch/precompiled.h"
#include "Handle.h"
#include "CParticleBeam.h"
#include "CParticleSystem.h"
#include "systems/graphics/SheepGraphics.h"
#include "../SheepUtil/include/SheepMath.h"

namespace Framework
{

  ParticleBeam::ParticleBeam()
  {
    transform = NULL;
    time = 0.0f;
    m_amount.m_startMin = 1.0f;
    m_rate.m_startMin = 30.0f;
    m_spawnOffset = Vec3();
  }

  ParticleBeam::~ParticleBeam()
  {
  }

  void ParticleBeam::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    ParticleSystem* system = (ParticleSystem*)space->GetComponent(eParticleSystem, owner);

    // and a particle system is actually on this object
    if (system)
      system->parentToOwner = parented;

    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(ParticleBeam::UpdateEmitter));
    space->hooks.Add("FrameUpdate", self, BUILD_FUNCTION(ParticleBeam::FrameUpdate));
  }

  void ParticleBeam::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("FrameUpdate", self);
  }

  // for seeing the particles in the editor
  void ParticleBeam::FrameUpdate(float dt)
  {
    if (ENGINE->m_editorActive)
      UpdateEmitter(dt);
  }

  // 
  void ParticleBeam::UpdateEmitter(float dt)
  {
    if (space->Paused())
      return;

    // if we are set to currently spawn
    if (spawning)
    {
      ParticleSystem* system = (ParticleSystem*)space->GetComponent(eParticleSystem, owner);

      // and a particle system is actually on this object
      if (system)
        SpawnParticle(dt, system);
    }
  }

  void ParticleBeam::SetSpawnOffset(Vec3& offset)
  {
    m_spawnOffset = offset;
    return;
  }

  // spawns at the specified rate and amount 
  void ParticleBeam::SpawnParticle(float dt, ParticleSystem* system)
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);
    int curDepth = trans->GetTranslation().z;
    time += dt;

    // this loop handles how many particles per second
    while (time > 0.0f)
    {

      // this loop handles how many particles per "burst"
      for (int i = 0; i < curDepth; ++i)
      {
        Vec3 spawnLocation = Vec3(0, 0, -i);

        Mat3D rot = Mat3D(trans->GetRotation());

        spawnLocation = rot * spawnLocation;

        Vec3 location;

        if (parented)
          location = m_spawnOffset;
        else
          location = trans->GetTranslation() + m_spawnOffset;

        Particle &particle = system->SpawnParticle(location + spawnLocation, true);

      }

      time -= 1.0f / (m_rate.m_startMin);
    }

    //system->ResetZ();
  }

  void ParticleBeam::Toggle(bool state)
  {
    spawning = state;
  }

}
