/******************************************************************************
Filename: Input.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "pch/precompiled.h"

#include <iostream>
#include <windows.h>
#include "Input.h"

#include "wxeditor/App.h"
#include "input/InputBackend.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

namespace Framework
{
  InputManager* SHEEPINPUT = NULL;

/*****************************************************************************/
/*!
  \brief
    Default constructor
*/
/*****************************************************************************/
  InputManager::InputManager()
  {
    Pads[0] = GamePadInput(0);
    Pads[1] = GamePadInput(1);
    Pads[2] = GamePadInput(2);
    Pads[3] = GamePadInput(3);

    SHEEPINPUT = this;
  }

/*****************************************************************************/
/*!
  \brief
    Default destructor
*/
/*****************************************************************************/
  InputManager::~InputManager()
  {

  }

/*****************************************************************************/
/*!
  \brief
    Initializes the mouse and keyboard.
*/
/*****************************************************************************/
  void InputManager::Initialize()
  {
    Mouse.Initialize();
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

/*****************************************************************************/
/*!
  \brief
    Updates the mouse and keyboard. Then checks the messages in the queue and
    sets the current state of the mouse and keyboard accordingly.
*/
/*****************************************************************************/
  void InputManager::Update(float dt)
  {
    MSG msg = {0};

    Mouse.Update();
    Keyboard.Update();
    
    for (int i = 0; i < 4; ++i)
    {
      //XInputGetState(i, &Pads[i].State);
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

}// end namespace
