#include "Input.h"


void MouseInput::Update()
{
  MSG msg;

  // grab all of the mouse events.
  while(PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE))
  {
    switch(msg.message)
    {
      case WM_MOUSEMOVE:
        _UpdateMove(&msg);
        break;

      case WM_LBUTTONDOWN:

      case WM_LBUTTONUP:

      case WM_LBUTTONDBLCLK:

      case WM_RBUTTONDOWN:

      case WM_RBUTTONUP:

      case WM_RBUTTONDBLCLK:

      case WM_MBUTTONDOWN:

      case WM_MBUTTONUP:

      case WM_MBUTTONDBLCLK:

        _UpdateButton(&msg);
    }
  }

}

void MouseInput::_UpdateButton(MSG* msg)
{
  
}

void MouseInput::_UpdateMove(MSG* msg)
{
  _position.x = msg->pt.x;
  _position.y = msg->pt.y;
}