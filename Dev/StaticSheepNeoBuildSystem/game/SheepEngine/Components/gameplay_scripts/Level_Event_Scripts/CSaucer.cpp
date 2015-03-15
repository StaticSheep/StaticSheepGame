#include "pch/precompiled.h"
#include "CSaucer.h"
#include "types/space/Space.h"
#include "../../transform/CTransform.h"
#include "../systems/input/Input.h"
#include "SheepMath.h"

namespace Framework
{
  Saucer::Saucer() : crosshairColor(1,1,1,1)
  {

  }

  Saucer::~Saucer()
  {

  }

  void Saucer::LogicUpdate(float dt)
  {
    GamePad* gp = space->GetHandles().GetAs<GamePad>(controller);
    Transform* trans = space->GetHandles().GetAs<Transform>(sTransform);

    Vec3 newTrans = trans->GetTranslation();

    if (!gp->LStick_InDeadZone())
    {
     newTrans += (Vec3(gp->LeftStick_X(), gp->LeftStick_Y(), 0) * 20);
    }

    if (gp->RightTrigger() == 0)
      hasFired = false;

    if (gp->RightTrigger() && !hasFired)
      Fire();

    if (crosshairColor.g < 1)
      crosshairColor.g += dt / 2;

    if (crosshairColor.b < 1)
      crosshairColor.b += dt / 2;

    newTrans.x = Clamp(newTrans.x, -Draw::ScreenWidth() / 2, Draw::ScreenWidth() / 2);
    newTrans.y = Clamp(newTrans.y, -Draw::ScreenHeight() / 2, Draw::ScreenHeight() / 2);
    trans->SetTranslation(newTrans);
  }

  void Saucer::Draw()
  {
    Transform* trans = space->GetHandles().GetAs<Transform>(sTransform);

    GRAPHICS->SetPosition(trans->GetTranslation().X,
      trans->GetTranslation().Y, 0);

    GRAPHICS->SetRotation(trans->GetRotation());

    GRAPHICS->SetSize(.75,
      .75);

    GRAPHICS->SetColor(crosshairColor);

    GRAPHICS->SetObjectOrigin(0, 0);

    GRAPHICS->SetUV(Vec2(0,0), Vec2(1,1));

    GRAPHICS->SetSpriteFlip(false, false);

    GRAPHICS->SetCamState(0);

    GRAPHICS->DrawBatched(crosshairTex);
  }

  void Saucer::Initialize()
  {
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Saucer::LogicUpdate));
    space->hooks.Add("Draw", self, BUILD_FUNCTION(Saucer::Draw));

    sTransform = space->GetGameObject(owner)->GetComponentHandle(eTransform);

    controller = space->GetGameObject(owner)->GetComponentHandle(eGamePad);

    crosshairTex = GRAPHICS->LoadTexture(std::string("Crosshair.png"));
    TexDim = GRAPHICS->GetTextureDim(crosshairTex);
  }


  void Saucer::Fire()
  {
    crosshairColor = Vec4(1, 0, 0, 1);
    hasFired = true;
    Handle asteroid = (FACTORY->LoadObjectFromArchetype(space, "SaucerMissile"))->self;
    Transform *aT = (space->GetGameObject(asteroid)->GetComponent<Transform>(eTransform));
    Transform *trans = space->GetHandles().GetAs<Transform>(sTransform);

    aT->SetTranslation(trans->GetTranslation());
  }

  void Saucer::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
    space->hooks.Remove("Draw", self);
  }
}