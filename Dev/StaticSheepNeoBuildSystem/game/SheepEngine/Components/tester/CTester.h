/*****************************************************************
Filename: CTester.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class Tester : public GameComponent
  {
  public:
    virtual void Initialize();
    void Remove();

    void Update(float dt);

    float timer = 0;
    int phase = 0;

  };
}