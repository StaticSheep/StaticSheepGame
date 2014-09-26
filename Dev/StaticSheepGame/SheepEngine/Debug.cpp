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

  void Debug::ReceiveMessage(Message msg)
  {
    if (msg.MessageId == Message::PostDraw)
    {

      if(GetState())
      {
        std::string string;

        switch(currentState)
        {
        case DEBUG_AUDIO:
          AUDIO->GetDebugData();
          // print that shit after this...
          /*
          sprintf(buffers[0], "DSP    Load %% \t: %f%%\n", audio->cpuLoad.dspUsage);
          sprintf(buffers[1], "Stream Load %% \t: %f%%\n", audio->cpuLoad.streamUsage);
          sprintf(buffers[2], "Update Load %% \t: %f%%\n", audio->cpuLoad.updateUsage);*/
          

          string = "DSP Load : " + std::to_string(audio->cpuLoad.dspUsage) + "%\n" +
                   "Stream Load : " +  std::to_string(audio->cpuLoad.streamUsage) + "%\n" +
                   "Update Load : " + std::to_string(audio->cpuLoad.updateUsage) + "%\n" +
                   "Studio Load : " + std::to_string(audio->cpuLoad.studioUsage) + "%\n" +
                   "RAM Allocated : " + std::to_string(audio->RAM) + " bytes\n" +
                   "Channels Playing : " + std::to_string(audio->channels);

          //sprintf(buffer, "Studio Load %% \t: %f%%\n", audio->cpuLoad.studioUsage);
          /*
          sprintf(buffers[4], "Channels Playing: %i\n", audio->channels);
          sprintf(buffers[5], "RAM Allocated: %i\n", audio->RAM);*/

          GRAPHICS->DrawSpriteText(string.c_str(), 10.0f, "Helvetica");
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

