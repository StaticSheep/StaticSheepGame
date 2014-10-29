
#include "pch/precompiled.h"
#include "systems/skynet/Skynet.h"
#include "../SheepUtil/include/SheepMath.h"
#include "boost/random.hpp"

namespace Framework
{
  
  Skynet::Skynet(SkynetMode state)
  {
    // set the mode
    mode = state;
    // grab the input to touch it all over the place
    input = SHEEPINPUT;
  }
  
  Skynet::~Skynet()
  {
    // zero out the pointer?
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
    
    // to turn skynet on and off, press F8
    if (input->Keyboard.KeyIsPressed(VK_F8))
    {
      // if deactivated, activate... otherwise deactivate
      mode = (mode == Sky_Deactivated ? Sky_Active : Sky_Deactivated);
    }
    return;
  }
  
  const void* Skynet::GetDebugData()
  {
    // not a whole lot of debug data to give...
    debug.mode = mode;
    return (void*)&debug;
  }
  
  XINPUT_STATE AI::Update()
  {
    // countdown the timers, they represent frames
    --actionDelay;
    --movementDelay;

    // if enough frames have elapsed to change button combinations
    if (actionDelay < 0)
    {
      int random = GetRandom(0, 100);

      // reset the buttons
      state.Gamepad.wButtons = 0;

      // 40% chance to press buttons
      if (random > 60)
      {
        // press at least one button...
        int loop = 1;
        int previous = -1;

        // 20% chance to press two action buttons
        if (random > 80)
          loop = 2; // so set the loop to 2

        // kind of dumb but whatever
        for (int i = 0; i < loop; ++i)
        {
          // make sure we don't pick the same button twice
          while ((random = rand() % 4) == previous)
          {
            // first pass through will just grab the first random
            // second will make sure it's not the same as the last
          }

          previous = random;

          // add the button value to the gamepad
          switch (random)
          {
          case 0: // button A
            state.Gamepad.wButtons += XINPUT_GAMEPAD_A;
            break;

          case 1: // button B
            state.Gamepad.wButtons += XINPUT_GAMEPAD_B;
            break;

          case 2: // button X
            state.Gamepad.wButtons += XINPUT_GAMEPAD_X;
            break;

          case 3: // button Y
            state.Gamepad.wButtons += XINPUT_GAMEPAD_Y;
            break;

            // this should never happen... but makes the compiler happy
          default:
            state.Gamepad.wButtons = 0;
            break;
          }
        }
      }

      // get another random number
      random = GetRandom(0, 100);

      // shoulder buttons, 40% chance to press
      if (random > 60)
      {
        // 20% chance to press one
        if (random < 80)
        {
          // pick out shoulder
          random = random % 2;

          switch (random)
          {
          case 0:
            state.Gamepad.wButtons += XINPUT_GAMEPAD_LEFT_SHOULDER;
            break;

          case 1:
            state.Gamepad.wButtons += XINPUT_GAMEPAD_RIGHT_SHOULDER;
            break;

          // should never happen, but the compiler is happy now
          default:
            state.Gamepad.wButtons = 0;
            break;
          }
        }
        else // press both shoulder buttons, 20% chance
        {
          state.Gamepad.wButtons += XINPUT_GAMEPAD_RIGHT_SHOULDER;
          state.Gamepad.wButtons += XINPUT_GAMEPAD_LEFT_SHOULDER;
        }
      }

      random = GetRandom(0, 100);

      // 50% chance to press random pressure on triggers
      if (random > 50)
      {
        state.Gamepad.bLeftTrigger = GetRandom(0, 255);
        state.Gamepad.bRightTrigger = GetRandom(0, 255);
      }
      else //otherwise zero it out
      {
        state.Gamepad.bLeftTrigger = 0;
        state.Gamepad.bRightTrigger = 0;
      }

      // delay between 1 frame and 60 frames
      actionDelay = GetRandom(1, 60);
    }

    if (movementDelay < 0)
    {
      // left joystick
      state.Gamepad.sThumbLX = (short)GetRandom(-32767, 32767);
      state.Gamepad.sThumbLY = (short)GetRandom(-32767, 32767);

      // right joystick
      state.Gamepad.sThumbRX = (short)GetRandom(-32767, 32767);
      state.Gamepad.sThumbRY = (short)GetRandom(-32767, 32767);

      // delay between 0.5 seconds to 1.5 seconds
      movementDelay = GetRandom(30, 90);

      // otherwise too much crazy randomness and objects will just twerk forever
    }

    // return the gamepad state to change the input to what we randomized
    return state;
  }
}