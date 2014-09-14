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

  AntTweak::AntTweak()
  {
    ATWEAK = this;
  }

  void AntTweak::Initialize()
  {
  }

  void AntTweak::ReceiveMessage(Message msg)
  {
    if (msg.MessageId == Message::PostDraw)
    {
      TwDraw();
      return;
    }
    if (msg.MessageId == Message::GFXDeviceInit)
    {
      TwInit(TW_DIRECT3D11, GRAPHICS->GetDevice());
      TwWindowSize(ENGINE->Window.GetWidth(), ENGINE->Window.GetHeight());
      return;
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