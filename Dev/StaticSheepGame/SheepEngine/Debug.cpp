#include "Debug.h"
#include "SheepAudio.h"

class SheepAudio;

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

  }

  void Debug::Update(float dt)
  {
    audio = (DebugAudio*)AUDIO->GetDebugData();
  }
  
  void* Debug::GetDebugData(void)
  {
    return nullptr; 
  }

}

