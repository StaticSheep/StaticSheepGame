#include "Debug.h"
#include "SheepAudio.h"
#include "SheepGraphics.h"
#include "Input.h"

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
    //graphics = (DebugGraphics*)GRAPHICS->GetDebugData();

    // add hook to post draw

  }

  void Debug::Update(float dt)
  {
    
    
  }

  void Debug::ReceiveMessage(Message hamster)
  {
    if (hamster.MessageId == Message::PostDraw)
    {

      if(GetState())
      {
        std::string string;
        std::string format;

        switch(currentState)
        {
        case DEBUG_AUDIO:
          AUDIO->GetDebugData();
          string = "Buffer Capacity:\t" + std::to_string(audio->bufferInfo.studioHandle.capacity) + "\n";
          string += "Buffer Usage:\t" + std::to_string(audio->bufferInfo.studioHandle.currentUsage) + "\n";
          format = std::to_string(audio->cpuLoad.streamUsage);
          format.erase(4, std::string::npos );

          string += "Stream Load:\t" + format + "%\n";

          format = std::to_string(audio->cpuLoad.updateUsage);
          format.erase(4, std::string::npos);

          string += "Update Load:\t" + format + "%\n";

          format = std::to_string(audio->cpuLoad.studioUsage);
          format.erase(4, std::string::npos);

          string += "Studio Load:\t" + format + "%\n";

          string += "RAM Allocated:\t" + std::to_string(audio->RAM / 1000000) + "." + std::to_string((audio->RAM / 100000) % 10) + " mb\n";
          string += "Channels Playing:\t" + std::to_string(audio->channels);



          GRAPHICS->SetPosition(-100.0f, 200.0f);
          GRAPHICS->DrawSpriteText(string.c_str(), 15.0f, "Helvetica");

          break;

        case DEBUG_GRAPHICS:
          GRAPHICS->GetDebugData();
          // print that shit
          break;
        }
      }

      

      return;
    }
  }


  int Debug::GetState()
  {
    if(SHEEPINPUT->Keyboard.KeyIsPressed(VK_F5))
    {
      if(currentState == DEBUG_AUDIO)
        currentState = 0;
      else
        currentState = DEBUG_AUDIO;
    }

    if(SHEEPINPUT->Keyboard.KeyIsPressed(VK_F6))
    {
      if(currentState == DEBUG_GRAPHICS)
        currentState = 0;
      else
        currentState = DEBUG_GRAPHICS;
    }

    return currentState;

  }

}

