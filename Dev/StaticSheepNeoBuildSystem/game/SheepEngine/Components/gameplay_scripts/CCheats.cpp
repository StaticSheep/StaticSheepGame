/*****************************************************************
Filename: Cheats.cpp
Project:  Gam200
Author(s): Greg Walls (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"
#include "CCheats.h"
#include "types/space/Space.h"
#include "../transform/CTransform.h"
#include "../colliders/CBoxCollider.h"
#include "../sprites/CAniSprite.h"
#include "../controllers/player/CPlayerController.h"
#include "../sound/CSoundEmitter.h"

namespace Framework
{
  Cheats::Cheats()
  {
    //here I'm building the arrays of button press sequences, make sure it's only six buttons
    Buttons gm[] {Buttons::UP, Buttons::UP, Buttons::DOWN, Buttons::DOWN, Buttons::LEFT, Buttons::RIGHT};
    GM = CheatSequence(gm);
    GM.activated = false;

    Buttons gg[] {Buttons::UP, Buttons::DOWN, Buttons::UP, Buttons::DOWN, Buttons::UP, Buttons::DOWN};
    GG = CheatSequence(gg);
    GG.activated = false;

    Buttons pm[] {Buttons::UP, Buttons::UP, Buttons::UP, Buttons::UP, Buttons::UP, Buttons::UP};
    PM = CheatSequence(pm);
    PM.activated = false;

    Buttons iw[] {Buttons::UP, Buttons::RIGHT, Buttons::DOWN, Buttons::LEFT, Buttons::UP, Buttons::DOWN};
    IW = CheatSequence(iw);
    IW.activated = false;
  }

  Cheats::~Cheats()
  {

  }

  void Cheats::Initialize()
  {
    //logic setup, you're attached and components are in place
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(Cheats::LogicUpdate));
    space->GetGameObject(owner)->hooks.Add("ButtonPressed", self, BUILD_FUNCTION(Cheats::RecordButtonPress));
    
    playerController = space->GetGameObject(owner)->GetComponentHandle(ePlayerController);

  }

  void Cheats::Remove()
  {
    space->hooks.Remove("LogicUpdate", self);
  }

  void Cheats::LogicUpdate(float dt)
  {

  }

  void Cheats::RecordButtonPress(Buttons button)
  {
    PlayerController* pc = space->GetHandles().GetAs<PlayerController>(playerController); //get the player controller

    //if you make a new cheat sequence add it here with the rest to check its update
    GM.CheatUpdate(button);
    GG.CheatUpdate(button);
    PM.CheatUpdate(button);
    IW.CheatUpdate(button);

    //here I'm checking if the individual cheat sequences have been activated or not,
    //and activated the corresponding cheat bool in the player controller
    if (GM.activated)
      pc->Combat()->SetGodMode(true);
    else
      pc->Combat()->SetGodMode(false);
    if (GG.activated)
      pc->GoldenGun = true;
    else
      pc->GoldenGun = false;
    //if (PM.activated)
    //  pc->PerfectMachine = true;
    //else
    //  pc->PerfectMachine = false;
    if (IW.activated)
      space->hooks.Call("CheatWin");
  }

  Cheats::CheatSequence::CheatSequence(Buttons *input)
  {
    for (int i = 0; i < 6; ++i)
      buttonSequence[i] = input[i];
    for (int i = 0; i < 6; ++i)
      activateSequence[i] = false;
  }

  void Cheats::CheatSequence::CheatUpdate(Buttons button)
  {
    int i;
    for (i = 0; i < 6; ++i)
    {
      //check where in the sequence we are
      if (activateSequence[i])
      {
        continue;
      }
      else
        break;
    }
    if (button == buttonSequence[i])
    {
      activateSequence[i] = true;
      if (i == 5)
      {
        activated = !activated;
        AUDIO->Play("weapon_pickup", new SoundInstance(1.0f));
        ClearSequence();
        return;
      }
    }

    else
      ClearSequence();

  }

  void Cheats::CheatSequence::ClearSequence()
  {
    for (int i = 0; i < 6; ++i)
      activateSequence[i] = false;
  }
}
