/******************************************************************************
Filename: Input.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "pch/precompiled.h"
#include "Input.h"
#include <iostream>
#include <windows.h>

#include "engine/window/Window32.h"


#include "editor/App.h"
#include "input/wxw/WxInputBackend.h"


namespace Framework
{

  InputManager* SHEEPINPUT = NULL;

///*****************************************************************************/
///*********************************MOUSE***************************************/
///*****************************************************************************/
//
///*****************************************************************************/
///*!
//  \brief
//    Default constructor for mouseinput
//*/
///*****************************************************************************/
//  MouseInput::MouseInput()
//  {
//
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Default destructor for mouseinput
//*/
///*****************************************************************************/
//  MouseInput::~MouseInput()
//  {
//
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Initialize function. Set all of the buttons to 0. (not pressed)
//*/
///*****************************************************************************/
//  void MouseInput::Initialize()
//  {
//    // only doing left, right, and middle buttons
//    for(int i = 0; i < 3; ++i)
//    {
//      _previousState[i] = _currentState[i] = false;
//    }
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Gets passed a msg from peekmessage, and if it is a mouse message, it 
//    filters out which one and updates the mouse accordingly.
//
//  \param msg
//    Pointer the MSG struct that contains the data for the message.
//*/
///*****************************************************************************/
//  void MouseInput::GetMsg(void* pmsg)
//  {
//    MSG* msg = (MSG*)pmsg;
//
//    switch(msg->message)
//    {
//      case WM_MOUSEMOVE:
//        _UpdateMove(msg);
//        break;
//
//      case WM_LBUTTONDOWN:
//        _UpdateButton(LMB, true);
//        break;
//
//      case WM_RBUTTONDOWN:
//        _UpdateButton(RMB, true);
//        break;
//
//      case WM_MBUTTONDOWN:
//        _UpdateButton(MMB, true);
//        break;
//
//      case WM_LBUTTONUP:
//        _UpdateButton(LMB, false);
//        break;
//
//      case WM_RBUTTONUP:
//        _UpdateButton(RMB, false);
//        break;
//
//      case WM_MBUTTONUP:
//        _UpdateButton(MMB, false);
//        break;
//
//      default:
//        break;
//    }
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Updates the mouse. Sets the previous state to the current state.
//*/
///*****************************************************************************/
//  void MouseInput::Update()
//  {
//    for(int i = 0; i < 3; ++i)
//    {
//      _previousState[i] = _currentState[i];
//    }
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Checks to see if a specific button has been pressed.
//*/
///*****************************************************************************/
//  bool MouseInput::ButtonPressed(unsigned int button) const
//  {
//    if(_previousState[button] == 0 && _currentState[button] == 1)
//    {
//      return true;
//    }
//    else
//      return false;
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Checks to see if a specific button is down.
//*/
///*****************************************************************************/
//  bool MouseInput::ButtonDown(unsigned int button) const
//  {
//    if(_previousState[button] == 1 && _currentState[button] == 1)
//      return true;
//    else
//      return false;
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Checks to see if a specific button has been released this frame.
//*/
///*****************************************************************************/
//  bool MouseInput::ButtonReleased(unsigned int button) const
//  {
//    if(_previousState[button] == 1 && _currentState[button] == 0)
//      return true;
//    else
//      return false;
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Gets the current world position of the mouse.
//*/
///*****************************************************************************/
//  Vec2 MouseInput::GetWorldPosition(void)
//  {
//    return _worldPosition;
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Gets the current screen position of the mouse.
//*/
///*****************************************************************************/
//  Vec2 MouseInput::GetScreenPosition(void)
//  {
//    return _screenPosition;
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Updates a specific button depending on whether it was pressed or released.
//*/
///*****************************************************************************/
//  void MouseInput::_UpdateButton(unsigned int button, bool state)
//  {
//    _previousState[button] = _currentState[button];
//    _currentState[button] = state;
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Anytime the mouse is moved in the window, it sets the screen position
//    of the mouse, then updates the world position of the mouse.
//
//  \param msg
//    Pointer to the message that contains positional data.
//*/
///*****************************************************************************/
//  void MouseInput::_UpdateMove(void* msg)
//  {
//    POINT window;
//    GetCursorPos(&window);
//
//    if(ScreenToClient(ENGINE->Window->GetHandle(), &window))
//    {
//      _screenPosition.X = (float)window.x;
//      _screenPosition.Y = (float)window.y;
//    }
//
//    _ScreenToWorld();
//
//    return;
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Converts the screen coordinates of the mouse to world coordinates.
//*/
///*****************************************************************************/
//  void MouseInput::_ScreenToWorld()
//  {
//    return; // not doing this yet.
//  }
//
//
///*****************************************************************************/
///*********************************KEYBOARD************************************/
///*****************************************************************************/
//
//
///*****************************************************************************/
///*!
//  \brief
//    Default constructor...
//*/
///*****************************************************************************/
//  KeyboardInput::KeyboardInput()
//  {
//
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Default destructor
//*/
///*****************************************************************************/
//  KeyboardInput::~KeyboardInput()
//  {
//
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Initializes all of the keys to 0. (not pressed)
//*/
///*****************************************************************************/
//  void KeyboardInput::Initialize()
//  {
//    for(int i = 0; i < 256; ++i)
//    {
//      _previousState[i] = false;
//      _currentState[i] = false;
//    }
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Updates the key state to the passed in state.
//
//  \param key
//    The index of the bool to be set.
//
//  \param state
//    The state to change it to.
//*/
///*****************************************************************************/
//  void KeyboardInput::UpdateKey(unsigned int key, bool state)
//  {
//    // we are updating... so set the previous to the current
//    _previousState[key] = _currentState[key];
//
//    // then set the current to the new state
//    _currentState[key] = state;
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Translates a MSG from peekmessage, checks to see if it is a keyboard 
//    message, and updates accordingly.
//
//  \param msg
//    Pointer to the MSG struct that contains event data.
//*/
///*****************************************************************************/
//  void KeyboardInput::GetMsg(void* pmsg)
//  {
//    MSG* msg = (MSG*)pmsg;
//
//    switch(msg->message)
//    {
//    case WM_KEYUP:
//      TranslateMessage(msg);
//      UpdateKey(msg->wParam, false);
//      break;
//    case WM_KEYDOWN:
//      TranslateMessage(msg);
//      //std::cout << (char)msg->wParam << std::endl;
//      UpdateKey(msg->wParam, true);
//      break;
//
//    default:
//      break;
//    }
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Sets all of the previous states to the current state.
//*/
///*****************************************************************************/
//  void KeyboardInput::Update()
//  {
//    for(int i = 0; i < 256; ++i)
//    {
//      _previousState[i] = _currentState[i];
//    }
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Checks to see if a specific key has been pressed this frame.
//*/
///*****************************************************************************/
//  bool KeyboardInput::KeyIsPressed(unsigned int key) const
//  {
//#if USE_EDITOR
//    return false;
//#endif
//
//    if(_previousState[key] == 0 && _currentState[key] == 1)
//      return true;
//    else
//      return false;
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Checks to see if a specific key is being held down.
//*/
///*****************************************************************************/
//  bool KeyboardInput::KeyIsDown(unsigned int key) const
//  {
//#if USE_EDITOR
//    return dit::EDITOR_INPUT->GetKeyboard()->IsKeyDown((dit::Keys)key);
//#endif
//
//    if(_previousState[key] == 1 && _currentState[key] == 1)
//      return true;
//    else
//      return false;
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Checks to see if a specific key has been released this frame.
//*/
///*****************************************************************************/
//  bool KeyboardInput::KeyIsReleased(unsigned int key) const
//  {
//#if USE_EDITOR
//    return false;
//#endif
//
//    if(_previousState[key] == 1 && _currentState[key] == 0)
//      return true;
//    else
//      return false;
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Sets the keyboard to active or not... currently doesn't do anything.
//*/
///*****************************************************************************/
//  void KeyboardInput::SetActive(bool flag)
//  {
//    _active = flag;
//    return;
//  }
//
///*****************************************************************************/
///*!
//  \brief
//    Gets the active state of the keyboard.
//*/
///*****************************************************************************/
//  bool KeyboardInput::GetActiveState(void) const
//  {
//    return _active;
//  }

