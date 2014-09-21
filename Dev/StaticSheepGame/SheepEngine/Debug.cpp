#include "Debug.h"
#include "SheepAudio.h"
#include "SheepGraphics.h"

namespace Framework
{


  Debug::Debug()
  {

  }

  Debug::~Debug()
  {

  }

  void Debug::Initialize()
  {
    audio = (DebugAudio*)AUDIO->GetDebugData();
    graphics = (DebugGraphics*)GRAPHICS->GetDebugData();
  }

  void Debug::Update(float dt)
  {
    
    
  }

}

