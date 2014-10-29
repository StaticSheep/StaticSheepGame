/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "API.h"
#include "Keyboard.h"

namespace dit {

    //-----------------------------------//

    Keyboard::Keyboard()
    {
        Reset();
    }

    //-----------------------------------//

    bool Keyboard::IsKeyDown(Keys keycode) const
    {
        return keyState[(uint32) keycode];
    }

    //-----------------------------------//

    bool Keyboard::IsKeyPressed(Keys keyCode) const
    {
      if(keyState[(int) keyCode] && !prevKeyState[(int) keyCode])
        return true;
      return false;
    }

    //-----------------------------------//

    bool Keyboard::IsKeyReleased(Keys keyCode) const
    {
      if(!keyState[(int) keyCode] && prevKeyState[(int)keyCode])
        return true;
      return false;
    }

    //-----------------------------------//

    void Keyboard::Reset()
    {
        std::fill(keyState.begin(), keyState.end(), false);
        std::fill(prevKeyState.begin(), prevKeyState.end(), false);
    }

    void Keyboard::Update()
    {
      for(unsigned int i = 0; i < prevKeyState.size(); ++i)
      {
        prevKeyState[i] = keyState[i];
      }
    }

    //-----------------------------------//

    void Keyboard::ProcessEvent(const InputEvent& event)
    {
        if (event.deviceType != InputDeviceType::Keyboard)
            return;

        const KeyEvent& keyEvent = static_cast<const KeyEvent&>(event);

        switch (keyEvent.eventType)
        {
        case KeyboardEventType::KeyPressed:
        {
            KeyPressed(keyEvent);
            break;
        }
        case KeyboardEventType::KeyReleased:
        {
            KeyReleased(keyEvent);
            break;
        }
        case KeyboardEventType::KeyText:
        {
            for (const auto& fn : onKeyText)
                fn(keyEvent);
            break;
        }
        }
    }

    //-----------------------------------//

    void Keyboard::KeyPressed(const KeyEvent& keyEvent)
    {
        // Ignore automatic key repeat.
        if (keyState[(int) keyEvent.keyCode])
            return;

        keyState[(int) keyEvent.keyCode] = true;

        //prevKeyState[(int)keyEvent.keyCode] = false; //z

        lastKey = keyEvent.keyCode;

        for (const auto& fn : onKeyPress)
            fn(keyEvent);
    }

    //-----------------------------------//

    void Keyboard::KeyReleased(const KeyEvent& keyEvent)
    {
      if (!keyState[(int)keyEvent.keyCode])
        return;

      keyState[(int)keyEvent.keyCode] = false;

      //prevKeyState[(int)keyEvent.keyCode] = true; //z

      for (const auto& fn : onKeyRelease)
        fn(keyEvent);
    }

    //-----------------------------------//

    KeyEvent::KeyEvent(KeyboardEventType eventType)
        : InputEvent(InputDeviceType::Keyboard)
        , eventType(eventType)
    { }

    //-----------------------------------//

}