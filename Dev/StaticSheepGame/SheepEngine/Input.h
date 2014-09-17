#pragma once


#include "System.h"

#define MOUSE_OFFSET 0x0201
#define LMB 0
#define RMB 1
#define MMB 2

namespace Framework
{

  class MouseInput
  {
  public:

    MouseInput();
    ~MouseInput();

    void Initialize(void);
    void Update(void);
    void GetWorldPosition(void); // change this for working with everything else
    void GetScreenPosition(void); // change this for working with everything else
  
    bool ButtonPressed(unsigned int button) const;
    bool ButtonDown(unsigned int button) const;
    bool ButtonReleased(unsigned int button) const;

  private:

    bool _previousState[3];
    bool _currentState[3];

    void _UpdateButton(unsigned int);
    void _UpdateMove(void* msg);

    void _ScreenToWorld();

  };

  class KeyboardInput
  {
  public:

    KeyboardInput();
    ~KeyboardInput();

    void Initialize();
    void Update();

    bool KeyIsPressed(unsigned int key) const;
    bool KeyIsDown(unsigned int key) const;
    bool KeyIsReleased(unsigned int key) const;

    void SetActive(bool);
    bool GetActiveState(void) const;

  private:

    bool _active;
    bool _previousState[256];
    bool _currentState[256];
  };


  class InputManager  : public ISystem
  {
  public:

    InputManager();
    ~InputManager();

    virtual std::string GetName() {return "SheepInput";};

    void Initialize();
    void Update(float dt);

    MouseInput Mouse;
    KeyboardInput Keyboard;

  };
}