

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
    scale = 1.0f;
    endScale = 1.0f;
    color = Vec4();
    endColor = Vec4();
    direction = Vec3();
    endDirection = Vec3();
    speed = 0.0f;
    endSpeed = 0.0f;
    theta = 0.0f;
    life = 1.0f;
    endLife = 1.0f;
  }

  Particle::Particle(ParticleOption<float>& scale_,
                     ParticleOption<Vec4>&  color_,
                     ParticleOption<Vec3>&  dir_,
                     ParticleOption<float>& speed_,
                     ParticleOptionShort<float>& life_)
  {
    scale = GetRandom(scale_.m_startMin, scale_.m_startMax);
    endScale = GetRandom(scale_.m_endMin, scale_.m_endMax);

    color = GetRandom(color_.m_startMin, color_.m_startMax);
    endColor = GetRandom(color_.m_endMin, color_.m_endMax);

    direction = GetRandomNormalizedVector(dir_.m_startMin, dir_.m_startMax);
    endDirection = GetRandomNormalizedVector(dir_.m_endMin, dir_.m_endMax);

    speed = GetRandom(speed_.m_startMin, speed_.m_startMax);
    endSpeed = GetRandom(speed_.m_endMin, speed_.m_endMax);

    life = GetRandom(life_.m_startMin, life_.m_startMax);
    endLife = life;

    theta = GetRandom(0.0f, 2.0f * PI);
  }


  ParticleSystem::ParticleSystem()
  {
    if(ENGINE->m_editorAcitve)
      spawn = true;

    particleLife.m_startMin = 0.5f;
    particleLife.m_startMax = 1.0f;

    direction.m_startMin = Vec3(-1.0f, 1.0f, 0.0f);
    direction.m_startMax = Vec3( 1.0f, 1.0f, 0.0f);
    direction.m_endMin = Vec3(0.0f, 1.0f, 0.0f);
    direction.m_endMax = Vec3(0.0f, 1.0f, 0.0f);
    directionEase = EaseLinear;

    scale.m_startMin = 3.0f;
    scale.m_startMax = 5.0f;
    scale.m_endMin = 8.0f;
    scale.m_endMax = 10.0f;
    scaleEase = EaseLinear;

    color.m_startMin = Vec4(0.9f, 0.9f, 0.9f, 0.9f);
    color.m_startMax = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    color.m_endMin = Vec4( 0.9f, 0.125f, 0.125f, 0.1f);
    color.m_endMax = Vec4( 1.0f, 0.25f, 0.25f, 0.125f);
    colorEase = EaseLinear;

    rate.m_startMin = 120.0f;
    amount.m_startMin = 1.0f;

    speed.m_startMin = 100.0f;
    speed.m_startMax = 300.0f;
    speed.m_endMin = 0.0f;
    speed.m_endMax = 0.0f;
    speedEase = EaseLinear;

    textureName = "White.png";
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

    AntTweak::Tweaker::DefaultTweak(bar, Variable(ps->direction), "ParticleDirection", "Particle Direction");

  }

  ParticleSystem::~ParticleSystem()
  {

  }

  void ParticleSystem::Remove()
  {
    space->hooks.Remove("FrameUpdate", self);
    space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("Draw", self);
  }

  /*----- Hooked functions ----- */
  
  void ParticleSystem::FrameUpdate(float dt)
  {
    if(ENGINE->m_editorAcitve)
    {
      if(directionChange)
      {
        direction.m_startMin.Normalize();
        direction.m_startMax.Normalize();
        direction.m_endMin.Normalize();
        direction.m_endMax.Normalize();
        directionChange = false;
      }

      UpdateParticles(dt);
    }
  }

  void ParticleSystem::UpdateParticles(float dt)
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);
    localRotation = Mat3D(trans->GetRotation());

    time += dt;
    
    // change this so that the emitter handles this... system should just update the already existing particles
    if(spawn)
    {
      while(time > 0.0f)
      {
        for(int i = 0; i < (int)amount.m_startMin; ++i)
        {
          Vec3D location = localRotation * (trans->GetTranslation());// + Vec3D(randomX, randomY,0));
          Particle* temp = SpawnParticle(location);
        }

        time -= 1.0f / (rate.m_startMin);
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

        // gets a value from 0 to 1.
        //float time = 1.0f - (particles[i].life / particles[i].endLife);
        float time = dt / particles[i].endLife;

        UpdateDirection(i, time);
        UpdateSpeed(i, time);
        UpdatePosition(i, dt);
        UpdateScale(i, time);
        UpdateColor(i, time);

      }
    }
  }

  void ParticleSystem::DrawParticles()
  {
    DirectSheep::Handle handle = GRAPHICS->SetTexture(textureName.c_str());
    Draw::SetTexture(Draw::GetTextureID(textureName.c_str()));
    Vec2 scale = GRAPHICS->GetTextureDim(handle);

    //Draw::SetTexture(Draw::GetTextureID(textureName.c_str()));
    
    

    for(unsigned i = 0; i < particles.size(); ++i)
    {
      Vec4 color = particles[i].color;

      Draw::SetColor(color.x, color.y, color.z, color.w);
      Draw::DrawTexturedRectRotated(particles[i].position.x, particles[i].position.y, particles[i].scale / scale.x, particles[i].scale / scale.y, particles[i].theta);
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

  void ParticleSystem::SetLifetime(ParticleOptionShort<float>& op)
  {
    particleLife = op;
  }

  void ParticleSystem::SetSpawnRate(ParticleOptionShort<float>& op)
  {
    rate = op;
  }

  void ParticleSystem::SetDirection(void* op)
  {
    *(Vec3*)((char*)this + AntTweak::Tweaker::currentGeneric->realOffset) = *(Vec3*)op;

    ((Vec3*)((char*)this + AntTweak::Tweaker::currentGeneric->realOffset))->Normalize();
  }

  void ParticleSystem::SetSpeed(ParticleOption<float>& op)
  {
    speed = op;
  }

  void ParticleSystem::SetScale(ParticleOption<float>& op)
  {
    scale = op;
  }

  void ParticleSystem::SetVortex(float something, float something2)
  {
    return;
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

  void ParticleSystem::TweakSetDirection(const void* value)
  {
    SetDirection((Vec3*)value);
  }
    
  Particle* ParticleSystem::SpawnParticle(const Vec3& location)
  {
    ParticleOption<Vec3> newDir;
    newDir.m_startMin = localRotation * direction.m_startMin;
    newDir.m_startMax = localRotation * direction.m_startMax;
    newDir.m_endMin = localRotation * direction.m_endMin;
    newDir.m_endMax = localRotation * direction.m_endMax;

    Particle temp(scale, color, newDir, speed, particleLife);
    temp.position = location;
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

  void ParticleSystem::UpdateDirection(unsigned index, float t)
  {
    switch(directionEase)
    {
      // no ease, don't do anything
      case EaseNone:
        break;

      case EaseLinear:
        particles[index].direction = Ease::Linear(t, particles[index].direction, particles[index].endDirection);
        break;

      case EaseQuadraticIn:
        particles[index].direction = Ease::QuadraticIn(t, particles[index].direction, particles[index].endDirection);
        break;

      case EaseQuadraticOut:
        particles[index].direction = Ease::QuadraticOut(t, particles[index].direction, particles[index].endDirection);
        break;

      default:
        break;
    }
  }

  void ParticleSystem::UpdateSpeed(unsigned index, float t)
  {
    switch(speedEase)
    {
      // no ease, don't do anything
      case EaseNone:
        break;

      case EaseLinear:
        particles[index].speed = Ease::Linear(t, particles[index].speed, particles[index].endSpeed);
        break;

      case EaseQuadraticIn:
        particles[index].speed = Ease::QuadraticIn(t, particles[index].speed, particles[index].endSpeed);
        break;

      case EaseQuadraticOut:
        particles[index].speed = Ease::QuadraticOut(t, particles[index].speed, particles[index].endSpeed);
        break;

      default:
        break;
    }
  }

  void ParticleSystem::UpdatePosition(unsigned index, float dt)
  {
    particles[index].position += particles[index].direction * particles[index].speed * dt;
  }

  void ParticleSystem::UpdateScale(unsigned index, float t)
  {
    switch(scaleEase)
    {
      case EaseNone:
        break;

      case EaseLinear:
        particles[index].scale = Ease::Linear(t, particles[index].scale, particles[index].endScale);
        break;

      case EaseQuadraticIn:
        particles[index].scale = Ease::QuadraticIn(t, particles[index].scale, particles[index].endScale);
        break;

      case EaseQuadraticOut:
        particles[index].scale = Ease::QuadraticOut(t, particles[index].scale, particles[index].endScale);
        break;

      default:
        break;
    }
  }

  void ParticleSystem::UpdateRotation(unsigned index, float t)
  {
    // does nothing right now
    return;
  }

  void ParticleSystem::UpdateColor(unsigned index, float t)
  {

    switch(colorEase)
    {
      case EaseNone:
        break;

      case EaseLinear:
        particles[index].color = Ease::Linear(t, particles[index].color, particles[index].endColor);
        break;

      case EaseQuadraticIn:
        particles[index].color = Ease::QuadraticIn(t, particles[index].color, particles[index].endColor);
        break;

      case EaseQuadraticOut:
        particles[index].color = Ease::QuadraticOut(t, particles[index].color, particles[index].endColor);
        break;

      default:
        break;
    }
  }


}