/*****************************************************************
Filename: MouseInput.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class MouseInput
  {
  public:

    MouseInput();
    ~MouseInput();

    void Initialize(void);
    void Update(void);

    /*---------- Getters ----------*/

    Vec2 GetWorldPosition(void);
    Vec2 GetScreenPosition(void);
  
    /*---------- Button States ----------*/

    bool ButtonPressed(unsigned int button) const;
    bool ButtonDown(unsigned int button) const;
    bool ButtonReleased(unsigned int button) const;

  private:

    /*---------- States ----------*/

    bool _previousState[3];
    bool _currentState[3];

    /*---------- Positions ----------*/

    Vec2 _screenPosition;
    Vec2 _worldPosition;
    void GetMsg(void* msg);

    void _UpdateButton(unsigned int, bool state);
    void _UpdateMove(void* msg);

    void _ScreenToWorld();
    
    // let the manager touch our privates
    friend class InputManager;

  };
}