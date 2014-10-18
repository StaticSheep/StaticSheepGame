#pragma once
#include <Windows.h>
#include "Xinput.h"

namespace Framework
{
  //XInput button values
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

  //XInput button IDs
  struct XButtonIDs
  {
    //public variables
  
    int A,B,X,Y; //action buttons

    //directional pad (D-Pad)
    struct DirectionalPad
    {
      int Up, Down, Left, Right;
    }DPad;

    //shoulder ('bumber') buttons
    int LShoulder, RShoulder;

    //thumbstick buttons
    int LThumbstick, RThumbstick;

    int Start; // 'Start' button
    int Back;  // 'Back' button

    //function prototypes

    XButtonIDs(); //default constructor

  };

  class GamePad : public GameComponent
  {
  public:
    GamePad();
    ~GamePad() {};
    virtual void Remove();
    //Human based numbering system (i.e. player one is one, player two is two)
    void SetPad(int padNum);

    virtual void Initialize();

    void Update(float dt); //update gamepad state
    void RefreshState(); //update button states for next frame

    //thumbstick functions
    bool LStick_InDeadZone(); //returns true if left stick is inside its "deadzone"
    bool RStick_InDeadZone(); //returns true if right stick is inside its "deadzone"

    float LeftStick_X();  //Returns the X axis of the left stick
    float LeftStick_Y();  //returns the Y axis of the left stick
    float RightStick_X(); //returns the X axis of the right stick
    float RightStick_Y(); //returns the Y axis of the right stick

    //trigger functions
    float LeftTrigger();  //return value of the left trigger
    float RightTrigger(); //return value of the right trigger

    //Button functions
    bool ButtonPressed(int a_iButton); //return true if button is pressed
    //frame-specific verson of ButtonPressed
    bool ButtonDown(int a_iButton);

    //utility functions
    XINPUT_STATE GetState(); // Returns gamepad state
    int GetIndex();          // Returns gamepad index
    bool Connected();        // Return true if gamepad is connected

    //vibrate the gamepad
    void Rumble(float a_fLeftMotor = 0.0f, float a_fRightMotor = 0.0f);
  private:
    XINPUT_STATE State; //current gamepad state
    int GamepadIndex;   //Gamepad index (eg. 1, 2, 3, 4)

    static const int ButtonCount = 14; //total gamepad buttons
    bool bPrev_ButtonStates[ButtonCount]; //previous frame button states
    bool bButtonStates[ButtonCount]; //current frame button states
  
    bool bGamepad_ButtonsDown[ButtonCount]; //Buttons pressed on current frame
  };
  //externally define the XButtonIDs struct as XButtons
  extern XButtonIDs XButtons;
}