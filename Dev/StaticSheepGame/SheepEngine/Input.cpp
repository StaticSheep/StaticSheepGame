/*****************************************************************
Filename: Input.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Input.h"
#include <iostream>

namespace Framework
{

  MouseInput::MouseInput()
  {

  }

  MouseInput::~MouseInput()
  {

  }

  void MouseInput::Initialize()
  {
    // only doing left, right, and middle buttons
    for(int i = 0; i < 3; ++i)
    {
      _previousState[i] = _currentState[i] = 0;
    }
  }

  void MouseInput::Update()
  {
    MSG msg;

    for(int i = 0; i < 3; ++i)
    {
      _previousState[i] = _currentState[i];
      _currentState[i] = 0;
    }

    // grab all of the mouse events.
    while(PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))
    {
      switch(msg.message)
      {
        case WM_MOUSEMOVE:
          _UpdateMove(&msg);
          break;

        // intentional fall through
        case WM_LBUTTONDOWN:
          _UpdateButton(LMB);
          break;

        case WM_RBUTTONDOWN:
          _UpdateButton(RMB);
          break;

        case WM_MBUTTONDOWN:
          _UpdateButton(MMB);
          break;

        default:
          break;
      }
    }

  }

  bool MouseInput::ButtonPressed(unsigned int button) const
  {
    if(_previousState[button] == 0 && _currentState[button] == 1)
    {
      return true;
    }
    else
      return false;
  }

  bool MouseInput::ButtonDown(unsigned int button) const
  {
    if(_previousState[button] == 1 && _currentState[button] == 1)
      return true;
    else
      return false;
  }

  bool MouseInput::ButtonReleased(unsigned int button) const
  {
    if(_previousState[button] == 1 && _currentState[button] == 0)
      return true;
    else
      return false;
  }

  void MouseInput::GetWorldPosition(void)
  {
    return;
  }

  void MouseInput::GetScreenPosition(void)
  {
    return;
  }

  void MouseInput::_UpdateButton(unsigned int button)
  {
    _currentState[button] = 1;
  }

  void MouseInput::_UpdateMove(MSG* msg)
  {
    return;
  }

  void MouseInput::_ScreenToWorld()
  {
    // do stuff
  }

  /*****************************************************************************/
  /*****************************************************************************/
  /*****************************************************************************/

  KeyboardInput::KeyboardInput()
  {

  }

  KeyboardInput::~KeyboardInput()
  {

  }

  void KeyboardInput::Initialize()
  {
  }

  void KeyboardInput::Update()
  {
    MSG msg;

    for(int i = 0; i < 256; ++i)
    {
      _previousState[i] = _currentState[i];
      _currentState[i] = 0;
    }

    // grab all of the keyboard events
    while(PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
    {
      switch(msg.message)
      {
      case WM_KEYDOWN:
        TranslateMessage(&msg);
        _currentState[msg.wParam] = 1;
        DispatchMessage(&msg);
        break;

      default:
        break;
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

  }

  bool KeyboardInput::KeyIsPressed(unsigned int key) const
  {
    if(_previousState[key] == 0 && _currentState[key] == 1)
      return true;
    else
      return false;
  }

  bool KeyboardInput::KeyIsDown(unsigned int key) const
  {
    if(_previousState[key] == 1 && _currentState[key] == 1)
      return true;
    else
      return false;
  }

  bool KeyboardInput::KeyIsReleased(unsigned int key) const
  {
    if(_previousState[key] == 1 && _currentState[key] == 0)
      return true;
    else
      return false;
  }

  void KeyboardInput::SetActive(bool flag)
  {
    _active = flag;
    return;
  }

  bool KeyboardInput::GetActiveState(void) const
  {
    return _active;
  }

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/



  InputManager::InputManager()
  {

  }
  InputManager::~InputManager()
  {

  }

  void InputManager::Initialize()
  {
    Mouse.Initialize();
    Keyboard.Initialize();

    return;
  }
  void InputManager::Update(float dt)
  {

    Mouse.Update();
    Keyboard.Update();

    float poop = dt;

    return;
  }
}