/*****************************************************************
Filename: MouseInput.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "engine/window/Window32.h"
#include "MouseInput.h"

#define LMB 0
#define RMB 1
#define MMB 2

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
      _previousState[i] = _currentState[i] = false;
    }
  }

  // Updates the mouse states from winproc
  void MouseInput::GetMsg(void* pmsg)
  {
    MSG* msg = (MSG*)pmsg;

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
    for(int i = 0; i < 3; ++i)
    {
      _previousState[i] = _currentState[i];
    }
  }

  /*---------- Button Press ----------*/

  bool MouseInput::ButtonPressed(unsigned int button) const
  {
    if(_previousState[button] == 0 && _currentState[button] == 1)
    {
      return true;
    }
    else
      return false;
  }

  /*---------- Button Down ----------*/

  bool MouseInput::ButtonDown(unsigned int button) const
  {
    if(_previousState[button] == 1 && _currentState[button] == 1)
      return true;
    else
      return false;
  }

  /*---------- Button Released ----------*/

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

  void MouseInput::_UpdateMove(void* msg)
  {
    POINT window;
    GetCursorPos(&window);

    if(ScreenToClient(ENGINE->Window->GetHandle(), &window))
    {
      _screenPosition.X = (float)window.x;
      _screenPosition.Y = (float)window.y;
    }

    _ScreenToWorld();

    return;
  }

  // Not implemented yet. Probably not needed.
  void MouseInput::_ScreenToWorld()
  {
    return; // not doing this yet.
  }

}
