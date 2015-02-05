/*****************************************************************
Filename: CParticleSystem.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CParticleSystem.h"
#include "../SheepUtil/include/SheepMath.h"
#include "Handle.h"
#include "systems\anttweak\TweakHelper.h"
#include "systems/graphics/SheepGraphics.h"
#include "boost/random.hpp"

namespace Framework
{
  // default ctor, gives basic initial values
  ParticleSystem::ParticleSystem()
  {
    particles.clear();

    particleLife.m_startMin = 0.5f;
    particleLife.m_startMax = 1.0f;

    direction.m_startMin = Vec3(-1.0f, 1.0f, 0.0f);
    direction.m_startMax = Vec3( 1.0f, 1.0f, 0.0f);
    direction.m_endMin = Vec3(0.0f, 1.0f, 0.0f);
    direction.m_endMax = Vec3(0.0f, 1.0f, 0.0f);
    directionEase = 1;

    scale.m_startMin = 3.0f;
    scale.m_startMax = 5.0f;
    scale.m_endMin = 8.0f;
    scale.m_endMax = 10.0f;
    scaleEase = 1;

    color.m_startMin = Vec4(0.9f, 0.9f, 0.9f, 0.9f);
    color.m_startMax = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    color.m_endMin = Vec4( 0.9f, 0.125f, 0.125f, 0.1f);
    color.m_endMax = Vec4( 1.0f, 0.25f, 0.25f, 0.125f);
    colorEase = 1;

    speed.m_startMin = 100.0f;
    speed.m_startMax = 300.0f;
    speed.m_endMin = 0.0f;
    speed.m_endMax = 0.0f;
    speedEase = 1;

    textureName = "White.png";
  }

  // hooks functions to the hook system
  void ParticleSystem::Initialize()
  {
    transform = this->GetOwner()->GetComponentHandle(eTransform);

    space->hooks.Add("FrameUpdate", self, BUILD_FUNCTION(ParticleSystem::FrameUpdate));
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(ParticleSystem::UpdateParticles));
    space->hooks.Add("Draw", self, BUILD_FUNCTION(ParticleSystem::DrawParticles));
  }

  // Currently not used
  void ParticleSystem::ToTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label)
  {
    ParticleSystem* ps = var.GetValue<ParticleSystem*>();

    AntTweak::Tweaker::DefaultTweak(bar, Variable(ps->direction), "ParticleDirection", "Particle Direction");

  }

  ParticleSystem::~ParticleSystem()
  {

  }

  // remove from the hook system
  void ParticleSystem::Remove()
  {
    particles.clear();
    space->hooks.Remove("FrameUpdate", self);
    space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("Draw", self);
  }

  /*----- Hooked functions ----- */
  
  void ParticleSystem::FrameUpdate(float dt)
  {
    // only update the particles if the editor is active so we can see them
    if(ENGINE->m_editorAcitve && !ENGINE->PlayingInEditor())
    {
      // if antweak bar changed the direction vectors
      if(directionChange)
      {
        // normalize all of them... sigh
        direction.m_startMin.Normalize();
        direction.m_startMax.Normalize();
        direction.m_endMin.Normalize();
        direction.m_endMax.Normalize();
        directionChange = false;
      }

      // just call the normal update function
      UpdateParticles(dt);
    }
  }

  void ParticleSystem::UpdateParticles(float dt)
  {
    // need the transform of the current object to get local rotation
    Transform* trans = space->GetHandles().GetAs<Transform>(transform);
    localRotation = Mat3D(trans->GetRotation());

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

        // gets a value from 0 to 1 depending on how much life is left
        float time = 1.0f - (particles[i].life / particles[i].endLife);

        // update all of the fields
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
    DirectSheep::Handle texHandle =
      DirectSheep::Handle(DirectSheep::TEXTURE,
      Draw::GetTextureID(textureName.c_str()));

    // also set the texture
    Draw::SetTexture(Draw::GetTextureID(textureName.c_str()));

    // but also get the scale of the texture for normalizing the scale for particles
    Vec2 scale = GRAPHICS->GetTextureDim(texHandle);

    for(unsigned i = 0; i < particles.size(); ++i)
    {
      Vec4 color = particles[i].currentColor;
      Draw::SetColor(color.x, color.y, color.z, color.w);

      //                                x screen coords    ,    y screen coords     ,  particle scale / texture scale x,    particle scale / scale y          , rotation 
      Draw::DrawTexturedRectRotated(particles[i].position.x, particles[i].position.y, particles[i].currentScale / scale.x, particles[i].currentScale / scale.y, particles[i].theta);
    }
  }
    
  void ParticleSystem::DestroyParticles(void)
  {
    particles.clear();
  }
    
  /*----- Setters (mostly unused) ----- */
    
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
  
  // Spawns a particle at the passed in location. 
  Particle& ParticleSystem::SpawnParticle(const Vec3& location, bool setDirection)
  {
    ParticleOption<Vec3> newDir;

    // If the system is handling the direction of the particle
    if(setDirection)
    {
      newDir.m_startMin = localRotation * direction.m_startMin;
      newDir.m_startMax = localRotation * direction.m_startMax;
      newDir.m_endMin = localRotation * direction.m_endMin;
      newDir.m_endMax = localRotation * direction.m_endMax;

      Particle temp(scale, color, newDir, speed, particleLife);
      temp.position = location;
      particles.push_back(temp);
      return particles.back();
    }
    else // else the other component is handling the direction of the particle
    {
      //Particle temp(scale, color, direction, speed, particleLife);
      //temp.position = location;
      particles.emplace_back(scale, color, direction, speed, particleLife);

      auto it = particles.end();

      Particle& part = particles[particles.size() - 1]; //particles.back();
      part.position = location;

      return part;
    }
  }

  // Moves the right most particle in the vector to the index
  void ParticleSystem::RemoveParticle(unsigned index)
  {
    if(index < particles.size())
    {
      particles[index] = particles.back();
      particles.pop_back();
      int fuck = particles.size();
      fuck = fuck;
    }

    return;
  }

  // Uses eases to update the direction of the particle. Ease = 0 is most efficient, since it does nothing
  void ParticleSystem::UpdateDirection(unsigned index, float t)
  {
    switch(directionEase)
    {
      // no ease, don't do anything
      case EaseNone:
        break;

      case EaseLinear:
        particles[index].currentDirection = Ease::Linear(t, particles[index].direction, particles[index].endDirection);
        break;

      case EaseQuadraticIn:
        particles[index].currentDirection = Ease::QuadraticIn(t, particles[index].direction, particles[index].endDirection);
        break;

      case EaseQuadraticOut:
        particles[index].currentDirection = Ease::QuadraticOut(t, particles[index].direction, particles[index].endDirection);
        break;

      default:
        break;
    }
  }

  // Updates the speed depending on the type of ease
  void ParticleSystem::UpdateSpeed(unsigned index, float t)
  {
    switch(speedEase)
    {
      // no ease, don't do anything
      case EaseNone:
        break;

      case EaseLinear:
        particles[index].currentSpeed = Ease::Linear(t, particles[index].speed, particles[index].endSpeed);
        break;

      case EaseQuadraticIn:
        particles[index].currentSpeed = Ease::QuadraticIn(t, particles[index].speed, particles[index].endSpeed);
        break;

      case EaseQuadraticOut:
        particles[index].currentSpeed = Ease::QuadraticOut(t, particles[index].speed, particles[index].endSpeed);
        break;

      default:
        break;
    }
  }

  // updates the position of the particle by it's velocity
  void ParticleSystem::UpdatePosition(unsigned index, float dt)
  {
    particles[index].position += particles[index].currentDirection * particles[index].currentSpeed * dt;
  }

  // updates the scale of the particle based on the scaling parameters
  void ParticleSystem::UpdateScale(unsigned index, float t)
  {
    switch(scaleEase)
    {
      case EaseNone:
        break;

      case EaseLinear:
        particles[index].currentScale = Ease::Linear(t, particles[index].scale, particles[index].endScale);
        break;

      case EaseQuadraticIn:
        particles[index].currentScale = Ease::QuadraticIn(t, particles[index].scale, particles[index].endScale);
        break;

      case EaseQuadraticOut:
        particles[index].currentScale = Ease::QuadraticOut(t, particles[index].scale, particles[index].endScale);
        break;

      default:
        break;
    }
  }

  void ParticleSystem::UpdateRotation(unsigned index, float t)
  {
    // does nothing right now... rotation is currently random
    return;
  }

  // updates the color of the particle based on the color parameters
  void ParticleSystem::UpdateColor(unsigned index, float t)
  {
    switch(colorEase)
    {
      case EaseNone:
        break;

      case EaseLinear:
        particles[index].currentColor = Ease::Linear(t, particles[index].color, particles[index].endColor);
        break;

      case EaseQuadraticIn:
        particles[index].currentColor = Ease::QuadraticIn(t, particles[index].color, particles[index].endColor);
        break;

      case EaseQuadraticOut:
        particles[index].currentColor = Ease::QuadraticOut(t, particles[index].color, particles[index].endColor);
        break;

      default:
        break;
    }
  }
}