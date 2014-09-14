/*****************************************************************
Filename: AntTweak.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "System.h"

namespace Framework
{
  class AntTweak : public ISystem
  {
  public:
    AntTweak();

    void Initialize(void);

    // Update per frame
    void Update(float dt);

    void ReceiveMessage(Message msg);

    bool IsValid();

    //virtual void RegisterComponents();

    virtual std::string GetName() {return "AntTweakBar";};
  private:
    bool m_Valid;
  };

  extern AntTweak* ATWEAK;
}
