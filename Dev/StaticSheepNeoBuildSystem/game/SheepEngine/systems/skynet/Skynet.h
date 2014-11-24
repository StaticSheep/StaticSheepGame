/*****************************************************************
Filename: Skynet.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "systems/System.h"
#include "systems/input/Input.h"


namespace Framework
{
  enum SkynetMode
  {
    Sky_Deactivated = 0,
    Sky_Active,
    Sky_Replay
  };
  
  
  struct DebugSkynet
  {
    SkynetMode mode;
    // file shit
  };
  
  // Really random and stupid AI
  class AI
  {
  public:
    
    AI() {actionDelay = 60;movementDelay = 60;state.Gamepad.wButtons = 0;};
    
    XINPUT_STATE Update();
    
  private:
    
    int actionDelay;
    int movementDelay;
    XINPUT_STATE state;
    
  };
  
  class Skynet : public ISystem
  {
  public:

    Skynet(SkynetMode mode = Sky_Deactivated);
    ~Skynet();

    virtual std::string GetName() {return "Skynet";};
    
    void Initialize();
    void Update(float dt);
    
    // Disables skynet from modifying anything
    void Disable(){mode = Sky_Deactivated;};
    
    // Sets skynet to play any unused gamepads
    void AutoPlay(){mode = Sky_Active;};
    
    // Sets skynet to replay. Not implemented
    void Replay(){mode = Sky_Replay;};
    
    const void* GetDebugData();
    
  private:
    
    // FILE* inputFile;
    DebugSkynet debug;
    SkynetMode mode;
    InputManager* input;
    AI players[4];
  };
  
  
}