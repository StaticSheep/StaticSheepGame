

#include "pch/precompiled.h"
#include "CParticleSystem.h"
#include "../SheepUtil/include/SheepMath.h"
#include "Handle.h"
#include "systems\anttweak\TweakHelper.h"
#include "systems/graphics/SheepGraphics.h"
#include "boost/random.hpp"

namespace Framework
{
  Particle::Particle()
  {
    position = Vec3();
    scale = 0.0f;
    color = Vec4();
    velocity = Vec3();
    angularVelocity = 0.0f;
    theta = 0.0f;
    life = 0.0f;
  }

  Particle::Particle(ParticleOption<float>& scale_, 
                     ParticleOption<Vec4>& color_,
                     ParticleOption<float>& life_)
  {
    scale = scale_.min;
    color = color_.min;
    life = life_.min + (GetRandom(-life_.slop * 256, life_.slop * 256) / 256.0f);
  }


  ParticleSystem::ParticleSystem()
  {
    spawn = true;
    rate.min = 0.125f;
    rate.max = 0.125f;
    rate.slop = 0.0f;

    particleLife.min = 0.25f;
    particleLife.max = 1.0f;
    particleLife.slop = 1.0f;

    scale.min = 5.0f;
    scale.max = 8.0f;
    scale.slop = 1.0f;

    velocity.min = Vec3(0.0f, 1.0f, 0.0f);
    velocity.max = Vec3(0.0f, 3.0f, 0.0f);
    velocity.slop = Vec3(2.0f, 0.0f, 0.0f);

    color.min = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    color.max = Vec4(1.0f, 0.125f, 0.25f, 0.25f);

    
  }

  void ParticleSystem::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(ParticleSystem::UpdateParticles));
    space->hooks.Add("Draw", self, BUILD_FUNCTION(ParticleSystem::DrawParticles));
  }

  ParticleSystem::~ParticleSystem()
  {

  }

  /*----- Hooked functions ----- */
    
  void ParticleSystem::UpdateParticles(float dt)
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    if(spawn)
    {
      for(unsigned i = 0; i < 4; ++i)
      {
        Particle* temp = SpawnParticle(trans->GetTranslation());
        int rand = GetRandom(-2,2);
        temp->velocity = Vec3(5.0f * rand, 0.0f, 0.0f);
      }
      
    }

    // if there are particles to update...
    if(particles.size())
    {
      for(unsigned i = 0; i < particles.size(); ++i)
      {
        particles[i].life -= dt;

        if(particles[i].life <= 0)
        {
          RemoveParticle(i);
          if(i >= particles.size())
            break;
        }

        int rand = GetRandom(-1,1);

        particles[i].velocity = Ease::Linear(1.0f - particles[i].life / particleLife.max, velocity.min + (rand * velocity.slop), velocity.max + (rand * velocity.slop));
        particles[i].position += particles[i].velocity;
        particles[i].scale = Ease::Linear(1.0f - (particles[i].life / particleLife.max), scale.min + (rand * scale.slop), scale.max + (rand * scale.slop));
        particles[i].color = Ease::Linear(1.0f - particles[i].life / particleLife.max, color.min, color.max);

      }
    }
  }

  void ParticleSystem::DrawParticles()
  {
    
    Draw::SetTexture(Draw::GetTextureID("White.png"));
    

    for(unsigned i = 0; i < particles.size(); ++i)
    {
      Vec4 color = particles[i].color;

      Draw::SetColor(color.x, color.y, color.z, color.w);
      Draw::DrawTexturedRect(particles[i].position.x, particles[i].position.y, particles[i].scale, particles[i].scale);
    }
  }
    
  /*----- Toggles ----- */
    
  void ParticleSystem::Toggle(bool state)
  {
    spawn = state;
  }

  void ParticleSystem::ToggleTimed(float t)
  {
    timedSpawn = true;
    time = t;
  }
    
  void ParticleSystem::DestroyParticles(void)
  {
    for(unsigned i = 0; i < particles.size(); ++i)
    {
      particles[i] = Particle();
    }
  }
    
  /*----- Setters ----- */
    
  void ParticleSystem::SetTexture(unsigned int newTexture)
  {
    textureID = newTexture;
  }

  void ParticleSystem::SetLifetime(ParticleOption<float>& op)
  {
    particleLife = op;
  }

  void ParticleSystem::SetSpawnRate(ParticleOption<float>& op)
  {
    rate = op;
  }

  void ParticleSystem::SetVelocity(ParticleOption<Vec3>& op)
  {
    velocity = op;
  }

  void ParticleSystem::SetScale(ParticleOption<float>& op)
  {
    scale = op;
  }

  void ParticleSystem::SetVortex(float something, float something2)
  {
    return;
  }

  void ParticleSystem::SetDirection(ParticleOption<Vec3>& dir)
  {
    direction = dir;
  }

  void ParticleSystem::SetAngularVelocity(ParticleOption<float> op)
  {
    angularVelocity = op;
  }

  void ParticleSystem::SetColor(ParticleOption<Vec4>& op)
  {
    color = op;
  }
    
  void ParticleSystem::SetGravity(bool state)
  {
    gravity = state;
  }

  void ParticleSystem::SetGravityPull(Vec3& strength)
  {
    gravityPull = strength;
  }
    
  Particle* ParticleSystem::SpawnParticle(Vec3& location)
  {
    Particle temp(scale, color, particleLife);
    temp.position = location;
    temp.velocity = velocity.min;

    particles.push_back(temp);
    return &particles.back();
  }

  void ParticleSystem::RemoveParticle(unsigned index)
  {
    if(index < particles.size())
    {
      particles[index] = particles.back();
      particles.pop_back();
    }

    return;
  }

  void ParticleSystem::ClearParticles(void)
  {

  }

}