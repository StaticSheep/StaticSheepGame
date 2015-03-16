/*****************************************************************
Filename: CBasicPSystem.cpp
Project:
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CBasicPSystem.h"
#include "systems/graphics/DrawLib.h"
#include "SheepMath.h"
#include "components/transform/CTransform.h"
#include "systems/graphics/SheepGraphics.h"
#include "Context/Context.h"

namespace Framework
{
  BasicParticleSystem::BasicParticleSystem()
  {
    m_scaling.scale = Vec2(32, 32);
  }
  
  BasicParticleSystem::~BasicParticleSystem()
  {

  }

  void BasicParticleSystem::Initialize()
  {
    SetTexture(m_texture);

    if (ENGINE->m_editorActive)
    {
      space->hooks.Add("FrameUpdate", self,
        BUILD_FUNCTION(BasicParticleSystem::LogicUpdate));
    }
    else
    {
      space->hooks.Add("LogicUpdate", self,
        BUILD_FUNCTION(BasicParticleSystem::LogicUpdate));
    }

    if (m_hooked)
    {
      space->hooks.Remove("PreDraw", self);
      space->hooks.Remove("Draw", self);
      space->hooks.Remove("PostDraw", self);
    }

    switch (m_layer)
    {
    case 2:
      space->hooks.Add("PostDraw", self, BUILD_FUNCTION(BasicParticleSystem::Draw));
      break;
    case 0:
      space->hooks.Add("PreDraw", self, BUILD_FUNCTION(BasicParticleSystem::Draw));
      break;
    case 1:
    default:
      space->hooks.Add("Draw", self, BUILD_FUNCTION(BasicParticleSystem::Draw));
      break;
    }

    m_hooked = true;
    
    m_nextSpawn = 1.0f / m_system.spawnInterval;
  }

  void BasicParticleSystem::Remove()
  {
    switch (m_layer)
    {
    case 2:
      space->hooks.Remove("PostDraw", self);
      break;
    case 0:
      space->hooks.Remove("PreDraw", self);
      break;
    case 1:
    default:
      space->hooks.Remove("Draw", self);
      break;
    }

    space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("FrameUpdate", self);
  }

  void BasicParticleSystem::SetTexture(std::string texname)
  {
    m_texture = texname;
    m_textureID = Draw::GetTextureID(texname.c_str());
  }

  void BasicParticleSystem::TweakSetTexture(const void* texString)
  {
    SetTexture(*(std::string*)texString);
  }

  void BasicParticleSystem::LogicUpdate(float dt)
  {
    

    int i;

    if (m_system.active && !(ENGINE->m_editorActive && !m_system.editorActive))
    {
      m_nextSpawn -= dt;

      if (m_nextSpawn < 0.0f)
      {
        for (i = 0; i < m_system.spawnAmount; ++i)
          SpawnParticle();

        m_nextSpawn += 1.0f / m_system.spawnInterval;
      }
    }

    std::stack<int> removals;

    // Update and remove all stuff
    for (i = 0; i < m_particles.size(); ++i)
    {
      Particle& prt = m_particles[i];

      prt.life -= dt;
      
      prt.pos += prt.velocity * dt;

      if (prt.life < 0.0f)
        removals.push(i);
    }

    while (!removals.empty())
    {
      i = removals.top();
      removals.pop();

      // Fill with the right-most element
      if (i != m_particles.size() - 1)
      {
        Particle pt = m_particles[m_particles.size() - 1];
        m_particles.pop_back();
        m_particles[i] = pt;
      }
      else
        m_particles.pop_back();
        

      
    }
  }

  void BasicParticleSystem::SpawnParticle()
  {
    Transform* trans = space->GetGameObject(owner)->
      GetComponent<Transform>(eTransform);

    

    Particle prt;

    prt.life = m_life.lifetime + GetRandom(-m_life.randLifetime, m_life.randLifetime);
    prt.maxLife = prt.life;
    
    Vec2 center;
    
    if (m_system.parentToOwner)
      center = m_pos.offset;
    else
      center = trans->GetTranslation() + m_pos.offset;

    if (m_pos.square)
    {
      if (m_pos.edgeOnly)
      {
        bool min = rand() % 2;

        switch (min)
        {
        case true:
          prt.pos.x = m_pos.randOffset.x;
          break;
        default:
          prt.pos.x = -m_pos.randOffset.x;
          break;
        }

        min = rand() % 2;

        switch (min)
        {
        case true:
          prt.pos.y = m_pos.randOffset.y;
          break;
        default:
          prt.pos.y = -m_pos.randOffset.y;
          break;
        }

        prt.pos += center;
      }
      else
      {
        prt.pos = center;
        prt.pos += GetRandom(-m_pos.randOffset, m_pos.randOffset);
      }
    }
    else
    {
      float radius;

      switch (m_pos.edgeOnly)
      {
      case true:
        radius = GetRandom(0.0f, m_pos.radius);
        break;
      default:
        radius = m_pos.radius;
        break;
      }

      float angle = GetRandom(0, 360 / (int)(m_pos.angle * m_pos.angle));
      angle += GetRandom(-m_pos.randAngle, m_pos.randAngle);

      prt.pos = center;
      prt.pos.x += cos(angle * (3.14f / 180.0f)) * radius;
      prt.pos.y += sin(angle * (3.14f / 180.0f)) * radius;
    }

    prt.velocity = GetRandom(-m_vel.randVelocity, m_vel.randVelocity);
    prt.velocity += m_vel.velocity;

    if (m_vel.speedIn)
    {
      float speed = GetRandom(-m_vel.randSpeedIn, m_vel.randSpeedIn);
      Vec2 toCenter = center - prt.pos;
      
      speed += m_vel.speedIn;
      prt.velocity += toCenter * speed;
    }
      

    prt.rotation = m_rot.rotation + GetRandom(-m_rot.randRotation, m_rot.randRotation);

    prt.scale = m_scaling.scale + GetRandom(-m_scaling.randScale, m_scaling.randScale);

    if (m_fade.fadeOut)
    {
      prt.fadeStart = m_fade.fadeOutTime +
        GetRandom(-m_fade.randFadeoutTime, m_fade.randFadeoutTime);
    }

    m_particles.push_back(prt);
  }

  void BasicParticleSystem::Draw()
  {
    

    DirectSheep::Handle texHandle(DirectSheep::TEXTURE, m_textureID);

    GRAPHICS->SetColor(Vec4(1, 1, 1, 1));
    GRAPHICS->SetUV(Vec2(0, 0), Vec2(1, 1));

    Vec2 scaleMult = GRAPHICS->GetTextureDim(texHandle);
    float z = 0;
    float zStep = 0.2f / m_particles.size();


    Vec2 center;

    if (m_system.parentToOwner)
    {
      Transform* pTrans = space->GetGameObject(owner)->GetComponent<Transform>(eTransform);
      if (pTrans)
        center = pTrans->GetTranslation();
    }

    for (int i = 0; i < m_particles.size(); ++i)
    {
      Particle& prt = m_particles[i];

      GRAPHICS->SetSize(prt.scale.x / scaleMult.x,
        prt.scale.y / scaleMult.x);
      GRAPHICS->SetPosition(center.x + prt.pos.x, center.y + prt.pos.y, z += zStep);

      if (m_fade.fadeOut)
      {
        GRAPHICS->RC()->SetAlpha(min(prt.life / prt.fadeStart, 1.0f));
      }
      
      GRAPHICS->SetRotation(prt.rotation);

      GRAPHICS->DrawBatched(texHandle);
    }

  }

  void BasicParticleSystem::Toggle(bool state)
  {
    m_system.active = state;

    if (state)
      m_nextSpawn = 0;
  }
}