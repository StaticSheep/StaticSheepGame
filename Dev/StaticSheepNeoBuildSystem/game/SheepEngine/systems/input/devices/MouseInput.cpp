

#include "pch/precompiled.h"
#include "engine/window/Window32.h"
#include "MouseInput.h"

#define LMB 0
#define RMB 1
#define MMB 2

namespace Framework
{

/*****************************************************************************/
/*!
  \brief
    Default constructor for mouseinput
*/
/*****************************************************************************/
  MouseInput::MouseInput()
  {

  }

/*****************************************************************************/
/*!
  \brief
    Default destructor for mouseinput
*/
/*****************************************************************************/
  MouseInput::~MouseInput()
  {

  }

/*****************************************************************************/
/*!
  \brief
    Initialize function. Set all of the buttons to 0. (not pressed)
*/
/*****************************************************************************/
  void MouseInput::Initialize()
  {
    // only doing left, right, and middle buttons
    for(int i = 0; i < 3; ++i)
    {
      _previousState[i] = _currentState[i] = false;
    }
  }

/*****************************************************************************/
/*!
  \brief
    Gets passed a msg from peekmessage, and if it is a mouse message, it 
    filters out which one and updates the mouse accordingly.

  \param msg
    Pointer the MSG struct that contains the data for the message.
*/
/*****************************************************************************/
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

/*****************************************************************************/
/*!
  \brief
    Updates the mouse. Sets the previous state to the current state.
*/
/*****************************************************************************/
  void MouseInput::Update()
  {
    for(int i = 0; i < 3; ++i)
    {
      _previousState[i] = _currentState[i];
    }
  }

/*****************************************************************************/
/*!
  \brief
    Checks to see if a specific button has been pressed.
*/
/*****************************************************************************/
  bool MouseInput::ButtonPressed(unsigned int button) const
  {
    if(_previousState[button] == 0 && _currentState[button] == 1)
    {
      return true;
    }
    else
      return false;
  }

/*****************************************************************************/
/*!
  \brief
    Checks to see if a specific button is down.
*/
/*****************************************************************************/
  bool MouseInput::ButtonDown(unsigned int button) const
  {
    if(_previousState[button] == 1 && _currentState[button] == 1)
      return true;
    else
      return false;
  }

/*****************************************************************************/
/*!
  \brief
    Checks to see if a specific button has been released this frame.
*/
/*****************************************************************************/
  bool MouseInput::ButtonReleased(unsigned int button) const
  {
    if(_previousState[button] == 1 && _currentState[button] == 0)
      return true;
    else
      return false;
  }

/*****************************************************************************/
/*!
  \brief
    Gets the current world position of the mouse.
*/
/*****************************************************************************/
  Vec2 MouseInput::GetWorldPosition(void)
  {
    return _worldPosition;
  }

/*****************************************************************************/
/*!
  \brief
    Gets the current screen position of the mouse.
*/
/*****************************************************************************/
  Vec2 MouseInput::GetScreenPosition(void)
  {
    return _screenPosition;
  }

/*****************************************************************************/
/*!
  \brief
    Updates a specific button depending on whether it was pressed or released.
*/
/*****************************************************************************/
  void MouseInput::_UpdateButton(unsigned int button, bool state)
  {
    _previousState[button] = _currentState[button];
    _currentState[button] = state;
  }

/*****************************************************************************/
/*!
  \brief
    Anytime the mouse is moved in the window, it sets the screen position
    of the mouse, then updates the world position of the mouse.

  \param msg
    Pointer to the message that contains positional data.
*/
/*****************************************************************************/
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

/*****************************************************************************/
/*!
  \brief
    Converts the screen coordinates of the mouse to world coordinates.
*/
/*****************************************************************************/
  void MouseInput::_ScreenToWorld()
  {
    return; // not doing this yet.
  }

}