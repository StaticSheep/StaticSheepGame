/******************************************************************************
Filename: FramerateController.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "pch/precompiled.h"

#include "engine/core/Engine.h"
#include <Windows.h>

// needed to multiply numbers, since we are dividing by a lot and we don't want
// to lose resolution. Also for getting miliseconds and not microseconds
#define WILSONS_CONSTANT1 1000000000

namespace Framework
{
/*****************************************************************************/
/*!
  \brief
    Default constructor for the FramerateController class. Sets FPS to 60.
*/
/*****************************************************************************/
  FramerateController::FramerateController()
  {
    frameTime = new LARGE_INTEGER; // MAKE SOME BIG GODDAMN INTEGERS
    systemTime = new LARGE_INTEGER;
    frequency = new LARGE_INTEGER;
    fps = 1.0 / 60.0; // default to 60 fps

    // get time now
    QueryPerformanceCounter((LARGE_INTEGER*)frameTime);
    // get the frequency
    QueryPerformanceFrequency((LARGE_INTEGER*)frequency);
  }

/*****************************************************************************/
/*!
  \brief
    Non-Default constructor for the FramerateController class. Sets FPS to the
    passed in double.
*/
/*****************************************************************************/
  FramerateController::FramerateController(double fps)
  {
    frameTime = new LARGE_INTEGER; // MAKE SOME BIG GODDAMN INTEGERS
    systemTime = new LARGE_INTEGER;
    frequency = new LARGE_INTEGER;
    fps = 1.0 / fps; // get the set fps

    // get time now
    QueryPerformanceCounter((LARGE_INTEGER*)frameTime);
    // get the frequency of the hardware
    QueryPerformanceFrequency((LARGE_INTEGER*)frequency);
  }

/*****************************************************************************/
/*!
  \brief
    Destructor for Framerate, just get right of some dynamically allocated
    things.
*/
/*****************************************************************************/
  FramerateController::~FramerateController()
  {
    for (auto it = debug.systems.begin(); it != debug.systems.end(); ++it)
    {
      delete it->second;
      it->second = 0;
    }

    delete frequency;
    delete systemTime;
    delete frameTime;

  }

/*****************************************************************************/
/*!
  \brief
    For setting up debug information about each system being added to the 
    engine. This is later used to test how long each system takes to update.
*/
/*****************************************************************************/
  void FramerateController::Initialize(void)
  {
    for (int i = 0; i < ENGINE->m_systems.size(); ++i)
    {
      debug.systems[ENGINE->m_systems[i]->GetName()] = (new DebugSystem);
    }
  }

/*****************************************************************************/
/*!
  \brief
    Used by the Debug class to get the current fps.
*/
/*****************************************************************************/
  void* FramerateController::GetDebugData()
  {
    debug.currentFps = (float)currentFps;
    return (void*)&debug;
  }

/*****************************************************************************/
/*!
  \brief
    Checks to see if enough time has passed since the last frame to continue
    another frame of update.
*/
/*****************************************************************************/
  bool FramerateController::FramerateCheck()
  {
    LARGE_INTEGER currentTime, elapsedTime;

    // get the current time
    QueryPerformanceCounter(&currentTime);

    // check how much time has elapsed since the last frame
    elapsedTime.QuadPart = currentTime.QuadPart - ((LARGE_INTEGER*)frameTime)->QuadPart;
    elapsedTime.QuadPart *= WILSONS_CONSTANT1; // for resolution of the number
    elapsedTime.QuadPart /= ((LARGE_INTEGER*)frequency)->QuadPart;

    if((double)elapsedTime.QuadPart / WILSONS_CONSTANT1 > fps)
    {
      ((LARGE_INTEGER*)frameTime)->QuadPart = currentTime.QuadPart;
      dt = (double)elapsedTime.QuadPart / WILSONS_CONSTANT1;
      currentFps = 1.0 / dt;
      debug.totalTime = 0.0;
      return true;
    }

    return false;
  }

/*****************************************************************************/
/*!
  \brief
    Used at the start of a system/module, sets the start time.
*/
/*****************************************************************************/
  void FramerateController::StartFrame()
  {
    QueryPerformanceCounter((LARGE_INTEGER*)systemTime);
  }

/*****************************************************************************/
/*!
  \brief
    Used at the end of a system/module, calculates how long the system/module
    took to update.
*/
/*****************************************************************************/
  double FramerateController::EndFrame(const char* name)
  {
    LARGE_INTEGER currentTime, elapsedTime;

    QueryPerformanceCounter(&currentTime);
    elapsedTime.QuadPart = currentTime.QuadPart - ((LARGE_INTEGER*)systemTime)->QuadPart;
    elapsedTime.QuadPart *= WILSONS_CONSTANT1;
    elapsedTime.QuadPart /= ((LARGE_INTEGER*)frequency)->QuadPart;

    debug.systems[name]->timeTaken = (float)(elapsedTime.QuadPart / (double)WILSONS_CONSTANT1);
    debug.totalTime += debug.systems[name]->timeTaken;
    
    return (double)elapsedTime.QuadPart / (double)WILSONS_CONSTANT1;
  }

/*****************************************************************************/
/*!
  \brief
    Gets the accurate and current delta time.
*/
/*****************************************************************************/
  float FramerateController::GetDT()
  {
    return 0.0167f; // deterministic dt
  }

/*****************************************************************************/
/*!
  \brief
    Gets the current fps of the system.
*/
/*****************************************************************************/
  float FramerateController::GetCurrentFPS()
  {
    return (float)currentFps;
  }

} // end namespace
