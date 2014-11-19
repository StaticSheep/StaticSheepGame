#ifndef SHEEP_INPUT_H
#define SHEEP_INPUT_H

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

    // we have one mouse...
    MouseInput Mouse;
    // and one keyboard...
    KeyboardInput Keyboard;

    GamePadInput Pads[4];

    bool Autoplay;
    DebugInput debug;

  };


  extern InputManager* SHEEPINPUT;
}

#endif