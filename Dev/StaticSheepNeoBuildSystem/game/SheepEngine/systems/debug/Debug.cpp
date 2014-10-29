/******************************************************************************
Filename: Debug.cpp
Project: 
Author(s): Zakary Wilson

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

// debug needs to know about all of the systems.... sigh...
#include "pch/precompiled.h"

#include "systems/debug/Debug.h"
#include "systems/debug/tracelog/TraceLog.h"

#include "systems/audio/SheepAudio.h"
#include "systems/graphics/SheepGraphics.h"
#include "systems/input/Input.h"
#include "engine/framerate/FramerateController.h"
#include "systems/graphics/DrawLib.h"
#include "engine/window/Window32.h"

#include <Windows.h>

static bool fpsFlag;
static bool performanceFlag;
static int counter = 31;
static int systemCounter = 31;
static int performanceCounter = 61;
static float previousTime;
static float currentX;

namespace Framework
{
  static PerformanceData performance;

/*****************************************************************************/
/*!
  \brief
    Default constructor for the Debug class. Doesn't do anything yet...
*/
/*****************************************************************************/
  Debug::Debug()
  {

  }

/*****************************************************************************/
/*!
  \brief
    Destructor for the Debug class. Doesn't do anything yet...
*/
/*****************************************************************************/
  Debug::~Debug()
  {

  }

/*****************************************************************************/
/*!
  \brief
    Sets the pointers to the systems' debug data on initialize.
    GetDebugData returns a void*, so we cast it correctly depending on the
    system. That way custom structs can be passed to the Debug class.
*/
/*****************************************************************************/
  void Debug::Initialize()
  {
    if(AUDIO)
    {
      audio = (DebugAudio*)AUDIO->GetDebugData();
      TRACELOG->Log(DEBUG, "Linked to Audio system\n");
    }

    if(GRAPHICS)
    {
      graphics = (DebugGraphics*)GRAPHICS->GetDebugData();
      TRACELOG->Log(DEBUG, "Linked to Graphics system\n");
    }

    
    framerate = (DebugFramerate*)ENGINE->Framerate.GetDebugData();
    TRACELOG->Log(DEBUG, "Linked to Framerate Controller\n");
  }

/*****************************************************************************/
/*!
  \brief
    Placeholder for update. Otherwise the engine gets sad.
*/
/*****************************************************************************/
  void Debug::Update(float dt)
  {
    
  }

/*****************************************************************************/
/*!
  \brief
    Grabs messages from the engine. If PostDraw is passed, then we might need
    to draw some things.

  \param hamster
    The message hamster!
*/
/*****************************************************************************/
  void Debug::ReceiveMessage(Message hamster)
  {
    if (hamster.MessageId == Message::PostDraw)
    {
      int channels;
      // if F2 was pressed, lets display the FPS
      if(fpsFlag)
      {
        std::string fps_string;
        std::string format;

        // but only update it every half a second
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
        fps_string += "CurrentPosition" + std::to_string(currentX);

        if(SHEEPINPUT->Keyboard.KeyIsDown(VK_LEFT))
          currentX -= 1.0f;

        if(SHEEPINPUT->Keyboard.KeyIsDown(VK_RIGHT))
          currentX += 1.0f;

        Draw::SetRotation(0.0f);
        Draw::SetUseCamera(false);
        Draw::SetPosition( ENGINE->Window->GetWidth() / -2.0f, 0.0f);
        Draw::SetColor(1.0f,1.0f,1.0f,1.0f);
        Draw::DrawString(fps_string.c_str(), 15.0f, "Helvetica");
        Draw::SetUseCamera(true);
      }

      if(performanceFlag)
        DrawPerformance();


      // check which state we might need to print out.
      if(GetState())
      {
        switch(currentState)
        {
        case DEBUG_AUDIO:
          if(systemCounter > 30)
          {
            AUDIO->GetDebugData();
            FormatString(currentState);
            systemCounter = 0;
          }

          channels = (*(FMOD_DSP_PARAMETER_FFT*)audio->data).numchannels;
          Draw::SetUseCamera(false);
          for(int i = 0; i < 1; ++i)
          {
            for(int j = 0; j < 128; ++j)
            {
              float height = ((*(FMOD_DSP_PARAMETER_FFT*)audio->data).spectrum[i][j]);
              Draw::SetColor(1.0f,0.0f,0.0f,1.0f);
              Draw::DrawRect(-128.0f + j * 2.0f, ENGINE->Window->GetHeight() / -3.0f, 2.0f, height * 1000.0f);
            }
          }
          
          Draw::SetColor(1.0f,1.0f,1.0f,1.0f);
          Draw::SetRotation(0.0f);
          Draw::SetPosition(ENGINE->Window->GetWidth() / -4.0f, ENGINE->Window->GetHeight() / 2.0f - 100.0f);
          Draw::DrawString(string.c_str(), 15.0f, "Helvetica");
          Draw::SetUseCamera(true);

          

          break;

        case DEBUG_GRAPHICS:
          GRAPHICS->GetDebugData();
          
          break;
        }
      }

      ++systemCounter;

      return;
    }
  }

