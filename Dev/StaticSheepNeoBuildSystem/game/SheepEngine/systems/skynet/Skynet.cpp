
#include "pch/precompiled.h"
#include "systems/skynet/Skynet.h"
//#include <cstdlib>

namespace Framework
{
  
  Skynet::Skynet(SkynetMode state)
  {
    mode = state;
    input = SHEEPINPUT;
  }
  
  Skynet::~Skynet()
  {
    input = 0;
  }

  void Skynet::Initialize()
  {
  }
  
  void Skynet::Update(float dt)
  {
    // if skynet is active...
    if(mode == Sky_Active)
    {
      // let it touch all of the inputs
      for(int i = 0; i < 4; ++i)
        input->Pads[i].State = players[i].Update();
    }

    if (input->Keyboard.KeyIsPressed(VK_F8))
    {
      if (mode == Sky_Active)
        mode = Sky_Deactivated;
      else
      if (mode == Sky_Deactivated)
        mode = Sky_Active;
    }
    
    return;
  }
  
  XINPUT_STATE AI::Update()
  {
    --actionDelay;
    --movementDelay;
    
    if(actionDelay < 0)
    {
      int random = rand() % 100;
      
      // reset the buttons
      state.Gamepad.wButtons = 0;
      
      if( random > 60)
      {
        random = random % 4;
        
        switch(random)
        {
          case 0:
            state.Gamepad.wButtons = XINPUT_GAMEPAD_A;
            break;
            
          case 1:
            state.Gamepad.wButtons = XINPUT_GAMEPAD_B;
            break;
            
          case 2:
            state.Gamepad.wButtons = XINPUT_GAMEPAD_X;
            break;
            
          case 3:
            state.Gamepad.wButtons = XINPUT_GAMEPAD_Y;
            break;
            
          default:
            state.Gamepad.wButtons = 0;
            break;
        }
      }
      
      random = rand() % 100;
      
      // shoulder buttons
      if(random > 60)
      {
        random = random % 2;
        
        switch(random)
        {
          case 0:
            state.Gamepad.wButtons += XINPUT_GAMEPAD_LEFT_SHOULDER;
            break;
            
          case 1:
            state.Gamepad.wButtons += XINPUT_GAMEPAD_RIGHT_SHOULDER;
            break;
            
          default:
            state.Gamepad.wButtons = 0;
            break;
        }
      }
      
      random = rand() % 100;
      
      if(random > 60)
      {
        state.Gamepad.bLeftTrigger = (rand() % 4) * 80; // deep magic
        state.Gamepad.bRightTrigger = (rand() % 4) * 80;
      }
      
      actionDelay = (rand() % 60) + 1;
    }
    
    if(movementDelay < 0)
    {
      // left joystick
      state.Gamepad.sThumbLX = (rand() % 32767) * ((0 < rand() % 2) ? -1 : 1); // signed shorts
      state.Gamepad.sThumbLY = (rand() % 32767) * ((0 < rand() % 2) ? -1 : 1);
      
      // right joystick
      state.Gamepad.sThumbRX = (rand() % 32767) * ((0 < rand() % 2) ? -1 : 1);
      state.Gamepad.sThumbRY = (rand() % 32767) * ((0 < rand() % 2) ? -1 : 1);
      
      movementDelay = 30 + (rand() % 60);
    }
    
    return state;
  }
  
  const void* Skynet::GetDebugData()
  {
    debug.mode = mode;
    return (void*)&debug;
  }
  
}