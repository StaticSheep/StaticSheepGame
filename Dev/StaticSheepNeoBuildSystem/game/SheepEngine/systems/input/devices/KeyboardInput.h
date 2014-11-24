/*****************************************************************
Filename: KeyboardInput.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class KeyboardInput
  {
  public:

    KeyboardInput();
    ~KeyboardInput();

    void Initialize();
    void Update();

    /* ------- Getters ------ */

    bool KeyIsPressed(unsigned int key) const;
    bool KeyIsDown(unsigned int key) const;
    bool KeyIsReleased(unsigned int key) const;

    void UpdateKey(unsigned int key, bool state);

    void SetActive(bool);
    bool GetActiveState(void) const;

  private:

    void GetMsg(void*);

    /* ------- States ------ */
    bool _active;
    bool _previousState[256];
    bool _currentState[256];

    // let the input manager touch our privates
    friend class InputManager;
  };
}