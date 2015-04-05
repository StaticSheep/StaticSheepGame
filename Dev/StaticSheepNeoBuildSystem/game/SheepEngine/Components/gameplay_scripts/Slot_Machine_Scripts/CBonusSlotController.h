/*****************************************************************
Filename: CBonusSlotController.h
Project:  Gam250
Author(s): Zachary Nawar (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"

namespace Framework
{


  /* SPACE::BonusResult(bool left, ResultType type,
  int Result)
  */

  class BonusSlotController : public GameComponent
  {
  public:
    BonusSlotController() {};
    ~BonusSlotController() {};

    void Initialize();
    void LogicUpdate(float dt);
    void BounceDown(float dt);
    void Remove();

    void SetSMTextures(int, int*, int*);
    void SetSMResults(int, int*);
    void ReceiveSMResults(std::vector<int>*);

    bool leftHand = false;

  private:
    float bounceDownTimer = 0.0f;
    bool done = false;
    
  };


}