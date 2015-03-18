#include "pch/precompiled.h"
#include "LEPinwheel.h"
#include "systems/graphics/SheepGraphics.h"
#include "Components/transform/CTransform.h"
#include "Components/gameplay_scripts/Level_Scripts/CPinwheel.h"
#include "SheepMath.h"

namespace Framework
{
  LEPinwheel::LEPinwheel()
  {

  }

  LEPinwheel::~LEPinwheel()
  {

  }

  void LEPinwheel::Update(float dt)
  {

  }

  void LEPinwheel::FireEvent(GameObject *LogicController)
  {
    LC = LogicController;
    GameObject *pinWheel = (FACTORY->LoadObjectFromArchetype(LogicController->space, "Pinwheel"));
    Transform *trans = pinWheel->GetComponent<Transform>(eTransform);
    Pinwheel* pin = pinWheel->GetComponent<Pinwheel>(ePinwheel);

    if (GetRandom(0, 1))
    {
      trans->SetTranslation(Vec3(-(GRAPHICS->_ScreenWidth / 2.0), 0.0, 1.0));
    }
    else
    {
      pin->moveLeft = true;
      trans->SetTranslation(Vec3((GRAPHICS->_ScreenWidth / 2.0), 0.0, 1.0));
    }
  }
}