/*****************************************************************
Filename: GamepadInput.h
Project: 
Author(s): Greg Walls

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once


#include "systems/System.h"
#include <Windows.h>
#include "Xinput.h"

namespace Framework
{
  class GamePadInput
  {
  public:

    GamePadInput(){ padIndex = 0; };
    GamePadInput(int index)
    {
      padIndex = index;
      ZeroMemory(&State, sizeof(XINPUT_STATE));
    };
    ~GamePadInput(){};

    void Initialize(){};
    void Update();

    /* -------------- Helper Functions -------------- */

    /* -- thumb stick functions -- */

    // Checks if the stick is in the dead zone
    // true if right stick, false if left stick
    // Returns true/false if it is dead zone
    bool InDeadzone(bool right);

    // Returns the position of the sticks
    Vec2 LeftStick();
    Vec2 RightStick();

    /* -- button / trigger functions -- */

    float LeftTrigger();  //return value of the left trigger
    float RightTrigger(); //return value of the right trigger

    bool ButtonPressed(int a_iButton); //return true if button is pressed
    bool ButtonDown(int a_iButton);

    /* -- Misc -- */

    void Rumble(float a_fLeftMotor = 0.0f, float a_fRightMotor = 0.0f);

    bool Connected(); // Is it plugged in?

    /* -------------- Member Variables -------------- */

    int padIndex;
    XINPUT_STATE State;

  private:
    static const int ButtonCount = 14;

    bool bPrev_ButtonStates[ButtonCount]; //previous frame button states
    bool bButtonStates[ButtonCount]; //current frame button states
  
    bool bGamepad_ButtonsDown[ButtonCount]; //Buttons pressed on current frame

  };
}