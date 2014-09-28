#include "Debug.h"
#include "SheepAudio.h"
#include "SheepGraphics.h"
#include "Input.h"
#include "FramerateController.h"

static bool fpsFlag;
static int counter;
static int systemCounter = 31;

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
    framerate = (DebugFramerate*)ENGINE->Framerate.GetDebugData();
  }

  void Debug::Update(float dt)
  {
    
    
  }

  void Debug::ReceiveMessage(Message hamster)
  {
    if (hamster.MessageId == Message::PostDraw)
    {
      

      if(fpsFlag)
      {
        std::string fps_string;
        std::string format;

        if(counter > 30)
        {
          ENGINE->Framerate.GetDebugData();
          counter = 0;
        }
        else
          ++counter;
        format = std::to_string(framerate->currentFps);
        format.erase(4, std::string::npos);
        fps_string = "Current FPS: " + format + "\n";

        GRAPHICS->SetPosition(100.0f, 0.0f);
        GRAPHICS->SetColor(Vec4(1.0f,1.0f,1.0f,1.0f));
        GRAPHICS->DrawSpriteText(fps_string.c_str(), 15.0f, "Helvetica");
      }


      if(GetState())
      {
        switch(currentState)
        {
        case DEBUG_AUDIO:
          if(systemCounter > 60)
          {
            AUDIO->GetDebugData();
            FormatString(currentState);
            systemCounter = 0;
          }
          
          ++systemCounter;

          GRAPHICS->SetPosition(-100.0f, 0.0f);
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


  void Debug::FormatString(int type)
  {
    std::string format1, format2, format3;
    double timetaken;

    switch(type)
    {
    case DEBUG_AUDIO:
      format1 = std::to_string(audio->cpuLoad.streamUsage);
      format2 = std::to_string(audio->cpuLoad.updateUsage);
      format3 = std::to_string(audio->cpuLoad.studioUsage);

      format1.erase(4, std::string::npos );
      format2.erase(4, std::string::npos);
      format3.erase(4, std::string::npos);

      timetaken = framerate->systems["SheepAudio"]->timeTaken * 1000.0;


      string = "Buffer Capacity: " + std::to_string(audio->bufferInfo.studioHandle.capacity) + "\n"
               "Buffer Usage: " + std::to_string(audio->bufferInfo.studioHandle.currentUsage) + "\n"
               "Stream Load: " + format1 + "%\n"
               "Update Load: " + format2 + "%\n"
               "Studio Load: " + format3 + "%\n"
               "RAM Allocated: " + std::to_string(audio->RAM / 1000000) + "." + std::to_string((audio->RAM / 100000) % 10) + " mb\n"
               "Channels Playing: " + std::to_string(audio->channels) + "\n"
               "Total Time Taken : " + std::to_string(timetaken).erase(4,std::string::npos) + "ms\n";
      break;

    default:
      break;

    }

  }


  int Debug::GetState()
  {
    if(SHEEPINPUT->Keyboard.KeyIsPressed(VK_F2))
    {
      fpsFlag = !fpsFlag;
    }

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

