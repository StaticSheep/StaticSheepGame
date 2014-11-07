#include "pch/precompiled.h"
#include "CLevel1_Logic.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CBoxCollider.h"
#include "../sound/CSoundPlayer.h"
#include "../../systems/input/Input.h"

namespace Framework
{
  Level1_Logic::Level1_Logic()
	{
    timeLimit = 120;
    spawnTimer = 2;
    playing = false;
	}

  Level1_Logic::~Level1_Logic()
	{
	}

  void Level1_Logic::Initialize()
	{
		//logic setup, you're attached and components are in place
		space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Level1_Logic::LogicUpdate));
    levelSound = space->GetGameObject(owner)->GetComponentHandle(eSoundPlayer);
    
	}

  void Level1_Logic::Remove()
	{
		space->hooks.Remove("LogicUpdate", self);
	}

  void Level1_Logic::LogicUpdate(float dt)
	{
    spawnTimer -= dt;
    if (spawnTimer <= 0)
    {
      GameObject *ePlat = (FACTORY->LoadObjectFromArchetype(space, "SmallPlat"));
      Transform *PT = ePlat->GetComponent<Transform>(eTransform);
      BoxCollider *platC = ePlat->GetComponent <BoxCollider>(eBoxCollider);
      //Transform *playerTrans = space->GetHandles().GetAs<Transform>(playerTransform);
      PT->SetTranslation(Vec3(194.79,-320.0,0.0));

      GameObject *ePlat2 = (FACTORY->LoadObjectFromArchetype(space, "SmallPlat"));
      Transform *PT2 = ePlat2->GetComponent<Transform>(eTransform);
      BoxCollider *platC2 = ePlat2->GetComponent <BoxCollider>(eBoxCollider);
      //Transform *playerTrans = space->GetHandles().GetAs<Transform>(playerTransform);
      PT2->SetTranslation(Vec3(-194.79, -320.0, 0.0));

      spawnTimer = 2;
    }


    if(!playing)
    {
      SoundPlayer *sp = space->GetHandles().GetAs<SoundPlayer>(levelSound);

      // SoundPlayers now take a SoundInstance*, which is kind of like a messaging system.
      // if you want something to loop, you need to create a SoundInstance, and set it's
      // mode. You can also set other properties like volume, pitch, and later 
      // parameters. It is not absolutely necessary to give the player an instance, as it
      // will use a default one that plays once.

      SoundInstance instance;
      instance.mode = PLAY_LOOP;
      sp->Play("space_brawl", &instance);

      playing = true;
    }

    if(SHEEPINPUT->Keyboard.KeyIsPressed(0x41))
    {
      // Pause test. Pressing A pauses the main theme.
      static bool flag = true;
      SoundPlayer *sp = space->GetHandles().GetAs<SoundPlayer>(levelSound);
      sp->Pause("space_brawl", flag);

      flag = !flag;
    }
	}


}
