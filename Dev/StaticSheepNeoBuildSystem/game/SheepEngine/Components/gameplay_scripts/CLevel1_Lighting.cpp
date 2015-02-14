#include "pch/precompiled.h"

#include "../transform/CTransform.h"
#include "../SheepUtil/include/SheepMath.h"
#include "CLevel1_Lighting.h"

#define Countdown 5.0f
namespace Framework
{

  Level1_Lighting::Level1_Lighting()
  {
    m_spawnPos[0] = Vec3(-795.0f, -361.0f, 0.0f);
    m_spawnPos[1] = Vec3(795.0f, -361.0f, 0.0f);
    m_spawnPos[2] = Vec3(795.0f, 361.0f, 0.0f);
    m_spawnPos[3] = Vec3(-795.0f, 361.0f, 0.0f);
    m_spawnPos[4] = Vec3(-200.0f, 0.0f, 0.0f);
    m_spawnPos[5] = Vec3(200.0f, 0.0f, 0.0f);
  }

  Level1_Lighting::~Level1_Lighting()
  {

  }
  void Level1_Lighting::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Level1_Lighting::LogicUpdate));
    //space->hooks.Add("PlayerDied", self, BUILD_FUNCTION(Level1_Lighting::PlayerDied)); // might want this

    if (!ENGINE->m_editorActive)
      CreateSpawnLights();

    m_levelTime = 0;
    m_PulseT = 1;
    m_PulseFlip = false;
  }

  void Level1_Lighting::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

  void Level1_Lighting::LogicUpdate(float dt)
  {

    if (m_levelTime < Countdown)
    {
      space->GetGameObject(m_lights[4])->GetComponent<PointLight>(ePointLight)->m_brightness.a = Ease::Linear(m_levelTime / Countdown, 0.0f, 90.0f);
      space->GetGameObject(m_lights[5])->GetComponent<PointLight>(ePointLight)->m_brightness.a = Ease::Linear(m_levelTime / Countdown, 0.0f, 90.0f);
    }
    else
    {

      space->GetGameObject(m_lights[4])->GetComponent<PointLight>(ePointLight)->m_brightness.a = Ease::Linear(m_PulseT, 60.0f, 90.0f);
      space->GetGameObject(m_lights[5])->GetComponent<PointLight>(ePointLight)->m_brightness.a = Ease::Linear(m_PulseT, 60.0f, 90.0f);

      if (m_PulseT > 1.0f || m_PulseT < 0.0f)
        m_PulseFlip = !m_PulseFlip;

      if (m_PulseFlip)
        m_PulseT += dt / 4.0f;
      else
        m_PulseT -= dt / 4.0f;
    }

    m_levelTime += dt;
  }

  void Level1_Lighting::CreateSpawnLights()
  {
    Transform* lightTrans;
    PointLight* light[6];
    for (int i = 0; i < 6; ++i)
    {
      m_lights[i] = (FACTORY->LoadObjectFromArchetype(space, "SpawnLight"))->self;

      lightTrans = space->GetGameObject(m_lights[i])->GetComponent<Transform>(eTransform);
      light[i] = space->GetGameObject(m_lights[i])->GetComponent<PointLight>(ePointLight);

      lightTrans->SetTranslation(m_spawnPos[i]);
    }

    light[0]->SetBrightness(Vec4(0.0f, 1.0f, 0.0f, 50.0f));
    light[1]->SetBrightness(Vec4(1.0f, 0.0f, 0.0f, 50.0f));
    light[2]->SetBrightness(Vec4(1.0f, 0.0f, 1.0f, 50.0f));
    light[3]->SetBrightness(Vec4(0.0f, 0.0f, 1.0f, 50.0f));
  }
}