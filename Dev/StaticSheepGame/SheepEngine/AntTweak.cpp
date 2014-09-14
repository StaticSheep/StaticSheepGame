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
  void AntTweak::Initialize()
  {
    TwInit(TW_DIRECT3D11, GRAPHICS->GetDevice());
    TwWindowSize(400, 300);
    //TwWindowSize(ENGINE->Window.GetWidth(), ENGINE->Window.GetHeight());
  }

  void AntTweak::ReceiveMessage(Message msg)
  {
    if (msg.MessageId == Message::PostDraw)
    {
       TwDraw();
    }
  }

  void AntTweak::Update(float dt)
  {
    
  }
}