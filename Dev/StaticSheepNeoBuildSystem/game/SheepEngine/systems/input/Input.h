/*****************************************************************
Filename: Input.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "systems/System.h"
#include <Windows.h>
#include "Xinput.h"
#include "devices/GamepadInput.h"
#include "devices/KeyboardInput.h"
#include "devices/MouseInput.h"

#define LMB 0
#define RMB 1
#define MMB 2

class Msg;

namespace Framework
{
  struct DebugInput
  {
    GamePadInput* pads;
  };

  // Manager for all input devices. Mouse, keyboard, and gamepads
  class InputManager  : public ISystem
  {
  public:

    InputManager();
    ~InputManager();

    virtual std::string GetName() {return "SheepInput";};

    void Initialize();
    void Update(float dt);
    void GetGamePadState(int index, XINPUT_STATE* state);
    const void* GetDebugData();

    /*----- LUA BINDS -----*/

    static GamePadInput* GetGamePad(int index);
    static bool KeyIsPressed(unsigned key);
    static bool KeyIsDown(unsigned key);
    static bool KeyIsReleased(unsigned key);

    /*----- Input Devices -----*/

    MouseInput Mouse;
    KeyboardInput Keyboard;
    GamePadInput Pads[4];
  
    /*----- Debug -----*/

    bool Autoplay;
    DebugInput debug;

  };

  extern InputManager* SHEEPINPUT;
}
