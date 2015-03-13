/*****************************************************************
Filename: CBasicPSystem.h
Project:
Author(s): Zachary Nawar

Thanks to Judy Cheng

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "components/base/Component.h"
#include "types/vectors/Vec2.h"

namespace Framework
{

  class BasicParticleSystem : public GameComponent
  {
  public:
    BasicParticleSystem();
    ~BasicParticleSystem();

    void Initialize();
    void Remove();

    struct BPSPos
    {
      Vec2 offset;
      Vec2 randOffset;

      bool square;

      // Spawn on edge only
      bool edgeOnly;

      // Circle spawning radius
      float radius;

      // Angular spawning
      float angle = 1.0f;
      float randAngle;
    } m_pos;

    struct BPSScaling
    {
      Vec2 scale;
      Vec2 randScale;
    } m_scaling;

    struct BPSRotation
    {
      float rotation;
      float randRotation;
    } m_rot;

    struct BPSLife
    {
      // Life time of the particle
      float lifetime = 0.1f;

      // Random life time
      float randLifetime;
    } m_life;

    struct BPSFade
    {
      // Checks if it is fading
      bool fadeOut;

      // Time to fade out
      float fadeOutTime;

      // Random fade out time
      float randFadeoutTime;
    } m_fade;

    struct BPSVelocity
    {
      // Velocity of the particle
      Vec2 velocity;

      // Random velocity
      Vec2 randVelocity;

      // Speed Inwards
      float speedIn;
      float randSpeedIn;
    } m_vel;

    struct BPSSystem
    {
      // Number of intervals per second
      float spawnInterval = 1;

      // Number of particles per interval
      float spawnAmount = 15;

      // Number of particles per second
      float spawnPerSecond = 0;

      // Check if active or not
      bool active = true;
      bool editorActive = true;

      // When to go inactive
      float shutdownTimer = 0;

      // If shut down time is used 
      bool useShutdownTime = false;

      // Parent particles to owner
      bool parentToOwner = false;
    } m_system;

    

    void LogicUpdate(float dt);
    void Draw();

    void Toggle(bool state);

    void SetTexture(std::string texname);
    void TweakSetTexture(const void* texString);


    int m_layer = 1;
    bool m_hooked = false;

    // Never write directly to this!
    std::string m_texture = "Default.png";

  protected:
    struct Particle
    {
      Vec2 pos;
      Vec2 scale;
      float rotation = 0;
      float life = 0;
      float maxLife = 0;
      float fadeStart = 0;
      Vec2 velocity;
    };

    void SpawnParticle();

    
    unsigned m_textureID;

  private:
    // When to spawn the next
    float m_nextSpawn;

    std::vector<Particle> m_particles;
    
  };

  typedef BasicParticleSystem BPartSys;
}