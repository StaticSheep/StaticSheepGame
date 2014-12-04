/*****************************************************************
Filename: GamepadInput.cpp
Project: 
Author(s): Greg Walls

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "GamepadInput.h"

namespace Framework
{
  static const WORD XINPUT_Buttons[] = {
    XINPUT_GAMEPAD_A,
    XINPUT_GAMEPAD_B,
    XINPUT_GAMEPAD_X,
    XINPUT_GAMEPAD_Y,
    XINPUT_GAMEPAD_DPAD_UP,
    XINPUT_GAMEPAD_DPAD_DOWN,
    XINPUT_GAMEPAD_DPAD_LEFT,
    XINPUT_GAMEPAD_DPAD_RIGHT,
    XINPUT_GAMEPAD_LEFT_SHOULDER,
    XINPUT_GAMEPAD_RIGHT_SHOULDER,
    XINPUT_GAMEPAD_LEFT_THUMB,
    XINPUT_GAMEPAD_RIGHT_THUMB,
    XINPUT_GAMEPAD_START,
    XINPUT_GAMEPAD_BACK
  };

  void GamePadInput::Update()
  {
    memcpy(bPrev_ButtonStates, bButtonStates, sizeof(bPrev_ButtonStates));

    XInputGetState(padIndex, &State);

    //iterate through all gamepad buttons
    for(int i = 0; i < ButtonCount; i++)
    {
      //set button state for current frame
      bButtonStates[i] = (State.Gamepad.wButtons & XINPUT_Buttons[i]) == XINPUT_Buttons[i];

      //set 'DOWN' state for current frame
      bGamepad_ButtonsDown[i] = !bPrev_ButtonStates[i] && bButtonStates[i];
    }
  }

  bool GamePadInput::InDeadzone(bool right)
  {

    if(!right)
    {
      //obtain the X and Y axes of the stick
      short sX = State.Gamepad.sThumbLX;
      short sY = State.Gamepad.sThumbLY;

      if(sX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
         sY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
         sX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
         sY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
      {
        return false; //the stick is outside of its deadzone
      }

      return true; //the stick is inside of its deadzone
    }
    else
    {
      //obtain the X and Y axes of the stick
      short sX = State.Gamepad.sThumbRX;
      short sY = State.Gamepad.sThumbRY;

      if(sX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
         sY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
         sX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
         sY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
      {
        return false; //the stick is outside of its deadzone
      }

      return true; //the stick is inside its deadzone
    }
  }

  Vec2 GamePadInput::LeftStick()
  {
    Vec2 leftStick;
    //obtain the x and y axis of the left stick
    leftStick.x = (static_cast<float>(State.Gamepad.sThumbLX) / 32768.0f);
    leftStick.y = (static_cast<float>(State.Gamepad.sThumbLY) / 32768.0f);

    return leftStick;
  }

  Vec2 GamePadInput::RightStick()
  {
    Vec2 rightStick;
    //obtain the x and y axis of the right stick
    rightStick.x = (static_cast<float>(State.Gamepad.sThumbRX) / 32768.0f);
    rightStick.y = (static_cast<float>(State.Gamepad.sThumbRY) / 32768.0f);

    return rightStick;
  }

  float GamePadInput::LeftTrigger()
  {
    //obtain value of the trigger
    BYTE Trigger = State.Gamepad.bLeftTrigger;

    if(Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
      return Trigger / 255.0f;

    return 0.0f; //trigger was not pressed
  }

  float GamePadInput::RightTrigger()
  {
    //obtain value of the trigger
    BYTE Trigger = State.Gamepad.bRightTrigger;

    if(Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
      return Trigger / 255.0f;

    return 0.0f; //trigger was not pressed
  }

  bool GamePadInput::ButtonPressed(int a_iButton)
  {
    
    if (State.Gamepad.wButtons & XINPUT_Buttons[a_iButton] && !bPrev_ButtonStates[a_iButton])
    {
      return true; //the button is pressed
    }

    return false; // the button is not pressed
  }

  bool GamePadInput::ButtonDown(int a_iButton)
  {
    return State.Gamepad.wButtons & XINPUT_Buttons[a_iButton];
  }

  void GamePadInput::SetRightStick(Vec2D& input)
  {
    State.Gamepad.sThumbRX = (BYTE)(input.x * 255);
    State.Gamepad.sThumbRY = (BYTE)(input.y * 255);
  }

  void GamePadInput::Rumble(float a_fLeftMotor, float a_fRightMotor)
  {
    //vibration state
    XINPUT_VIBRATION VibrationState;

    //zero memory
    ZeroMemory(&VibrationState, sizeof(XINPUT_VIBRATION));

    //calculate vibration values
    int iLeftMotor = int(a_fLeftMotor * 65535.0f);
    int iRightMotor = int(a_fRightMotor * 65535.0f);

    //set vibration values
    VibrationState.wLeftMotorSpeed = iLeftMotor;
    VibrationState.wRightMotorSpeed = iRightMotor;

    //set the vibration state
    XInputSetState(padIndex, &VibrationState);
  }

  bool GamePadInput::Connected()
  {
    //zero memory
    ZeroMemory(&State, sizeof(XINPUT_STATE));

    //get the state of the gamepad
    DWORD Result = XInputGetState(padIndex, &State);

    if(Result == ERROR_SUCCESS)
      return true;
    else
      return false;
  }

}// end namespace