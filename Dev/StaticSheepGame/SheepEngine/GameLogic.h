/*****************************************************************
Filename: GameLogic.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "System.h"

namespace Framework
{
  class GameLogic : public ISystem
  {
  public:
    // Sheep graphics interface constructor
    GameLogic();
    // Sheep graphics interface deconstructor
    ~GameLogic();

    virtual std::string GetName() {return "GameLogic";};

    // Initialization call
    void Initialize(void);

    // Update per frame
    void Update(float dt);
  };

  extern GameLogic* GAMELOGIC;
}