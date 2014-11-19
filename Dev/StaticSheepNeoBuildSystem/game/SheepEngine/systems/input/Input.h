#ifndef SHEEP_INPUT_H
#define SHEEP_INPUT_H

#include "systems/System.h"
#include <Windows.h>
#include "Xinput.h"

#define LMB 0
#define RMB 1
#define MMB 2

class Msg;

namespace Framework
{
  class GamePadInput
  {
  public:

    GamePadInput(){ padIndex = 0; };
    GamePadInput(int index)
    {
      padIndex = index;
      ZeroMemory(&State, sizeof(XINPUT_STATE));
    };
    ~GamePadInput(){};

    void Initialize(){};
    void Update();

    /* -------------- Helper Functions -------------- */

    /* -- thumb stick functions -- */

    // Checks if the stick is in the dead zone
    // true if right stick, false if left stick
    // Returns true/false if it is dead zone
    bool InDeadzone(bool right);

    // Returns the position of the sticks
    Vec2 LeftStick();
    Vec2 RightStick();

    /* -- button / trigger functions -- */

    float LeftTrigger();  //return value of the left trigger
    float RightTrigger(); //return value of the right trigger

    bool ButtonPressed(int a_iButton); //return true if button is pressed
    bool ButtonDown(int a_iButton);

    /* -- Misc -- */

    void Rumble(float a_fLeftMotor = 0.0f, float a_fRightMotor = 0.0f);

    bool Connected(); // Is it plugged in?

    /* -------------- Member Variables -------------- */

    int padIndex;
    XINPUT_STATE State;

  };

  struct DebugInput
  {
    GamePadInput* pads;
  };

  class MouseInput
  {
  public:

    // default constructor/destructor
    MouseInput();
    ~MouseInput();

    // basic functions
    void Initialize(void);
    void Update(void);

    // getters
    Vec2 GetWorldPosition(void);
    Vec2 GetScreenPosition(void);
  
    // checks the state of the buttons
    bool ButtonPressed(unsigned int button) const;
    bool ButtonDown(unsigned int button) const;
    bool ButtonReleased(unsigned int button) const;

  private:

    // the previous frame's state
    bool _previousState[3];
    // the current frame's state
    bool _currentState[3];

    // positions
    Vec2 _screenPosition;
    Vec2 _worldPosition;
    void GetMsg(void* msg);

    void _UpdateButton(unsigned int, bool state);
    void _UpdateMove(void* msg);

    void _ScreenToWorld();
    
    // let the manager touch our privates
    friend class InputManager;

  };

  class KeyboardInput
  {
  public:

    // constructors/destructors
    KeyboardInput();
    ~KeyboardInput();

    // basic stuff
    void Initialize();
    void Update();

    // check if things are being touched
    bool KeyIsPressed(unsigned int key) const;
    bool KeyIsDown(unsigned int key) const;
    bool KeyIsReleased(unsigned int key) const;

    void UpdateKey(unsigned int key, bool state);

    void SetActive(bool);
    bool GetActiveState(void) const;

  private:

    void GetMsg(void*);

    bool _active;
    bool _previousState[256];
    bool _currentState[256];

    // let the input manager touch our privates
    friend class InputManager;
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