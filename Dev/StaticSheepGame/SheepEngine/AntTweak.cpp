/*****************************************************************
Filename: AntTweak.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "AntTweak.h"
#include "AntTweak\AntTweakBar.h"

#include "SheepGraphics.h"

namespace Framework
{
  AntTweak* ATWEAK = nullptr;

  void AntTweak::Initialize()
  {
    m_Valid = GRAPHICS->AntTweakBarLoaded();

    if (m_Valid)
      TwWindowSize(ENGINE->Window.GetWidth(), ENGINE->Window.GetHeight());

    ATWEAK = this;
  }

  void AntTweak::ReceiveMessage(Message msg)
  {
    if (msg.MessageId == Message::PostDraw)
    {
      if (m_Valid)
        TwDraw();
    }
  }

  void AntTweak::Update(float dt)
  {
    
  }

  bool AntTweak::IsValid()
  {
    return m_Valid;
  }
}