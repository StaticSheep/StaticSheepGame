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
  class Skynet;
  extern Skynet* SKYNET;

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
    void Update(float dt); // if active then do things
    
    void Disable(){mode = Sky_Deactivated;}; // disable skynet
    
    void AutoPlay(){mode = Sky_Active;}; // set it to play
    
    void Replay(){mode = Sky_Replay;}; // fill in later for reading in a file
    
    const void* GetDebugData();
    
    SkynetMode mode;
  private:
    
    // FILE* inputFile;
    DebugSkynet debug;
    InputManager* input;
    AI players[4];
  };
  
  
}