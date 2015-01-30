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
  struct ParticleOptionShort
  {
    T m_startMin;
    T m_startMax;
  };

  template<typename T>
  struct ParticleOption
  {
    T m_startMin;
    T m_startMax;
    T m_endMin;
    T m_endMax;
  };

  // byte budget 100 bytes
  struct Particle
  {
    Particle();
    Particle(ParticleOption<float>& scale,
             ParticleOption<Vec4>&  color,
             ParticleOption<Vec3>&  dir,
             ParticleOption<float>& speed,
             ParticleOptionShort<float>& life);


    Vec3 position;          // 12 bytes
    float scale;            //  4 bytes
    float endScale;         //  4 bytes
    Vec4 color;             // 16 bytes
    Vec4 endColor;          // 16 bytes
    Vec3 direction;         // 12 bytes
    Vec3 endDirection;      // 12 bytes
    float speed;            //  4 bytes
    float endSpeed;         //  4 bytes
    float theta;            //  4 bytes
    float life;             //  4 bytes
    float endLife;          //  4 bytes
                      // total 96 bytes
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
      void Remove();
    
      /*----- Toggles ----- */
    
      void Toggle(bool state);
      void ToggleTimed(float time); // toggle on for a specified amount of time
    
      void DestroyParticles(void);
    
      /*----- Setters ----- */
    
      void SetTexture(unsigned int newTexture);
      void SetLifetime(ParticleOptionShort<float>& op);
      void SetSpawnRate(ParticleOptionShort<float>& op);
      void SetDirection(void* op);
      void SetSpeed(ParticleOption<float>& op);
      void SetScale(ParticleOption<float>& op);
      void SetVortex(float something, float something2); // some sort of spinning
      void SetAngularVelocity(ParticleOption<float>);
      void SetColor(ParticleOption<Vec4>& op);
    
      void SetGravity(bool state);
      void SetGravityPull(Vec3& strength);

      void TweakSetDirection(const void*);


      std::string textureName;
      ParticleOptionShort<float> particleLife;     // how long particles live
      ParticleOptionShort<float> rate;             // spawn rate
      ParticleOptionShort<float> amount;           // how many particles per tick
      ParticleOption<Vec3> direction;         // velocity of the particles
      ParticleOption<float> scale;            // start to end scale of particles
      ParticleOption<float> angularVelocity;  
      ParticleOption<Vec4> color;
      ParticleOption<float> speed;

      int scaleEase;
      int directionEase;
      int colorEase;
      int speedEase;
  
    private:
    
      Particle* SpawnParticle(const Vec3& location);
      void RemoveParticle(unsigned index);
      void ClearParticles(void);

      void UpdateDirection(unsigned index, float t);
      void UpdateSpeed(unsigned index, float t);
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
      bool directionChange;
  };
}
