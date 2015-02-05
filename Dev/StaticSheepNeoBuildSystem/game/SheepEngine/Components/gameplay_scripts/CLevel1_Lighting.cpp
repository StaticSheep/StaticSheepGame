#include "pch/precompiled.h"

#include "../transform/CTransform.h"
#include "../SheepUtil/include/SheepMath.h"
#include "CLevel1_Lighting.h"

namespace Framework
{

  Level1_Lighting::Level1_Lighting()
  {
    m_spawnPos[0] = Vec3(-610.0f, -440.0f, 0.0f);
    m_spawnPos[1] = Vec3(610.0f, -440.0f, 0.0f);
    m_spawnPos[2] = Vec3(610.0f, 435.0f, 0.0f);
    m_spawnPos[3] = Vec3(-610.0f, 435.0f, 0.0f);
    m_spawnPos[4] = Vec3(0.0f, 435.0f, 0.0f);
  }

  Level1_Lighting::~Level1_Lighting()
  {

  }
  void Level1_Lighting::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Level1_Lighting::LogicUpdate));
    //space->hooks.Add("PlayerDied", self, BUILD_FUNCTION(Level1_Lighting::PlayerDied)); // might want this

    CreateSpawnLights();
  }

  void Level1_Lighting::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

  void Level1_Lighting::LogicUpdate(float dt)
  {
    
  }

  void Level1_Lighting::CreateSpawnLights()
  {
    Transform* lightTrans;
    for (int i = 0; i < 5; ++i)
    {
      m_lights[i] = (FACTORY->LoadObjectFromArchetype(space, "SpawnLight"))->self;

      lightTrans = space->GetGameObject(m_lights[i])->GetComponent<Transform>(eTransform);
      lightTrans->SetTranslation(m_spawnPos[i]);
    }
  }
}