

#include "pch/precompiled.h"
#include "CParticleSystem.h"
#include "../SheepUtil/include/SheepMath.h"
#include "Handle.h"
#include "systems\anttweak\TweakHelper.h"
#include "systems/graphics/SheepGraphics.h"
#include "boost/random.hpp"

namespace Framework
{
  static float test;


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
    scale = GetRandom(scale_.m_startMin, scale_.m_startMax);
    color = GetRandom(color_.m_startMin, color_.m_startMax);
    life = GetRandom(life_.m_startMin, life_.m_startMax);
  }


  ParticleSystem::ParticleSystem()
  {
    if(ENGINE->m_editorAcitve)
      spawn = true;

    particleLife.m_startMin = 0.5f;
    particleLife.m_startMax = 1.0f;
    particleLife.m_endMin = 1.0f;
    particleLife.m_endMax = 1.0f;
    particleLife.ease = EaseLinear;

    velocity.m_startMin = Vec3(-10.0f, 30.0f, 0.0f);
    velocity.m_startMax = Vec3( 10.0f, 30.0f, 0.0f);
    velocity.m_endMin = Vec3(-10.0f, 80.0f, 0.0f);
    velocity.m_endMax = Vec3( 10.0f, 80.0f, 0.0f);
    velocity.ease = EaseNone;

    scale.m_startMin = 3.0f;
    scale.m_startMax = 5.0f;
    scale.m_endMin = 8.0f;
    scale.m_endMax = 10.0f;
    scale.ease = EaseLinear;

    color.m_startMin = Vec4(0.9f, 0.9f, 0.9f, 0.9f);
    color.m_startMax = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    color.m_endMin = Vec4( 0.9f, 0.125f, 0.125f, 0.1f);
    color.m_endMax = Vec4( 1.0f, 0.25f, 0.25f, 0.125f);
    color.ease = EaseLinear;

    speed.m_startMin = 1000.0f;
    speed.m_endMax = 0.0f;
  }

  void ParticleSystem::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    space->hooks.Add("FrameUpdate", self, BUILD_FUNCTION(ParticleSystem::FrameUpdate));
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(ParticleSystem::UpdateParticles));
    space->hooks.Add("Draw", self, BUILD_FUNCTION(ParticleSystem::DrawParticles));
  }

  void ParticleSystem::ToTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label)
  {
    ParticleSystem* ps = var.GetValue<ParticleSystem*>();

    AntTweak::Tweaker::DefaultTweak(bar, Variable(ps->velocity), "Velocity", "Velocity");

  }

  ParticleSystem::~ParticleSystem()
  {

  }

  /*----- Hooked functions ----- */
  
  void ParticleSystem::FrameUpdate(float dt)
  {
    if(ENGINE->m_editorAcitve)
      UpdateParticles(dt);
  }

  void ParticleSystem::UpdateParticles(float dt)
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);
    localRotation = Mat3D(trans->GetRotation());
    
    // change this so that the emitter handles this... system should just update the already existing particles
    if(spawn)
    {
      for(unsigned i = 0; i < 8; ++i)
      {
        float randomX = GetRandom(-10.0f, 10.0f);
        float randomY = GetRandom(-10.0f, 10.0f);
        Vec3D location = localRotation * (trans->GetTranslation() + Vec3D(randomX, randomY,0));
        Particle* temp = SpawnParticle(location);
        temp->velocity = localRotation * temp->velocity;
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

        //int rand = GetRandom(-1,1);

        // gets a value from 0 to 1.
        float time = 1.0f - (particles[i].life / particleLife.m_endMax);

        UpdateVelocity(i, time);
        UpdatePosition(i, dt);
        UpdateScale(i, time);
        UpdateColor(i, time);

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
      Draw::DrawTexturedRectRotated(particles[i].position.x, particles[i].position.y, particles[i].scale, particles[i].scale, particles[i].theta);
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
    
  Particle* ParticleSystem::SpawnParticle(const Vec3& location)
  {
    Particle temp(scale, color, particleLife);
    temp.position = location;
    temp.velocity = GetRandom(velocity.m_startMin, velocity.m_startMax);

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


  void ParticleSystem::UpdateVelocity(unsigned index, float t)
  {
    Vec3D averageMin = velocity.m_startMin * 0.5f + velocity.m_startMax * 0.5f;

    switch(velocity.ease)
    {
      // no ease, don't do anything
      case EaseNone:
        particles[index].velocity.Normalize();
        particles[index].velocity *= Ease::Linear(t, speed.m_startMin, speed.m_endMax);
        break;

      case EaseLinear:
        particles[index].velocity = localRotation * Ease::Linear(t, (Vec3D)particles[index].velocity, GetRandom(velocity.m_endMin, velocity.m_endMax));
        break;

      case EaseQuadraticIn:
        break;

      case EaseQuadraticOut:
        break;

      default:
        break;
    }
  }

  void ParticleSystem::UpdatePosition(unsigned index, float dt)
  {
    particles[index].position += particles[index].velocity * dt;
  }

  void ParticleSystem::UpdateScale(unsigned index, float t)
  {
    float averageMin = scale.m_startMin * 0.5f + scale.m_startMax * 0.5f;

    switch(scale.ease)
    {
      case EaseNone:
        break;

      case EaseLinear:
        particles[index].scale = Ease::Linear(t, averageMin, GetRandom(scale.m_endMin, scale.m_endMax));
        break;

      default:
        break;
    }
  }

  void ParticleSystem::UpdateRotation(unsigned index, float t)
  {

  }

  void ParticleSystem::UpdateColor(unsigned index, float t)
  {
    Vec4D averageMin = color.m_startMin * 0.5f + color.m_startMax * 0.5f;

    switch(color.ease)
    {
      case EaseNone:
        break;

      case EaseLinear:
        particles[index].color = Ease::Linear(t, averageMin, GetRandom(color.m_endMin, color.m_endMax));
        break;

      default:
        break;
    }
  }


}