/*****************************************************************
Filename: Input.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Input.h"
#include <iostream>
#include <windows.h>

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

  void MouseInput::GetMsg(MSG* msg)
  {
    switch(msg->message)
    {
      case WM_MOUSEMOVE:
        _UpdateMove(msg);
        break;

      case WM_LBUTTONDOWN:
        _UpdateButton(LMB, true);
        break;

      case WM_RBUTTONDOWN:
        _UpdateButton(RMB, true);
        break;

      case WM_MBUTTONDOWN:
        _UpdateButton(MMB, true);
        break;

      case WM_LBUTTONUP:
        _UpdateButton(LMB, false);
        break;

      case WM_RBUTTONUP:
        _UpdateButton(RMB, false);
        break;

      case WM_MBUTTONUP:
        _UpdateButton(MMB, false);
        break;

      default:
        break;
    }
  }

  void MouseInput::Update()
  {
    MSG msg;

    for(int i = 0; i < 3; ++i)
    {
      _previousState[i] = _currentState[i];
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

  Vec2 MouseInput::GetWorldPosition(void)
  {
    return _worldPosition;
  }

  Vec2 MouseInput::GetScreenPosition(void)
  {
    return _screenPosition;
  }

  void MouseInput::_UpdateButton(unsigned int button, bool state)
  {
    _previousState[button] = _currentState[button];
    _currentState[button] = state;
  }

  void MouseInput::_UpdateMove(MSG* msg)
  {
    POINT window;
    GetCursorPos(&window);

    if(ScreenToClient(ENGINE->Window.GetHandle(), &window))
    {
      _screenPosition.X = window.x;
      _screenPosition.Y = window.y;
    }

    _ScreenToWorld();

    return;
  }

  void MouseInput::_ScreenToWorld()
  {
    
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
    for(int i = 0; i < 256; ++i)
    {
      _previousState[i] = 0;
      _currentState[i] = 0;
    }
  }

  void KeyboardInput::UpdateKey(unsigned int key, bool state)
  {
    _previousState[key] = _currentState[key];
    _currentState[key] = state;
  }

  void KeyboardInput::GetMsg(MSG* msg)
  {
    switch(msg->message)
    {
    case WM_KEYUP:
      TranslateMessage(msg);
      UpdateKey(msg->wParam, false);
      break;
    case WM_KEYDOWN:
      TranslateMessage(msg);
      //std::cout << (char)msg->wParam << std::endl;
      UpdateKey(msg->wParam, true);
      break;

    default:
      break;
    }
  }

  void KeyboardInput::Update()
  {
    for(int i = 0; i < 256; ++i)
    {
      _previousState[i] = _currentState[i];
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

    frame = true;

    return;
  }
  void InputManager::Update(float dt)
  {
    MSG msg = {0};

    Mouse.Update();
    Keyboard.Update();

    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Query message queue
    {
      Mouse.GetMsg(&msg);
      Keyboard.GetMsg(&msg);
      DispatchMessage(&msg);                   // Dispatch
    }

    return;
  }
}