/*****************************************************************
Filename: CParticleSystem.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "components/base/Component.h"
#include "../SheepUtil/include/Matrix3D.h"
#include <string>
#include <vector>

// todo, write the ease functions
// ParticleSystem spawns particles and manages them based on it's options
// ParticleBoxEmitter handles where to spawn the particles
// templatize the particle options

namespace Framework
{

  enum Eases
  {
    EaseNone = 0,
    EaseLinear,
    EaseQuadraticIn,
    EaseQuadraticOut,
    EaseCubicIn,
    EaseCubicOut,
    Catspline
  };

  template<typename T>
  struct ParticleOption
  {
    T m_startMin;
    T m_startMax;
    T m_endMin;
    T m_endMax;
    Eases ease;
  };

  struct Particle
  {
    Particle();
    Particle(ParticleOption<float>& scale, 
             ParticleOption<Vec4>&  color,
             ParticleOption<float>& life);

    Vec3 position;
    float scale;
    Vec4 color;
    Vec3 velocity;
    float angularVelocity;
    float theta;
    float life;
  };

  // interpolation crap
  

  // class ParticleBoxEmitter handles initial spawn location
  // class ParticleCircleEmitter "  "
  
  

  // handles spawning of particles and updating them...
  class ParticleSystem : public GameComponent
  {
    public:

      ParticleSystem();
      ~ParticleSystem();
    
      void Initialize();

      static void ToTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label);

      /*----- Hooked functions ----- */
    
      void UpdateParticles(float dt); // update behavior
      void FrameUpdate(float dt);
      void DrawParticles(); // tell batcher to draw the things
    
      /*----- Toggles ----- */
    
      void Toggle(bool state);
      void ToggleTimed(float time); // toggle on for a specified amount of time
    
      void DestroyParticles(void);
    
      /*----- Setters ----- */
    
      void SetTexture(unsigned int newTexture);
      void SetLifetime(ParticleOption<float>& op);
      void SetSpawnRate(ParticleOption<float>& op);
      void SetVelocity(ParticleOption<Vec3>& op);
      void SetScale(ParticleOption<float>& op);
      void SetVortex(float something, float something2); // some sort of spinning
      void SetDirection(ParticleOption<Vec3>& dir); // normalize it first bitch
      void SetAngularVelocity(ParticleOption<float>);
      void SetColor(ParticleOption<Vec4>& op);
    
      void SetGravity(bool state);
      void SetGravityPull(Vec3& strength);

      std::string textureName;
      ParticleOption<float> particleLife;     // how long particles live
      ParticleOption<float> rate;             // spawn rate
      ParticleOption<float> amount;           // how many particles per tick
      ParticleOption<Vec3> velocity;          // velocity of the particles
      ParticleOption<float> scale;            // start to end scale of particles
      ParticleOption<Vec3> direction;         // direction to spawn
      ParticleOption<float> angularVelocity;  
      ParticleOption<Vec4> color;
      ParticleOption<float> speed;
  
    private:
    
      Particle* SpawnParticle(const Vec3& location);
      void RemoveParticle(unsigned index);
      void ClearParticles(void);

      void UpdateVelocity(unsigned index, float t);
      void UpdateAngularVelocity(unsigned index, float t);
      void UpdatePosition(unsigned index, float dt);
      void UpdateScale(unsigned index, float t);
      void UpdateRotation(unsigned index, float t);
      void UpdateColor(unsigned index, float t);
    
      std::vector<Particle> particles; // bucket of particles
      
      Handle transform;
      Mat3D localRotation;
      
      unsigned int textureID;
      
      float time;                             // current emitter time for spawning
    
      //Vec4D startColor;             // start color for the particles
      //Vec4D endColor;               // end color to interpolate to
      //Eases colorEase;              // how to interpolate the color
    
      Vec3 gravityPull;
    
      bool spawn;                   // if currently spawning
      bool timedSpawn;              // set on timed spawn
      bool gravity;                 // if gravity is being used
      bool physics;                 // if trying to simulate physics
  };
}
