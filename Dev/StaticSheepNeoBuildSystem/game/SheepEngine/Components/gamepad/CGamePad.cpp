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
    //space->hooks.Remove("FrameUpdate", self);
  }

  //sets the controller
  void GamePad::SetPad(int padNum)
  {
     //set gamepad
    GamepadIndex = padNum;
  }

  void GamePad::EditorSetPad(void* padNum)
  {
    SetPad(*(int*)padNum);
  }

  //initialize the component
  void GamePad::Initialize()
  {
    SetPad(GamepadIndex);
    //space->hooks.Add("FrameUpdate", self, BUILD_FUNCTION(GamePad::FrameUpdate));
  }

  //////////////////////////////////////////////////////////////////////
  //update
  //////////////////////////////////////////////////////////////////////

  //update gamepad state
  void GamePad::FrameUpdate(float dt)
  {
  }

  //update button states for next frame
  void GamePad::RefreshState()
  {
  }

  /////////////////////////////////////////////////////////////////////
  //thumbsticks
  ////////////////////////////////////////////////////////////////////

  //Returns true if the left stick is inside its "deadzone"
  bool GamePad::LStick_InDeadZone()
  {
    return SHEEPINPUT->Pads[GamepadIndex].InDeadzone(false);
  }

  //return true if the right stick is inside its deadzone
  bool GamePad::RStick_InDeadZone()
  {
    return SHEEPINPUT->Pads[GamepadIndex].InDeadzone(true);
  }

  //return the x axis of the left stick
  float GamePad::LeftStick_X()
  {
    return SHEEPINPUT->Pads[GamepadIndex].LeftStick().x;
  }

  //return the Y axis of the left stick
  float GamePad::LeftStick_Y()
  {
    return SHEEPINPUT->Pads[GamepadIndex].LeftStick().y;
  }

  //return the X axis of the right stick
  float GamePad::RightStick_X()
  {
    return SHEEPINPUT->Pads[GamepadIndex].RightStick().x;
  }

  //return the Y axis of the right stick
  float GamePad::RightStick_Y()
  {
    return SHEEPINPUT->Pads[GamepadIndex].RightStick().y;
  }

  ////////////////////////////////////////////////////////////////////
  //trigger functions
  ////////////////////////////////////////////////////////////////////

  //return value of the left trigger
  float GamePad::LeftTrigger()
  {
    return SHEEPINPUT->Pads[GamepadIndex].LeftTrigger();
  }

  float GamePad::RightTrigger()
  {
    return SHEEPINPUT->Pads[GamepadIndex].RightTrigger();
  }

  ////////////////////////////////////////////////////////////////////
  //Button press
  ///////////////////////////////////////////////////////////////////

  //return true if button is pressed
  bool GamePad::ButtonPressed(int a_iButton)
  {
    return SHEEPINPUT->Pads[GamepadIndex].ButtonPressed(a_iButton);
  }

  //frame-specific version of 'ButtonPressed' function
  bool GamePad::ButtonDown(int a_iButton)
  {
    return SHEEPINPUT->Pads[GamepadIndex].ButtonDown(a_iButton);
  }

  //return gamepad index
  int GamePad::GetIndex()
  {
    return GamepadIndex;
  }
  //return true if the gamepad is connected
  bool GamePad::Connected()
  {
    return SHEEPINPUT->Pads[GamepadIndex].Connected();
  }

  //vibrate the gamepad (values of 0.0f to 1.0f only)
  void GamePad::Rumble(float a_fLeftMotor, float a_fRightMotor)
  {
    SHEEPINPUT->Pads[GamepadIndex].Rumble(a_fLeftMotor, a_fRightMotor);
  }
}