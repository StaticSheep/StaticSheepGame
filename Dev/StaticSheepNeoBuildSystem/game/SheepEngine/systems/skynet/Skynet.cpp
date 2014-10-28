
#include "pch/precompiled.h"
#include "systems/skynet/Skynet.h"
#include "../SheepUtil/include/SheepMath.h"
#include "boost/random.hpp"

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
    
    // turn skynet on and off
    if (input->Keyboard.KeyIsPressed(VK_F8))
    {
      mode = (mode == Sky_Deactivated ? Sky_Active : Sky_Deactivated);
    }
    
    return;
  }
  
  XINPUT_STATE AI::Update()
  {
    // countdown the timers
    --actionDelay;
    --movementDelay;
    
    // if enough time has elapsed to change button combinations
    if(actionDelay < 0)
    {
      int random = GetRandom(0,100);

      // reset the buttons
      state.Gamepad.wButtons = 0;
      
      // 40% chance to press buttons
      if(random > 60)
      {
        int loop = 1;
        int previous = -1;

        // 20% chance to press two action buttons
        if (random > 80)
          loop = 2;

        for (int i = 0; i < loop; ++i)
        {
          // make sure we don't pick the same button twice
          while ((random = rand() % 4) == previous)
          {}

          previous = random;

          // add the button value to the gamepad
          switch (random)
          {
          case 0:
            state.Gamepad.wButtons += XINPUT_GAMEPAD_A;
            break;

          case 1:
            state.Gamepad.wButtons += XINPUT_GAMEPAD_B;
            break;

          case 2:
            state.Gamepad.wButtons += XINPUT_GAMEPAD_X;
            break;

          case 3:
            state.Gamepad.wButtons += XINPUT_GAMEPAD_Y;
            break;

          // this should never happen...
          default:
            state.Gamepad.wButtons = 0;
            break;
          }
        }
      }
      
      random = GetRandom(0,100);
      
      // shoulder buttons, 40% chance to press
      if(random > 60)
      {
        if (random < 80)
        {
          random = random % 2;

          switch (random)
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
        else // press both shoulder buttons
        {
          state.Gamepad.wButtons += XINPUT_GAMEPAD_RIGHT_SHOULDER;
          state.Gamepad.wButtons += XINPUT_GAMEPAD_LEFT_SHOULDER;
        }
      }
      
      random = GetRandom(0, 100);
      
      if(random > 60)
      {
        state.Gamepad.bLeftTrigger = GetRandom(0, 255);
        state.Gamepad.bRightTrigger = GetRandom(0, 255);
      }
      
      actionDelay = GetRandom(1, 60);
    }
    
    if(movementDelay < 0)
    {
      // left joystick
      state.Gamepad.sThumbLX = GetRandom(-32767, 32767);
      state.Gamepad.sThumbLY = GetRandom(-32767, 32767);
      
      // right joystick
      state.Gamepad.sThumbRX = GetRandom(-32767, 32767);
      state.Gamepad.sThumbRY = GetRandom(-32767, 32767);
      
      movementDelay = GetRandom(30, 90);
    }
    
    return state;
  }
  
  const void* Skynet::GetDebugData()
  {
    debug.mode = mode;
    return (void*)&debug;
  }
  
}