/*****************************************************************************/
/*********************************MANAGER*************************************/
/*****************************************************************************/


/*****************************************************************************/
/*!
  \brief
    Default constructor
*/
/*****************************************************************************/
  InputManager::InputManager()
  {
    SHEEPINPUT = this;
  }

/*****************************************************************************/
/*!
  \brief
    Default destructor
*/
/*****************************************************************************/
  InputManager::~InputManager()
  {
    delete input;
  }

/*****************************************************************************/
/*!
  \brief
    Initializes the mouse and keyboard.
*/
/*****************************************************************************/
  void InputManager::Initialize()
  {
#if USE_EDITOR
    input = new dit::WxInputBackend();
#else
    input = new WinProcInput();
#endif

    return;
  }

/*****************************************************************************/
/*!
  \brief
    Updates the mouse and keyboard. Then checks the messages in the queue and
    sets the current state of the mouse and keyboard accordingly.
*/
/*****************************************************************************/
  void InputManager::Update(float dt)
  {
    MSG msg = {0};

    input->Update();

#if USE_EDITOR


#else

    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Query message queue
    {
      Mouse.GetMsg(&msg);
      Keyboard.GetMsg(&msg);

      // dispatch the msg for anything that was not handled.
      DispatchMessage(&msg);
    }

#endif

    return;
  }

  bool InputManager::KeyIsPressed(unsigned int key) const
  {
    dit::Keyboard* k = input->GetKeyboard();
    return k->IsKeyPressed(dit::Keys(key));
  }

  bool InputManager::KeyIsDown(unsigned int key) const
  {
    dit::Keyboard* k = input->GetKeyboard();
    return k->IsKeyDown(dit::Keys(key));
  }

  bool InputManager::KeyIsReleased(unsigned int key) const
  {
    dit::Keyboard* k = input->GetKeyboard();
    return k->IsKeyReleased(dit::Keys(key));
  }

  bool InputManager::ButtonPressed(unsigned int button) const
  {
    return false;
  }

  bool InputManager::ButtonDown(unsigned int button) const
  {
    return false;
  }

  bool InputManager::ButtonReleased(unsigned int button) const
  {
    return false;
  }



}// end namespace
