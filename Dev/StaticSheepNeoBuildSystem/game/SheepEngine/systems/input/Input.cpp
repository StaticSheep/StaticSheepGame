/*****************************************************************
Filename: Input.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include <iostream>
#include <windows.h>
#include "Input.h"

//#include "wxeditor/App.h"
#include "input/InputBackend.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

namespace Framework
{
  InputManager* SHEEPINPUT = NULL;

  InputManager::InputManager()
  {
    TRACELOG->Log(TraceLevel::INFO, "Initializing Game Pads");
    // initialize all gamepads
    Pads[0] = GamePadInput(0);
    Pads[1] = GamePadInput(1);
    Pads[2] = GamePadInput(2);
    Pads[3] = GamePadInput(3);

    SHEEPINPUT = this;
  }

  InputManager::~InputManager()
  {

  }
  
  // Initialize devices
  void InputManager::Initialize()
  {
    TRACELOG->Log(TraceLevel::INFO, "Initializing Mouse");
    Mouse.Initialize();
    TRACELOG->Log(TraceLevel::INFO, "Initializing Keyboard");
    Keyboard.Initialize();
    Autoplay = false;

    return;
  }

  void InputManager::GetGamePadState(int index, XINPUT_STATE* state)
  {
    *state = Pads[index].State;
  }

  const void* InputManager::GetDebugData()
  {
    debug.pads = Pads;
    XInputGetState(0, &Pads[0].State);
    return (void*)&debug;
  }

  GamePadInput* InputManager::GetGamePad(int index)
  {
    return &SHEEPINPUT->Pads[index];
  }

  // Updates all of the input devices that are connected
  void InputManager::Update(float dt)
  {
    MSG msg = {0};

    Mouse.Update();
    Keyboard.Update();
    
    // update all of the game pads
    for (int i = 0; i < 4; ++i)
    {
      Pads[i].Update();
    }

#if USE_EDITOR


#else

    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Query message queue
    {
      Mouse.GetMsg(&msg);
      Keyboard.GetMsg(&msg);

      // dispatch the msg for anything that was not handled.
      DispatchMessage(&msg);
    }

#endif

    return;
  }

  bool InputManager::KeyIsPressed(unsigned key)
  {
    return SHEEPINPUT->Keyboard.KeyIsPressed(key);
  }

  bool InputManager::KeyIsDown(unsigned key)
  {
    return SHEEPINPUT->Keyboard.KeyIsDown(key);
  }

  bool InputManager::KeyIsReleased(unsigned key)
  {
    return SHEEPINPUT->Keyboard.KeyIsReleased(key);
  }


}// end namespace
