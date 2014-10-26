#include "pch/precompiled.h"
#include "CGamePad.h"
#include "systems/input/Input.h"
#pragma comment(lib, "Xinput9_1_0.lib")

namespace Framework
{
  //Define the XButtonIDs struct as XButtons
  XButtonIDs XButtons;

  //'XButtonIDs' - default constructor
  XButtonIDs::XButtonIDs()
  {
    //These valus are used to index the XINPUT_Buttons array,
    //accessing the matching XINPUT button value

    A = 0;
    B = 1;
    X = 2;
    Y = 3;

    DPad.Up = 4;
    DPad.Down = 5;
    DPad.Left = 6;
    DPad.Right = 7;

    LShoulder = 8;
    RShoulder = 9;

    Start = 12;
    Back = 13;

    LThumbstick = 10;
    RThumbstick = 11;
  }
  //default constructor
  GamePad::GamePad() {GamepadIndex = 0;}
  
  //remove the component
  void GamePad::Remove()
  {
    space->hooks.Remove("FrameUpdate", self);
  }

  //sets the controller
  void GamePad::SetPad(int padNum)
  {
     //set gamepad
    GamepadIndex = padNum;

    //Iterate through all gamepad buttons
    for(int i = 0; i < ButtonCount; i++)
    {
      bPrev_ButtonStates[i]   = false;
      bButtonStates[i]        = false;
      bGamepad_ButtonsDown[i] = false;
    }
  }

  void GamePad::EditorSetPad(void* padNum)
  {
    SetPad(*(int*)padNum);
  }

  //initialize the component
  void GamePad::Initialize()
  {
    SetPad(GamepadIndex);
    space->hooks.Add("FrameUpdate", self, BUILD_FUNCTION(GamePad::FrameUpdate));
  }

  //////////////////////////////////////////////////////////////////////
  //update
  //////////////////////////////////////////////////////////////////////

  //update gamepad state
  void GamePad::FrameUpdate(float dt)
  {
    RefreshState(); //store old button presses

    State = GetState(); //obtain current gamepad state

    //iterate through all gamepad buttons
    for(int i = 0; i < ButtonCount; i++)
    {
      //set button state for current frame
      bButtonStates[i] = (State.Gamepad.wButtons & XINPUT_Buttons[i]) == XINPUT_Buttons[i];

      //set 'DOWN' state for current frame
      bGamepad_ButtonsDown[i] = !bPrev_ButtonStates[i] && bButtonStates[i];
    }
  }

  //update button states for next frame
  void GamePad::RefreshState()
  {
    memcpy(bPrev_ButtonStates, bButtonStates, sizeof(bPrev_ButtonStates));
  }

  /////////////////////////////////////////////////////////////////////
  //thumbsticks
  ////////////////////////////////////////////////////////////////////

  //Returns true if the left stick is inside its "deadzone"
  bool GamePad::LStick_InDeadZone()
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

  //return true if the right stick is inside its deadzone
  bool GamePad::RStick_InDeadZone()
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

  //return the x axis of the left stick
  float GamePad::LeftStick_X()
  {
    //obtain the x axis of the left stick
    short sX = State.Gamepad.sThumbLX;

    //convert the value to a float
    float fX = (static_cast<float>(sX) / 32768.0f);

    return fX;
  }

  //return the Y axis of the left stick
  float GamePad::LeftStick_Y()
  {
    //obtain the Y axis of the left stick
    short sY = State.Gamepad.sThumbLY;

    //convert the value to a float
    float fY = (static_cast<float>(sY) / 32768.0f);

    return fY;
  }

  //return the X axis of the right stick
  float GamePad::RightStick_X()
  {
    //Obtain the X axis of th eright stick
    short sX = State.Gamepad.sThumbRX;

    //convert the value to a float
    float fX = (static_cast<float>(sX) / 32768.0f);

    return fX;
  }

  //return the Y axis of the right stick
  float GamePad::RightStick_Y()
  {
    //Obtain the Y axis of the right stick
    short sY = State.Gamepad.sThumbRY;

    //convert the value to a float
    float fY = (static_cast<float>(sY) / 32768.0f);

    return fY;
  }

  ////////////////////////////////////////////////////////////////////
  //trigger functions
  ////////////////////////////////////////////////////////////////////

  //return value of the left trigger
  float GamePad::LeftTrigger()
  {
    //obtain value of the trigger
    BYTE Trigger = State.Gamepad.bLeftTrigger;

    if(Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
      return Trigger / 255.0f;

    return 0.0f; //trigger was not pressed
  }

  float GamePad::RightTrigger()
  {
    //obtain value of the trigger
    BYTE Trigger = State.Gamepad.bRightTrigger;

    if(Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
      return Trigger / 255.0f;

    return 0.0f; //trigger was not pressed
  }

  ////////////////////////////////////////////////////////////////////
  //Button press
  ///////////////////////////////////////////////////////////////////

  //return true if button is pressed
  bool GamePad::ButtonPressed(int a_iButton)
  {
    if(State.Gamepad.wButtons & XINPUT_Buttons[a_iButton])
    {
      return true; //the button is pressed
    }

    return false; // the button is not pressed
  }

  //frame-specific version of 'ButtonPressed' function
  bool GamePad::ButtonDown(int a_iButton)
  {
    return bGamepad_ButtonsDown[a_iButton];
  }

  ////////////////////////////////////////////////////////////////////
  //utility functions
  ////////////////////////////////////////////////////////////////////

  //Returns the gamepads state
  XINPUT_STATE GamePad::GetState()
  {
    //temporary XINPUT_STATE to return
    XINPUT_STATE GamePadState;

    //Zero memory
    ZeroMemory(&GamePadState, sizeof(XINPUT_STATE));

    //Get the state of the gamepad
    SHEEPINPUT->GetGamePadState(GamepadIndex, &GamePadState);

    //return the gampad state
    return GamePadState;
  }

  //return gamepad index
  int GamePad::GetIndex()
  {
    return GamepadIndex;
  }
  //return true if the gamepad is connected
  bool GamePad::Connected()
  {
    //zero memory
    ZeroMemory(&State, sizeof(XINPUT_STATE));

    //get the state of the gamepad
    DWORD Result = XInputGetState(GamepadIndex, &State);

    if(Result == ERROR_SUCCESS)
      return true;
    else
      return false;
  }

  //vibrate the gamepad (values of 0.0f to 1.0f only)
  void GamePad::Rumble(float a_fLeftMotor, float a_fRightMotor)
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
    XInputSetState(GamepadIndex, &VibrationState);

  }
}