/*****************************************************************
Filename: KeyboardInput.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "KeyboardInput.h"
#include <Windows.h>



namespace Framework
{
  KeyboardInput::KeyboardInput()
  {
  }

  KeyboardInput::~KeyboardInput()
  {
  }

  void KeyboardInput::Initialize()
  {
    for(int i = 0; i < 256; ++i)
    {
      _previousState[i] = false;
      _currentState[i] = false;
    }
  }

  void KeyboardInput::UpdateKey(unsigned int key, bool state)
  {
    // we are updating... so set the previous to the current
    _previousState[key] = _currentState[key];

    // then set the current to the new state
    _currentState[key] = state;
  }

  void KeyboardInput::GetMsg(void* pmsg)
  {
    MSG* msg = (MSG*)pmsg;

    switch(msg->message)
    {
    case WM_KEYUP:
      TranslateMessage(msg);
      UpdateKey(msg->wParam, false);
      break;
    case WM_KEYDOWN:
      TranslateMessage(msg);
      UpdateKey(msg->wParam, true);
      break;

    default:
      break;
    }
  }

  void KeyboardInput::Update()
  {
    // Sets previous to current so we can do key released, and key pressed
    for(int i = 0; i < 256; ++i)
    {
      _previousState[i] = _currentState[i];
    }
  }

  /*---------- Key Pressed ----------*/

  bool KeyboardInput::KeyIsPressed(unsigned int key) const
  {
#if USE_EDITOR
    return false;
#endif

    if(_previousState[key] == 0 && _currentState[key] == 1)
      return true;
    else
      return false;
  }

  /*---------- Key Down ----------*/

  bool KeyboardInput::KeyIsDown(unsigned int key) const
  {
#if USE_EDITOR
    return dit::EDITOR_INPUT->GetKeyboard()->IsKeyDown((dit::Keys)key);
#endif

    if(_previousState[key] == 1 && _currentState[key] == 1)
      return true;
    else
      return false;
  }

  /*---------- Key Released ----------*/

  bool KeyboardInput::KeyIsReleased(unsigned int key) const
  {
#if USE_EDITOR
    return false;
#endif

    if(_previousState[key] == 1 && _currentState[key] == 0)
      return true;
    else
      return false;
  }

  /*---------- Keyboard Active/Inactive ----------*/

  void KeyboardInput::SetActive(bool flag)
  {
    _active = flag;
    return;
  }

  bool KeyboardInput::GetActiveState(void) const
  {
    return _active;
  }
}