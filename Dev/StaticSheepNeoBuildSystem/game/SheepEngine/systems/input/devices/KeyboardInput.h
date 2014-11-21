
#pragma once

namespace Framework
{
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
}