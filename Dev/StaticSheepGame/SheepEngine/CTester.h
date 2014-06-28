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
    virtual void Initialize(){};

    int testvalue1;
    int testvalue2;
  };
}