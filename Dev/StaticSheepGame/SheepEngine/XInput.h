#pragma once
#include <Windows.h>
#include <XInput.h>

namespace Framework
{
  namespace Input
  {
    class XInput : public ISystem
    {
    private:
      GamePad controllers[4];
    public:
      XInput();

      ~XInput() {};

      void Update(float dt);
      std::string GetName() {return "XInput";};
    };

    class GamePad
    {
      private:
        XINPUT_STATE State; //current gamepad state
        int GamepadIndex;   //Gamepad index (eg. 1, 2, 3, 4)

        static const int ButtonCount = 14; //total gamepad buttons
        bool bPrev_ButtonStates[ButtonCount]; //previous frame button states
        bool bButtonStates[ButtonCount]; //current frame button states
  
        bool bGamepad_ButtonsDown[ButtonCount]; //Buttons pressed on current frame

      public:
        //Constructors
        GamePad();
        GamePad(int a_iIndex);

        void Update(); //update gamepad state
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
    };
  }
}