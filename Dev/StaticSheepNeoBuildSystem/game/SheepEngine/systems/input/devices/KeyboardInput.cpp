

#include "pch/precompiled.h"
#include "KeyboardInput.h"
#include <Windows.h>



namespace Framework
{


/*****************************************************************************/
/*!
  \brief
    Default constructor...
*/
/*****************************************************************************/
  KeyboardInput::KeyboardInput()
  {

  }

/*****************************************************************************/
/*!
  \brief
    Default destructor
*/
/*****************************************************************************/
  KeyboardInput::~KeyboardInput()
  {

  }

/*****************************************************************************/
/*!
  \brief
    Initializes all of the keys to 0. (not pressed)
*/
/*****************************************************************************/
  void KeyboardInput::Initialize()
  {
    for(int i = 0; i < 256; ++i)
    {
      _previousState[i] = false;
      _currentState[i] = false;
    }
  }

/*****************************************************************************/
/*!
  \brief
    Updates the key state to the passed in state.

  \param key
    The index of the bool to be set.

  \param state
    The state to change it to.
*/
/*****************************************************************************/
  void KeyboardInput::UpdateKey(unsigned int key, bool state)
  {
    // we are updating... so set the previous to the current
    _previousState[key] = _currentState[key];

    // then set the current to the new state
    _currentState[key] = state;
  }

/*****************************************************************************/
/*!
  \brief
    Translates a MSG from peekmessage, checks to see if it is a keyboard 
    message, and updates accordingly.

  \param msg
    Pointer to the MSG struct that contains event data.
*/
/*****************************************************************************/
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
      //std::cout << (char)msg->wParam << std::endl;
      UpdateKey(msg->wParam, true);
      break;

    default:
      break;
    }
  }

/*****************************************************************************/
/*!
  \brief
    Sets all of the previous states to the current state.
*/
/*****************************************************************************/
  void KeyboardInput::Update()
  {
    for(int i = 0; i < 256; ++i)
    {
      _previousState[i] = _currentState[i];
    }
  }

/*****************************************************************************/
/*!
  \brief
    Checks to see if a specific key has been pressed this frame.
*/
/*****************************************************************************/
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

/*****************************************************************************/
/*!
  \brief
    Checks to see if a specific key is being held down.
*/
/*****************************************************************************/
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

/*****************************************************************************/
/*!
  \brief
    Checks to see if a specific key has been released this frame.
*/
/*****************************************************************************/
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

/*****************************************************************************/
/*!
  \brief
    Sets the keyboard to active or not... currently doesn't do anything.
*/
/*****************************************************************************/
  void KeyboardInput::SetActive(bool flag)
  {
    _active = flag;
    return;
  }

/*****************************************************************************/
/*!
  \brief
    Gets the active state of the keyboard.
*/
/*****************************************************************************/
  bool KeyboardInput::GetActiveState(void) const
  {
    return _active;
  }
}