/*****************************************************************************/
/*!
  \brief
    Formats the string according to which type of debug data we need to print
    out.

  \param type
    The type of debug info to print out. Passing in an enum.
*/
/*****************************************************************************/
  void Debug::FormatString(int type)
  {
    // these are for formatting floats/doubles
    std::string format1, format2, format3;

    // gets the time in ms that the system took to update
    double timetaken;

    switch(type)
    {
    case DEBUG_AUDIO:
      format1 = std::to_string(audio->cpuLoad.streamUsage);
      format2 = std::to_string(audio->cpuLoad.updateUsage);
      format3 = std::to_string(audio->cpuLoad.studioUsage);

      format1.erase(4, std::string::npos);
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

  void Debug::DrawPerformance()
  {
    double totalTime = 0.0;
    float offsetX = 0.0f;
    int color = 0;
    int i = 0;
    Draw::SetUseCamera(false);

    if(performanceCounter > 30)
    {
      for(auto it = framerate->systems.begin(); it != framerate->systems.end(); ++it)
      {
        totalTime += it->second->timeTaken;
        ++i;
      }

      previousTime = (float)(totalTime * 1000.0f);

      if(!performance.systemCount)
      {
        performance.systemCount = i;

        for(i = 0; i < performance.systemCount; ++i)
        {

          switch(i)
          {
          case 0:
            //246, 34, 23 red
            performance.color[i] = Vec4(0.96f, 0.13f, 0.09f, 1.0f);
            break;

          case 1:
            //53, 126, 199 blue
            performance.color[i] = Vec4(0.21f, 0.49f, 0.78f, 1.0f);
            break;

          case 2:
            // 125, 5, 82 purple
            performance.color[i] = Vec4(0.49f, 0.02f, 0.32f, 1.0f);
            break;

          case 3:
            // 247, 120, 161 pink
            performance.color[i] = Vec4(0.97f, 0.47f, 0.63f, 1.0f);
            break;

          case 4:
            //70, 199, 199 teal
            performance.color[i] = Vec4(0.27f, 0.78f, 0.78f, 1.0f);
            
            break;

          case 5:
            // 248, 114, 23 orange
            performance.color[i] = Vec4(0.97f, 0.56f, 0.09f, 1.0f);
            break;

          case 6:
            // 255, 245, 238 cream
            performance.color[i] = Vec4(1.0f, 0.96f, 0.93f, 1.0f);
            break;

          case 7:
            // 251, 177, 23 yellow
            performance.color[i] = Vec4(0.98f, 0.69f, 0.09f, 1.0f);
            break;

          default:
            performance.color[i] = Vec4(0.0f, 1.0f, 1.0f, 1.0f);
            break;
          }
        }

      }
      i = 0;

      for(auto it = framerate->systems.begin(); it != framerate->systems.end(); ++it)
      {
        performance.times[i] = it->second->timeTaken;
        double percent = it->second->timeTaken / totalTime;
        performance.width[i] = (float)(300.0 * percent);

        performance.pos[i] = (performance.width[i] / 2.0f) + offsetX;
        offsetX += performance.width[i];
       
        
        performance.names[i] = it->first.c_str();
        ++i;
      }

      performanceCounter = 0;
    }

    for(int i = 0; i < performance.systemCount; ++i)
    {
      Draw::SetColor(performance.color[i].R, performance.color[i].G, performance.color[i].B, 1.0f);
      Draw::SetRotation(0.0f);
      
      Draw::DrawRect((ENGINE->Window->GetWidth() / 2.0f) - performance.pos[i] - 4.0f, (ENGINE->Window->GetHeight() / 2.0f) - 16.0f, performance.width[i], 24.0f);

      float x = (ENGINE->Window->GetWidth() / 2.0f) - 296.0f;
      float y = (ENGINE->Window->GetHeight() / 2.0f) + (i * -16.0f) - 48.0f;

      Draw::DrawRect(x, y, 16.0f, 16.0f);
      Draw::SetPosition(x + 24.0f, y);

      std::string newString(performance.names[i]);
      newString += "   " + std::to_string(performance.times[i] * 1000.0f).erase(4,std::string::npos) + "ms";

      Draw::DrawString(newString.c_str(), 15.0f, "Helvetica");
    }
    Draw::SetColor(1.0f,1.0f,1.0f,1.0f);
    Draw::SetPosition((ENGINE->Window->GetWidth() / 2.0f) - 296.0f, (ENGINE->Window->GetHeight() / 2.0f - 16.0f));

    std::string stringTime = std::to_string(previousTime).erase(4,std::string::npos) + "ms";
    Draw::DrawString(stringTime.c_str(), 15.0f, "Helvetica");

    Draw::SetUseCamera(true);
    ++performanceCounter;

  }


/*****************************************************************************/
/*!
  \brief
    Gets the current keyboard state for which debug info to print out.
*/
/*****************************************************************************/
  int Debug::GetState()
  {
    // F2 for FPS printing
    if(SHEEPINPUT->Keyboard.KeyIsPressed(VK_F2))
    {
      fpsFlag = !fpsFlag;
    }

    if(SHEEPINPUT->Keyboard.KeyIsPressed(VK_F3))
    {
      performanceFlag = !performanceFlag;
    }

    // F5 for audio debug
    if(SHEEPINPUT->Keyboard.KeyIsPressed(VK_F5))
    {
      if(currentState == DEBUG_AUDIO)
        currentState = 0;
      else
        currentState = DEBUG_AUDIO;
    }

    // F6 for graphics debug
    if(SHEEPINPUT->Keyboard.KeyIsPressed(VK_F6))
    {
      if(currentState == DEBUG_GRAPHICS)
        currentState = 0;
      else
        currentState = DEBUG_GRAPHICS;
    }

    // Add more states here.

    return currentState;
  }
} // end namespace
 